#include "../utils/framework/ScenarioRegistry.h"
#include "../utils/game_access/State.h"
#include "../utils/game_access/Seeding.h"
#include "../../Global.h"

/**
 * New Game Seeded Scenario
 *
 * Starts a new game with deterministic seed for reproducible tests.
 *
 * Parameters:
 *   - seed (int): RNG seed value (default: 12345)
 */

static void setupNewGameSeeded(TestFramework::Test* test, TestFramework::TestStages* stages, const TestFramework::ScenarioParams& params)
{
    test->section("Scenario: New Game Seeded");

    const int randomSeed =  random() % 100000;

    // Get seed parameter (default randomSeed)
    const int seed = params.getInt("seed", randomSeed);


    // Stage 1: Open ship builder
    stages->addStage("Open ship builder", [test]() {
        ShipBuilder* shipBuilder = GameAccess::State::getShipBuilder();
        test->requireNotNull(shipBuilder, "ShipBuilder is accessible");
        shipBuilder->Open();
        test->log("ShipBuilder opened");
    }, 10); // Wait 0.5 seconds

    // Stage 2: Set seed and finish
    stages->addStage("Set seed and start game", [test, seed]() {
        ShipBuilder* shipBuilder = GameAccess::State::getShipBuilder();
        test->requireNotNull(shipBuilder, "ShipBuilder still accessible");

        // Set seed between Open() and Finish()
        GameAccess::Seeding::setSeed(seed);
        test->log("Seed " + std::to_string(seed) + " set");

        // Finish to start the game
        shipBuilder->Finish();
        test->log("ShipBuilder finished - game starting");
    }, 30); // Wait 0.5 seconds

    // Stage 3: Verify game started
    stages->addStage("Verify game started", [test]() {
        test->requireTrue(GameAccess::State::isInGame(), "Game started successfully");
        test->log("Game is running");
    }, 0);

    // Stage 4: Verify player ship exists
    stages->addStage("Verify player ship", [test]() {
        ShipManager* ship = GameAccess::State::getPlayerShip();
        test->requireNotNull(ship, "Player ship exists");
        test->log("Player ship initialized");
    }, 0);

    test->log("New game seeded scenario ready");
}

// Auto-register this scenario with default seed
static TestFramework::ScenarioRegistrar _newGameSeededScenario(
    "NewGameSeeded",
    setupNewGameSeeded,
    TestFramework::ScenarioParams()
);
