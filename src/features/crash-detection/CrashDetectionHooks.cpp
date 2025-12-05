#include "Global.h"
#include "integration/CrashReportFlow.h"

// Initialize crash detection and show dialog on menu open
// After main menu opens, mods usually initialize their custom savePrefix
HOOK_METHOD(MainMenu, Open, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> MainMenu::Open -> Begin (CrashDetectionHooks.cpp)\n")
    super();
    CrashReportFlow::GetInstance()->OnMenuOpen();
}

// Render crash detection dialogs
HOOK_METHOD(MainMenu, OnRender, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> MainMenu::OnRender -> Begin (CrashDetectionHooks.cpp)\n")
    super();
    CrashReportFlow::GetInstance()->OnRender();
}

// Handle crash detection dialog input
HOOK_METHOD(MainMenu, MouseClick, (int x, int y) -> void)
{
    LOG_HOOK("HOOK_METHOD -> MainMenu::MouseClick -> Begin (CrashDetectionHooks.cpp)\n")

    bool shouldPropagate = true;
    CrashReportFlow::GetInstance()->OnMouseClick(x, y, shouldPropagate);

    if (shouldPropagate)
    {
        super(x, y);
    }
}

// Handle crash detection dialog mouse move
HOOK_METHOD(MainMenu, MouseMove, (int x, int y) -> void)
{
    LOG_HOOK("HOOK_METHOD -> MainMenu::MouseMove -> Begin (CrashDetectionHooks.cpp)\n")

    bool shouldPropagate = true;
    CrashReportFlow::GetInstance()->OnMouseMove(x, y, shouldPropagate);

    if (shouldPropagate)
    {
        super(x, y);
    }
}

// Clear crash flag on normal game exit
HOOK_METHOD(CApp, OnExit, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> CApp::OnExit -> Begin (CrashDetectionHooks.cpp)\n")
    CrashReportFlow::GetInstance()->OnGameExit();
    super();
}
