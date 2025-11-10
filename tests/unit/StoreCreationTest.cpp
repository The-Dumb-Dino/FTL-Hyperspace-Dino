

#include "../utils/TestUtils.h"
#include "../../Global.h"

/**
 * Unit Test: Store Creation
 *
 * Tests that stores can be created without crashing
 * This was the original macOS Sequoia bug with memory protection
 */

static void RunStoreCreationTest(TestFramework::Test& test, TestFramework::TestStages& stages)
{
    test.section("Store Creation Test");

    // Close initial dialogue by pressing button 1
    stages.addStage("Close initial dialogue", [&test]() {
        CommandGui* gui = GameAccess::State::getCommandGui();
        test.requireNotNull(gui, "CommandGui is accessible");

        if (gui->choiceBox.bOpen && !gui->choiceBox.choiceBoxes.empty())
        {
            // Try pressing key "1" to select first choice
            gui->choiceBox.KeyDown(SDLKey::SDLK_1);
            test.pass("Pressed key 1 for dialogue choice");
        }
        else
        {
            test.pass("No dialogue to close");
        }
    }, 10); // Wait 1 second for dialogue to close

    // Verify dialogue closed
    stages.addStage("Verify dialogue closed", [&test]() {
        CommandGui* gui = GameAccess::State::getCommandGui();
        test.requireNotNull(gui, "CommandGui is accessible");
        test.assertFalse(gui->choiceBox.bOpen, "Dialogue is closed");
    }, 10); // Wait 0.5 seconds

    // Create store
    stages.addStage("Create store", [&test]() {
        CommandGui* gui = GameAccess::State::getCommandGui();
        test.requireNotNull(gui, "CommandGui is accessible");
        gui->CreateNewStore(1);
    }, 10); // Wait 0.5 seconds

    // Open store
    stages.addStage("Open store", [&test]() {
        CommandGui* gui = GameAccess::State::getCommandGui();
        gui->storeButton.OnClick();
    }, 10); // Wait 0.5 seconds for store to render

    // Take screenshot
    stages.addStage("Take screenshot of store", [&test]() {
        if (!TestHelpers::takeScreenshot("store_test.bmp"))
        {
            test.fail("Screenshot", "Failed to capture screenshot");
        }
    }, 0);
}

// Auto-register this test with NewGameSeeded scenario
static TestFramework::TestRegistrar _storeCreationTest("StoreCreation", RunStoreCreationTest, "NewGameSeeded");


