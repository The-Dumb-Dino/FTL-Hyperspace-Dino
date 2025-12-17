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
        // Load a test save
        static bool loadTestSave(const std::string& saveName);

        // Get the full path to save files
        static std::string getTestSavePath();
    };
}
