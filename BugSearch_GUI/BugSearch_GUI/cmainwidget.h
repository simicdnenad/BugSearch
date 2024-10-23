#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QProgressBar>
#include <QTextBrowser>
#include <QFileDialog>
#include <QCloseEvent>
#include "SocketClassClient.h"
#include "connstates.h"

class CMainWidget : public QMainWindow
{
    Q_OBJECT
private:
    QPushButton* p_ButtonProcess, *p_ButtonLandscape, *p_ButtonBug;
    QProgressBar* p_ProgressBar;
    QTextBrowser* p_textLandscapePath, *p_textBugPath, *p_textConnectionStatus;
    QFileDialog* p_fileDialogLandscape, *p_fileDialogBug;
    CSocketClient m_socketClient;
    EConnState e_connState = EConnState::FILES_NOT_SELECTED;
protected:
    void closeEvent(QCloseEvent *event) override;
public:
    explicit CMainWidget(QWidget *parent = nullptr);
private:
    bool initCommunication();
    bool forwardFileNames();
    void createActions();
signals:

public slots:
    void onProcessButtonReleased(); // Handler for button pressed
    void onLandscapeButtonClicked();
    void onBugButtonClicked();
    void onAboutMenuClicked();
};

#endif // CMAINWIDGET_H
