#include "cmainwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextStream>

CMainWidget::CMainWidget(QWidget *parent) : QMainWindow(parent)
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
    // Connect the Buttons "released" signal to CMainWidgets onButtonReleased method.
    connect(p_ButtonProcess, SIGNAL(released()), this, SLOT(onProcessButtonReleased()));
    // Create a progress bar
    // with the range between 0 and 100, and a starting value of 0
    p_ProgressBar  = new QProgressBar(this);
    p_ProgressBar->setRange(0, 100);
    p_ProgressBar->setValue(0);
    p_ProgressBar->setGeometry(20, 200, 300, 30);
    // Enable status bar for showing program state
    statusBar()->showMessage(tr("Ready"));

    createActions();
}

void CMainWidget::createActions() {
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    const QIcon aboutIcon = QIcon::fromTheme("document-about", QIcon(":/images/about.png"));
    QAction *aboutBugApp = new QAction(aboutIcon, tr("&About"), this);
    aboutBugApp->setShortcuts(QKeySequence::HelpContents);
    aboutBugApp->setStatusTip(tr("Shows info about the program"));
    helpMenu->addAction(aboutBugApp);
    connect(aboutBugApp, SIGNAL(triggered()), this, SLOT(onAboutMenuClicked()));
}

// Handler for button click
void CMainWidget::onProcessButtonReleased() {
    switch(e_connState) {
        case EConnState::FILES_NOT_SELECTED:
        {
            if (!(p_textLandscapePath->toPlainText().isEmpty()) && !(p_textBugPath->toPlainText().isEmpty())) {
                e_connState = EConnState::NOT_CONNECTED;
            } else {
                statusBar()->showMessage("Landscape or Bug files not selected. Please select both of them!");
                break;
            }
        }
        case EConnState::NOT_CONNECTED:
        {
            bool bStatus = false;
            bStatus = initCommunication();
            if (true == bStatus) {
                e_connState = EConnState::CONNECTED;
                p_ProgressBar->setValue(10);
                statusBar()->showMessage("Successfully connected to the BugSearch app. Click button again to forward file paths and to start data processing.");
                p_ButtonProcess->setText("Start data processing");
            } else {
                statusBar()->showMessage("Problem in communication with BugSeach app!");
            }
            break;
        }
        case EConnState::CONNECTED:
        {
            if (true == forwardFileNames()) {
                e_connState = EConnState::PROCESSING;
            } else {
                statusBar()->showMessage("Problem in communication with BugSeach app!");
                e_connState = EConnState::UNKNOWN_FAILURE;
            }
            break;
        }
        default:
        {
            break;
        }
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
        p_textLandscapePath->clear();
        p_textLandscapePath->append(*it);
        showLandscapeFileContent(*it);
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
        p_textBugPath->clear();
        p_textBugPath->append(*it);
        showBugFileContent(*it);
    }
}

bool CMainWidget::initCommunication() {
    bool retVal = false;

    if (true == m_socketClient.initSocket()) {
        statusBar()->showMessage("Client Socket successfully connected.");
        retVal = true;
    }
    else {
        statusBar()->showMessage("Client Socket failed to connect!");
    }
    return retVal;
}

bool CMainWidget::forwardFileNames() {
    bool bRet = true;
    if (m_socketClient.setTxData(reinterpret_cast<const uint8_t*>(p_textLandscapePath->toPlainText().toStdString().c_str()),
                                 strlen(p_textLandscapePath->toPlainText().toStdString().c_str())) == true   &&
        m_socketClient.setTxData(reinterpret_cast<const uint8_t*>(p_textBugPath->toPlainText().toStdString().c_str()),
                                 strlen(p_textBugPath->toPlainText().toStdString().c_str())) == true) {
        if ((bRet = m_socketClient.SendMsg()) == true) {
            statusBar()->showMessage("File paths successfully sent to BugSearch app and data processing has started.");
        } else {
            statusBar()->showMessage("Failed to send file paths to BugSearch app.");
        }
    } else {
        statusBar()->showMessage("Landscape and Bug file paths are too long.");
        bRet = false;
    }

    return bRet;
}

void CMainWidget::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "BugSearch_GUI", "Quit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (nullptr != p_fBugFile ) {
            // close and free old resources
            p_editBugFile->close();
            delete p_editBugFile;
            p_fBugFile->close();
            delete p_fBugFile;
        }
        if (nullptr != p_fLandscapeFile ) {
            // close and free old resources
            p_editLandscapeFile->close();
            delete p_editLandscapeFile;
            p_fLandscapeFile->close();
            delete p_fLandscapeFile;
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void CMainWidget::onAboutMenuClicked() {
    QMessageBox::StandardButton about;
    QString strInfo = "Before starting the search, Landspace and Bug files should be selected. "
                      "Inside Landscape file is searched for a content of Bug file. \n";
    about = QMessageBox::information(this, "BugSearch_GUI",
                                  strInfo, QMessageBox::Ok);
}

void CMainWidget::showLandscapeFileContent(QString& strFileName){
    if (nullptr != p_fLandscapeFile ) {
        // close and free old resources
        p_editLandscapeFile->close();
        delete p_editLandscapeFile;
        p_fLandscapeFile->close();
        delete p_fLandscapeFile;
    }
    // alocate new one
    p_fLandscapeFile = new QFile (strFileName);
    p_editLandscapeFile = new QTextEdit();

    if (p_fLandscapeFile->open(QIODevice::ReadOnly)) {
        QTextStream stream(p_fLandscapeFile);
        p_editLandscapeFile->setPlainText( stream.readAll() );
        p_editLandscapeFile->setWindowFilePath(strFileName);
        p_editLandscapeFile->show();
    }
}

void CMainWidget::showBugFileContent(QString& strFileName){
    if (nullptr != p_fBugFile ) {
        // close and free old resources
        p_editBugFile->close();
        delete p_editBugFile;
        p_fBugFile->close();
        delete p_fBugFile;
    }
    // alocate new one
    p_fBugFile = new QFile (strFileName);
    p_editBugFile = new QTextEdit();

    if (p_fBugFile->open(QIODevice::ReadOnly)) {
        QTextStream stream(p_fBugFile);
        p_editBugFile->setPlainText( stream.readAll() );
        p_editBugFile->setWindowFilePath(strFileName);
        p_editBugFile->show();
    }
}
