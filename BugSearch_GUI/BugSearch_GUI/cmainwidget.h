#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QProgressBar>
#include <QTextBrowser>
#include "SocketClassClient.h"

class CMainWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* p_ButtonConnect;
    QProgressBar* p_ProgressBar;
    QTextBrowser* p_textBrowser;
public:
    explicit CMainWidget(QWidget *parent = nullptr);

signals:

public slots:
    void onButtonReleased(); // Handler for button pressed
};

#endif // CMAINWIDGET_H
