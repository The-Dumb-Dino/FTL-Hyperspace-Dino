#ifdef __APPLE__

#include "Global.h"
#include <objc/runtime.h>
#include <objc/message.h>

// AppKit.h and CoreGraphics headers cannot be included here because Foundation
// pulls in MacTypes.h which conflicts with FTL's Point type. Both NSCursor and
// the CoreGraphics cursor APIs are loaded in the process via Cocoa, so we can
// forward-declare what we need and call them directly.

extern "C" {
    typedef uint32_t CGDirectDisplayID;
    CGDirectDisplayID CGMainDisplayID(void);
    int CGDisplayHideCursor(CGDirectDisplayID display);
    int CGDisplayShowCursor(CGDirectDisplayID display);
}

// Swizzle NSCursor +hide and +unhide to no-ops so SDL cannot interfere with
// the cursor hide counter. We call the original IMPs directly.
static IMP g_originalHideIMP   = nullptr;
static IMP g_originalUnhideIMP = nullptr;

static void nscursor_hide_noop(id, SEL) {}
static void nscursor_unhide_noop(id, SEL) {}

static void InstallCursorSwizzle()
{
    static bool installed = false;
    if (installed) return;
    installed = true;

    // Class methods are instance methods of the metaclass
    Class metaclass = objc_getMetaClass("NSCursor");
    if (!metaclass) return;

    Method hideMethod   = class_getInstanceMethod(metaclass, sel_registerName("hide"));
    Method unhideMethod = class_getInstanceMethod(metaclass, sel_registerName("unhide"));
    if (!hideMethod || !unhideMethod) return;

    g_originalHideIMP   = method_getImplementation(hideMethod);
    g_originalUnhideIMP = method_getImplementation(unhideMethod);

    method_setImplementation(hideMethod,   reinterpret_cast<IMP>(nscursor_hide_noop));
    method_setImplementation(unhideMethod, reinterpret_cast<IMP>(nscursor_unhide_noop));
}

// Hides the cursor at both the NSCursor and CoreGraphics levels.
// Called every frame while focused — no bool guard — so that if macOS's
// cursor tracking area system resets either counter on window exit, it
// recovers within one frame (~16 ms at 60 fps).
static void HideMacOSCursor()
{
    InstallCursorSwizzle();
    if (g_originalHideIMP)
    {
        reinterpret_cast<void (*)(id, SEL)>(g_originalHideIMP)(
            (id)objc_getClass("NSCursor"), sel_registerName("hide"));
    }
    CGDisplayHideCursor(CGMainDisplayID());
}

// Flushes both hide counters on blur. Both NSCursor and CGDisplay clamp at 0,
// so excess unhide calls are harmless. 1000 is safely above any realistic
// accumulated count from one focus session.
static void ShowMacOSCursor()
{
    if (g_originalUnhideIMP)
    {
        id cls = (id)objc_getClass("NSCursor");
        SEL sel = sel_registerName("unhide");
        for (int i = 0; i < 1000; i++)
            reinterpret_cast<void (*)(id, SEL)>(g_originalUnhideIMP)(cls, sel);
    }
    for (int i = 0; i < 1000; i++)
        CGDisplayShowCursor(CGMainDisplayID());
}

HOOK_METHOD(CApp, OnInputFocus, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> CApp::OnInputFocus -> Begin (CursorHide.cpp)\n")
    super();
    HideMacOSCursor();
}

HOOK_METHOD(CApp, OnInputBlur, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> CApp::OnInputBlur -> Begin (CursorHide.cpp)\n")
    super();
    ShowMacOSCursor();
}

HOOK_METHOD(CApp, GenInputEvents, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> CApp::GenInputEvents -> Begin (CursorHide.cpp)\n")
    super();
    if (focus)
    {
        HideMacOSCursor();
    }
}

#endif // __APPLE__
