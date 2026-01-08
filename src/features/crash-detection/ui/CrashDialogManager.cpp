#include "CrashDialogManager.h"
#include "Global.h"

CrashDialogManager* CrashDialogManager::instance = nullptr;

CrashDialogManager::CrashDialogManager()
{
}

CrashDialogManager::~CrashDialogManager()
{
    delete askReportDialog;
    delete chooseDestinationDialog;
    delete instructionsDialog;
    delete errorDialog;
}

CrashDialogManager* CrashDialogManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new CrashDialogManager();
    }
    return instance;
}

void CrashDialogManager::InitButton()
{
    bugReportLabel.data = "BUG";
    bugReportLabel.isLiteral = true;
    bugReportButton = new TextButton();
    bugReportButton->OnInit(Point(1208, 632), Point(50, 24), 4, &bugReportLabel, 62);
    bugReportButton->bActive = true;   
}

void CrashDialogManager::UpdateButtonHover(int x, int y)
{
    if (bugReportButton)
    {
        bugReportButton->MouseMove(x, y, false);
        if (bugReportButton->bActive && bugReportButton->bHover)
        {
            G_->GetMouseControl()->SetTooltip("Click to report a bug");
        }
    }
}

bool CrashDialogManager::IsBugButtonClicked() const
{
    return bugReportButton && bugReportButton->bActive && bugReportButton->bHover;
}

void CrashDialogManager::ShowAskReportDialog(bool isManualReport)
{
    if (askReportDialog == nullptr)
    {
        askReportDialog = new ConfirmWindow();
    }

    TextString text;
    text.isLiteral = true;
    if (isManualReport)
    {
        text.data = "Do you want to create a bug report with your saves and logs?";
    }
    else
    {
        text.data = "A Hyperspace mod crash was detected on the previous game session.\n\nDo you want to create a bug report with your saves and logs?";
    }

    TextString yes;
    yes.isLiteral = true;
    yes.data = "Create Bug Report";

    TextString no;
    no.isLiteral = true;
    no.data = "Skip";

    askReportDialog->SetText(text, 400, true, yes, no);
    askReportDialog->Open();
}

void CrashDialogManager::ShowChooseDestinationDialog()
{
    if (chooseDestinationDialog == nullptr)
    {
        chooseDestinationDialog = new ConfirmWindow();
    }

    TextString text;
    text.isLiteral = true;
    text.data = "Bug report created successfully!\n\nWhere would you like to report this?";

    TextString yes;
    yes.isLiteral = true;
    yes.data = "Report on Discord";

    TextString no;
    no.isLiteral = true;
    no.data = "Create GitHub Issue";

    chooseDestinationDialog->SetText(text, 400, true, yes, no);
    chooseDestinationDialog->Open();
}

void CrashDialogManager::ShowInstructionsDialog(const std::string& bugReportPath, bool discordSelected)
{
    if (instructionsDialog == nullptr)
    {
        instructionsDialog = new ConfirmWindow();
    }

    TextString text;
    text.isLiteral = true;

    if (discordSelected)
    {
        text.data = "Please report the bug on the FTL:Multiverse Discord server.\n\nBug report location:\n" + bugReportPath;
    }
    else
    {
        text.data = "Please create a GitHub issue at:\ngithub.com/FTL-Hyperspace/FTL-Hyperspace/issues\n\nBug report location:\n" + bugReportPath;
    }

    TextString yes;
    yes.isLiteral = true;
    yes.data = "Open Bug Report Folder";

    TextString no;
    no.isLiteral = true;
    no.data = "Done";

    instructionsDialog->SetText(text, 450, true, yes, no);
    instructionsDialog->Open();
}

void CrashDialogManager::ShowErrorDialog()
{
    if (errorDialog == nullptr)
    {
        errorDialog = new ConfirmWindow();
    }

    TextString text;
    text.isLiteral = true;
    text.data = "Failed to create bug report.\n\nPlease check the logs for more details.";

    TextString yes;
    yes.isLiteral = true;
    yes.data = "OK";

    TextString no;
    no.isLiteral = true;
    no.data = "";

    errorDialog->SetText(text, 400, true, yes, no);
    errorDialog->Open();
}

void CrashDialogManager::OnRender()
{
    // Render bug report button
    if (bugReportButton)
    {
        bugReportButton->OnRender();
    }

    // Render dialogs
    if (askReportDialog != nullptr && askReportDialog->bOpen)
    {
        askReportDialog->OnRender();
    }

    if (chooseDestinationDialog != nullptr && chooseDestinationDialog->bOpen)
    {
        chooseDestinationDialog->OnRender();
    }

    if (instructionsDialog != nullptr && instructionsDialog->bOpen)
    {
        instructionsDialog->OnRender();
    }

    if (errorDialog != nullptr && errorDialog->bOpen)
    {
        errorDialog->OnRender();
    }
}

void CrashDialogManager::OnMouseClick(int x, int y, bool& shouldPropagate)
{
    if (askReportDialog != nullptr && askReportDialog->bOpen)
    {
        askReportDialog->MouseClick(x, y);
        shouldPropagate = false;
        return;
    }

    if (chooseDestinationDialog != nullptr && chooseDestinationDialog->bOpen)
    {
        chooseDestinationDialog->MouseClick(x, y);
        shouldPropagate = false;
        return;
    }

    if (instructionsDialog != nullptr && instructionsDialog->bOpen)
    {
        instructionsDialog->MouseClick(x, y);
        shouldPropagate = false;
        return;
    }

    if (errorDialog != nullptr && errorDialog->bOpen)
    {
        errorDialog->MouseClick(x, y);
        shouldPropagate = false;
        return;
    }

    shouldPropagate = true;
}

void CrashDialogManager::OnMouseMove(int x, int y, bool& shouldPropagate)
{
    if (askReportDialog != nullptr && askReportDialog->bOpen)
    {
        askReportDialog->MouseMove(x, y);
        shouldPropagate = false;
        return;
    }

    if (chooseDestinationDialog != nullptr && chooseDestinationDialog->bOpen)
    {
        chooseDestinationDialog->MouseMove(x, y);
        shouldPropagate = false;
        return;
    }

    if (instructionsDialog != nullptr && instructionsDialog->bOpen)
    {
        instructionsDialog->MouseMove(x, y);
        shouldPropagate = false;
        return;
    }

    if (errorDialog != nullptr && errorDialog->bOpen)
    {
        errorDialog->MouseMove(x, y);
        shouldPropagate = false;
        return;
    }

    shouldPropagate = true;
}

bool CrashDialogManager::IsAskReportDialogOpen() const
{
    return askReportDialog != nullptr && askReportDialog->bOpen;
}

bool CrashDialogManager::IsChooseDestinationDialogOpen() const
{
    return chooseDestinationDialog != nullptr && chooseDestinationDialog->bOpen;
}

bool CrashDialogManager::IsInstructionsDialogOpen() const
{
    return instructionsDialog != nullptr && instructionsDialog->bOpen;
}

bool CrashDialogManager::IsErrorDialogOpen() const
{
    return errorDialog != nullptr && errorDialog->bOpen;
}

bool CrashDialogManager::AnyCrashDialogOpen() const
{
    return IsAskReportDialogOpen() || IsChooseDestinationDialogOpen() || IsInstructionsDialogOpen() || IsErrorDialogOpen();
}

bool CrashDialogManager::GetAskReportResult() const
{
    return askReportDialog != nullptr ? askReportDialog->result : false;
}

bool CrashDialogManager::GetChooseDestinationResult() const
{
    return chooseDestinationDialog != nullptr ? chooseDestinationDialog->result : false;
}

bool CrashDialogManager::GetInstructionsResult() const
{
    return instructionsDialog != nullptr ? instructionsDialog->result : false;
}
