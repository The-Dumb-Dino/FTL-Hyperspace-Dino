#pragma once

#include <string>

/**
 * CrashDialogManager - Manages all crash reporting dialogs
 *
 * Dialog Flow:
 * 1. AskReportDialog - Ask if user wants to report crash
 * 2. ChooseDestinationDialog - Ask where to report (Discord or GitHub)
 * 3. ShowInstructionsDialog - Show instructions + Open folder option
 */
class CrashDialogManager
{
public:
    static CrashDialogManager* GetInstance();
    ~CrashDialogManager();

    // Dialog 1: "Do you want to report this crash?"
    void ShowAskReportDialog();

    // Dialog 2: "Report to Discord or GitHub?"
    void ShowChooseDestinationDialog();

    // Dialog 3: "Here's how to report + Open folder option"
    void ShowInstructionsDialog(const std::string& bugReportPath, bool discordSelected);

    // Error dialog: "Failed to create bug report"
    void ShowErrorDialog();

    // Rendering and input handling
    void OnRender();
    void OnMouseClick(int x, int y, bool& shouldPropagate);
    void OnMouseMove(int x, int y, bool& shouldPropagate);

    // Dialog state queries
    bool IsAskReportDialogOpen() const;
    bool IsChooseDestinationDialogOpen() const;
    bool IsInstructionsDialogOpen() const;
    bool IsErrorDialogOpen() const;
    bool AnyCrashDialogOpen() const;

    // Results from dialogs
    bool GetAskReportResult() const;         // true = "Create Report", false = "Skip"
    bool GetChooseDestinationResult() const; // true = Discord, false = GitHub
    bool GetInstructionsResult() const;      // true = "Open Folder", false = "Done"

private:
    CrashDialogManager();
    static CrashDialogManager* instance;

    class ConfirmWindow* askReportDialog = nullptr;
    class ConfirmWindow* chooseDestinationDialog = nullptr;
    class ConfirmWindow* instructionsDialog = nullptr;
    class ConfirmWindow* errorDialog = nullptr;
};
