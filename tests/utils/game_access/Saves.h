#pragma once
#include <string>

/**
 * GameAccess::Saves
 *
 * Save file management for tests.
 * Provides utilities to create, load, and manage test save files
 * for repeatable testing scenarios.
 */

namespace GameAccess
{
    /**
     * Manages test save files for repeatable testing scenarios
     */
    class Saves
    {
    public:
        // Create a test save from the current game state
        static void createTestSave(const std::string& saveName);

        // Load a test save
        static bool loadTestSave(const std::string& saveName);

        // Get the full path to a test save file
        static std::string getTestSavePath(const std::string& saveName);

        // Check if a test save exists
        static bool testSaveExists(const std::string& saveName);
    };
}
