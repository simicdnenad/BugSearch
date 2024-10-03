#include "cmainwidget.h"

CMainWidget::CMainWidget(QWidget *parent) : QWidget(parent),p_ButtonConnect(nullptr)
{
    this->setFixedSize(1000, 500);
    p_ButtonConnect = new QPushButton("Connect to BugSearch app", this);
    p_ButtonConnect->setGeometry(10, 10, 300, 30);
    // Should show the connection status
    p_textBrowser = new QTextBrowser(this);
    p_textBrowser->setGeometry(10, 100, 300, 30);
    // Create a progress bar
    // with the range between 0 and 100, and a starting value of 0
    p_ProgressBar  = new QProgressBar(this);
    p_ProgressBar->setRange(0, 100);
    p_ProgressBar->setValue(0);
    p_ProgressBar->setGeometry(10, 50, 300, 30);
    // Connect the Buttons "released" signal to CMainWidgets onButtonReleased method.
    connect(p_ButtonConnect, SIGNAL(released()), this, SLOT(onButtonReleased()));
}

// Handler for button click
void CMainWidget::onButtonReleased()
{
    CSocketClient SocketClient;

     p_textBrowser->clear();
    if (true == SocketClient.initSocket()) {
        p_textBrowser->append("Client Socket successfully connected.");
    }
    else {
        p_textBrowser->append("Client Socket failed to connect!");
    }
}
