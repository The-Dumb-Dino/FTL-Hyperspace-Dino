#include "Saves.h"
#include "State.h"
#include "../../../Global.h"
#include "../../../src/overrides/FileHelper.h"

namespace GameAccess
{


    bool Saves::loadTestSave(const std::string& saveName)
    {   
        // copy save name from 
        FileHelperExtension::setUserFolder("saves/"+saveName+"/");
        return true;
    }

    std::string Saves::getTestSavePath()
    {
        return "saves/";
    }

}
