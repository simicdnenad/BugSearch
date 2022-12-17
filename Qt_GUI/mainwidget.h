#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <memory>
#include <QProcess>
using namespace std;

class QPushButton;
class QTextBrowser;

// This is the declaration of our MainWidget class
// The definition/implementation is in mainwidget.cpp
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0); //Constructor
    virtual ~MainWidget(); // Destructor

private slots:
    void onButtonReleased(); // Handler for button pressed
    void onCaptureProcessOutput(); // Handler for Process output

private:
    unique_ptr<QPushButton> up_Button;
    unique_ptr<QTextBrowser> up_textBrowser;
    unique_ptr<QProcess> up_Process;
};

#endif // MAINWIDGET_H


