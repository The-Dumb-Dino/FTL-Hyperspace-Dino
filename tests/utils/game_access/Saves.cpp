#include "Saves.h"
#include "State.h"
#include "../../../Global.h"

namespace GameAccess
{
    void Saves::createTestSave(const std::string& saveName)
    {
        WorldManager* world = State::getWorld();
        if (world)
        {
            world->SaveGame();
            // TODO: Copy continue.sav to tests/saves/saveName.sav
        }
    }

    bool Saves::loadTestSave(const std::string& saveName)
    {
        WorldManager* world = State::getWorld();
        if (world)
        {
            std::string path = getTestSavePath(saveName);
            // TODO: Copy test save to continue.sav then load
            // world->LoadGame(path);
            return true;
        }
        return false;
    }

    std::string Saves::getTestSavePath(const std::string& saveName)
    {
        return "tests/saves/" + saveName + ".sav";
    }

    bool Saves::testSaveExists(const std::string& saveName)
    {
        // TODO: Check if file exists
        return false;
    }
}
