#include "../utils/TestUtils.h"
#include "../../Global.h"

/**
 * Integration Test: Merchant Jump Save to be used with Multiverse mod (will be moved to MV tests folder)
 *
 * Tests loading a save and jumping to a merchant beacon.
 * Uses the SavedGame scenario with mv-merchant save.
 */

static void MerchantJumpSaveTest(TestFramework::Test& test, TestFramework::TestStages& stages)
{
    test.section("Merchant Jump Save Test");

    // Verify starting state
    stages.addStage("Verify starting state", [&test]() {
        test.assertTrue(GameAccess::State::getPlayerHull() > 0, "Player has positive hull");
        test.assertTrue(GameAccess::State::getPlayerScrap() >= 0, "Player has valid scrap count");
    }, 0);

    // Open star map
    stages.addStage("Open star map", [&test]() {
        StarMap* starMap = GameAccess::State::getStarMap();
        test.requireNotNull(starMap, "StarMap is available");

        starMap->Open();
        test.log("Called starMap->Open()");
        test.log("bOpen: " + std::to_string(starMap->bOpen));
    }, 50);

    // Find and select merchant beacon
    stages.addStage("Select merchant beacon", [&test]() {
        StarMap* starMap = GameAccess::State::getStarMap();
        test.requireNotNull(starMap, "StarMap still available");

        Location* currentLoc = starMap->currentLoc;
        test.requireNotNull(currentLoc, "Current location exists");

        // Log all connected locations with screen coordinates
        test.log("Connected locations from current (with screen coords):");
        int idx = 0;
        for (auto& loc : currentLoc->connectedLocations)
        {
            int screenX = starMap->position.x + (int)loc->loc.x;
            int screenY = starMap->position.y + (int)loc->loc.y;
            std::string info = "  [" + std::to_string(idx) + "] ";
            info += "screen=(" + std::to_string(screenX) + "," + std::to_string(screenY) + ")";
            info += ", visited=" + std::to_string(loc->visited);
            if (loc->event)
            {
                info += ", store=" + std::to_string(loc->event->store);
            }
            test.log(info);
            idx++;
        }

        // Also log current location screen pos
        int curX = starMap->position.x + (int)currentLoc->loc.x;
        int curY = starMap->position.y + (int)currentLoc->loc.y;
        test.log("Current loc screen: (" + std::to_string(curX) + "," + std::to_string(curY) + ")");

        // Select location index 4 (merchant beacon in mv-merchant save)
        int merchantIndex = 4;
        test.requireTrue(currentLoc->connectedLocations.size() > merchantIndex,
            "Location index " + std::to_string(merchantIndex) + " exists");

        Location* merchantLoc = currentLoc->connectedLocations[merchantIndex];
        test.log("Selecting merchant beacon at index " + std::to_string(merchantIndex));

        // Need position + translation + loc to get screen coordinates
        int clickX = starMap->position.x + (int)starMap->translation.x + (int)merchantLoc->loc.x;
        int clickY = starMap->position.y + (int)starMap->translation.y + (int)merchantLoc->loc.y;
        test.log("Will click at: (" + std::to_string(clickX) + "," + std::to_string(clickY) + ")");

        // Draw debug marker and screenshot
        GL_Color red = {1.0f, 0.0f, 0.0f, 1.0f};
        CSurface::GL_DrawRect(clickX - 5, clickY - 5, 10, 10, red);
        TestHelpers::takeScreenshot("click_debug.bmp");

        // Now click through CommandGui
        CommandGui* gui = GameAccess::State::getCommandGui();
        gui->MouseMove(clickX, clickY);
        gui->LButtonDown(clickX, clickY, false);
        test.log("Clicked on location");
    }, 500);

    // Take screenshot
    stages.addStage("Take screenshot", [&test]() {
        if (!TestHelpers::takeScreenshot("merchant_jump_test.bmp"))
        {
            test.fail("Screenshot", "Failed to capture screenshot");
        }
    }, 30);
}

// Auto-register (commented out - for MV testing)
static TestFramework::TestRegistrar _merchantJumpTest("MerchantJump", MerchantJumpSaveTest, "SavedGame",
    TestFramework::ScenarioParams().setString("saveName", "mv-merchant"));
