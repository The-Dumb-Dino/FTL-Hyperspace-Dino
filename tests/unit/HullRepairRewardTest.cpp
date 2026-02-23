#include "../utils/TestUtils.h"
#include "../../Global.h"

/**
 * Test: Hull Repair Reward Window
 *
 * Tests that hull repair shows "Hull already repaired" when hull is full.
 * Uses proper event flow through WorldManager::CreateChoiceBox.
 */

static void HullRepairRewardTest(TestFramework::Test& test, TestFramework::TestStages& stages)
{
    test.section("Hull Repair Reward Window Test");

    // Verify game started
    stages.addStage("Verify game started", [&test]() {
        test.assertTrue(GameAccess::State::isInGame(), "Game is running");
        test.requireNotNull(GameAccess::State::getPlayerShip(), "Player ship exists");
    }, 0);

    // Verify hull is at max before test
    stages.addStage("Verify hull is full", [&test]() {
        ShipManager* ship = G_->GetShipManager(0);
        test.requireNotNull(ship, "Player ship accessible");

        int currentHull = ship->ship.hullIntegrity.first;
        int maxHull = ship->ship.hullIntegrity.second;

        test.log("Hull before test: " + std::to_string(currentHull) + "/" + std::to_string(maxHull));
        test.requireEquals(currentHull, maxHull, "Hull is at maximum");
    }, 0);

    // Create and show reward event with 30 hull repair using proper event flow
    stages.addStage("Create hull repair event", [&test]() {
        WorldManager* world = G_->GetWorld();
        test.requireNotNull(world, "WorldManager available");

        // Create a LocationEvent with hull repair
        LocationEvent event;
        event.stuff.hullDamage = -30;  // 30 hull repair
        event.stuff.systemId = -1;     // No system reward
        event.text.data = "A friendly ship offers to repair your hull!";
        event.text.isLiteral = true;

        // Add a choice
        LocationEvent* choiceEvent = new LocationEvent();
        choiceEvent->text.data = "Continue...";
        choiceEvent->text.isLiteral = true;
        ChoiceReq req;
        event.AddChoice(choiceEvent, "Accept the repair", req, false);

        // Use proper event flow - this should go through ModifyResources
        world->CreateChoiceBox(&event);

        test.log("Created hull repair event with proper event flow");
    }, 50);

    // Verify hull state from dialogue's ResourceEvent
    stages.addStage("Verify dialogue hull data", [&test]() {
        CommandGui* gui = GameAccess::State::getCommandGui();
        test.requireNotNull(gui, "CommandGui available");

        // Access the ResourceEvent stored in the ChoiceBox
        int hullDamage = gui->choiceBox.rewards.hullDamage;

        // Get actual hull state
        ShipManager* ship = G_->GetShipManager(0);
        test.requireNotNull(ship, "Player ship accessible");
        int currentHull = ship->ship.hullIntegrity.first;
        int maxHull = ship->ship.hullIntegrity.second;

        test.log("Dialogue hullDamage: " + std::to_string(hullDamage));
        test.log("Ship hull: " + std::to_string(currentHull) + "/" + std::to_string(maxHull));

        // When hull is full, hullDamage should be 0 (adjusted by ModifyResources)
        // If this fails, the bug is confirmed
        test.assertTrue(hullDamage == 0 || currentHull < maxHull,
            "Hull repair should be 0 when hull is full");
    }, 0);

    // Wait and take screenshot
    stages.addStage("Take screenshot", [&test]() {
        TestHelpers::takeScreenshot("hull_repair_reward_test.bmp");
        test.log("Screenshot taken");
    }, 20);
}

// Auto-register with seeded gameplay
static TestFramework::TestRegistrar _("HullRepairReward", HullRepairRewardTest, "NewGameSeeded");
