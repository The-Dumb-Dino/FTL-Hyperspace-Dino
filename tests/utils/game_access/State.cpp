#include "State.h"
#include "../../../Global.h"

namespace GameAccess
{
    // ============================================
    // State Queries
    // ============================================

    bool State::isInGame()
    {
        return G_ && G_->GetWorld() && G_->GetWorld()->commandGui;
    }

    bool State::isInMenu()
    {
        return G_ && G_->GetCApp() && G_->GetCApp()->menu.bOpen;
    }

    // ============================================
    // Game Object Accessors
    // ============================================

    CommandGui* State::getCommandGui()
    {
        if (!isInGame()) return nullptr;
        return G_->GetWorld()->commandGui;
    }

    ShipManager* State::getPlayerShip()
    {
        CommandGui* gui = getCommandGui();
        if (!gui || !gui->shipComplete) return nullptr;
        return gui->shipComplete->shipManager;
    }

    StarMap* State::getStarMap()
    {
        if (!G_ || !G_->GetWorld()) return nullptr;
        return &G_->GetWorld()->starMap;
    }

    WorldManager* State::getWorld()
    {
        if (!G_) return nullptr;
        return G_->GetWorld();
    }

    MainMenu* State::getMainMenu()
    {
        if (G_ && G_->GetCApp())
        {
            return &G_->GetCApp()->menu;
        }
        return nullptr;
    }

    ShipBuilder* State::getShipBuilder()
    {
        MainMenu* menu = getMainMenu();
        if (menu)
        {
            return &menu->shipBuilder;
        }
        return nullptr;
    }

    // ============================================
    // Player Resource Modifiers
    // ============================================

    void State::setPlayerHull(int hull)
    {
        ShipManager* ship = getPlayerShip();
        if (ship)
        {
            ship->ship.hullIntegrity.first = hull;
        }
    }

    void State::setPlayerScrap(int scrap)
    {
        ShipManager* ship = getPlayerShip();
        if (ship)
        {
            ship->currentScrap = scrap;
        }
    }

    void State::setPlayerFuel(int fuel)
    {
        ShipManager* ship = getPlayerShip();
        if (ship)
        {
            ship->fuel_count = fuel;
        }
    }

    void State::setPlayerMissiles(int missiles)
    {
        ShipManager* ship = getPlayerShip();
        if (ship)
        {
            ship->tempMissileCount = missiles;
        }
    }

    void State::setPlayerDrones(int drones)
    {
        ShipManager* ship = getPlayerShip();
        if (ship)
        {
            ship->tempDroneCount = drones;
        }
    }

    // ============================================
    // Player Resource Getters
    // ============================================

    int State::getPlayerHull()
    {
        ShipManager* ship = getPlayerShip();
        return ship ? ship->ship.hullIntegrity.first : -1;
    }

    int State::getPlayerScrap()
    {
        ShipManager* ship = getPlayerShip();
        return ship ? ship->currentScrap : -1;
    }

    // ============================================
    // Test Isolation
    // ============================================

    // Always return to main menu between tests

    void State::returnToMainMenu()
    {
        if (G_ && G_->GetWorld())
        {
            G_->GetCApp()->menu.Open();
        }
    }

    // ============================================
    // UI Interaction
    // ============================================

    void State::clickButton(GenericButton* button)
    {
        if (!button || !G_ || !G_->GetCApp()) return;

        // Get button hitbox center
        Globals::Rect hitbox = button->hitbox;
        int clickX = hitbox.x + hitbox.w / 2;
        int clickY = hitbox.y + hitbox.h / 2;

        // MouseMove to set hover state, then click
        MainMenu* menu = &G_->GetCApp()->menu;
        menu->MouseMove(clickX, clickY);
        menu->MouseClick(clickX, clickY);
    }
}
