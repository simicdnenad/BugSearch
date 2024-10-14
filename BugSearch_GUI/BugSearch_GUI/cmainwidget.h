#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QProgressBar>
#include <QTextBrowser>
#include <QFileDialog>
#include "SocketClassClient.h"
#include "connstates.h"

class CMainWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* p_ButtonProcess, *p_ButtonLandscape, *p_ButtonBug;
    QProgressBar* p_ProgressBar;
    QTextBrowser* p_textLandscapePath, *p_textBugPath, *p_textConnectionStatus;
    QFileDialog* p_fileDialogLandscape, *p_fileDialogBug;
    CSocketClient m_socketClient;
    EConnState e_connState = EConnState::NOT_CONNECTED;
public:
    explicit CMainWidget(QWidget *parent = nullptr);
private:
    bool initCommunication();
    bool forwardFileNames();
signals:

public slots:
    void onProcessButtonReleased(); // Handler for button pressed
    void onLandscapeButtonClicked();
    void onBugButtonClicked();
};

#endif // CMAINWIDGET_H
