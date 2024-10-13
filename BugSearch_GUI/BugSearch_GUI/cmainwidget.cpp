#include "cmainwidget.h"

CMainWidget::CMainWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000, 500);
    // Landsape file components
    p_ButtonLandscape = new QPushButton("Select Landscape file", this);
    p_ButtonLandscape->setGeometry(20, 50, 300, 30);
    // should show selected Landscape file path
    p_textLandscapePath = new QTextBrowser(this);
    p_textLandscapePath->setGeometry(350, 50, 300, 30);
    connect(p_ButtonLandscape, SIGNAL(released()), this, SLOT(onLandscapeButtonClicked()));
    // Bug file components
    p_ButtonBug = new QPushButton("Select Bug file", this);
    p_ButtonBug->setGeometry(20, 100, 300, 30);
    // should show selected Bug file path
    p_textBugPath = new QTextBrowser(this);
    p_textBugPath->setGeometry(350, 100, 300, 30);
    connect(p_ButtonBug, SIGNAL(released()), this, SLOT(onBugButtonClicked()));
    // Load the files into the app and start the searching for Bug pattern
    p_ButtonProcess = new QPushButton("Connect to BugSearch app", this);
    p_ButtonProcess->setGeometry(20, 150, 300, 30);
    p_textConnectionStatus = new QTextBrowser(this);
    p_textConnectionStatus->setGeometry(350, 150, 300, 30);
    // Connect the Buttons "released" signal to CMainWidgets onButtonReleased method.
    connect(p_ButtonProcess, SIGNAL(released()), this, SLOT(onProcessButtonReleased()));
    // Create a progress bar
    // with the range between 0 and 100, and a starting value of 0
    p_ProgressBar  = new QProgressBar(this);
    p_ProgressBar->setRange(0, 100);
    p_ProgressBar->setValue(0);
    p_ProgressBar->setGeometry(20, 200, 300, 30);
}

// Handler for button click
void CMainWidget::onProcessButtonReleased()
{
    CSocketClient SocketClient;

     p_textConnectionStatus->clear();
    if (true == SocketClient.initSocket()) {
        p_textConnectionStatus->append("Client Socket successfully connected.");
    }
    else {
        p_textConnectionStatus->append("Client Socket failed to connect!");
    }
}

void CMainWidget::onLandscapeButtonClicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Text (*.nfo *.txt)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    } else {
        return;
    }

    QStringList::iterator it = fileNames.begin();
    if (it != fileNames.end()) {
        // if list not empty (file is selected)
        p_textLandscapePath->append(*it);
    }
}
void CMainWidget::onBugButtonClicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Text (*.nfo *.txt)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    } else {
        return;
    }

    QStringList::iterator it = fileNames.begin();
    if (it != fileNames.end()) {
        // if list not empty (file is selected)
        p_textBugPath->append(*it);
    }
}
