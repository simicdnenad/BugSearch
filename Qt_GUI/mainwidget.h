#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <memory>
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

private:
    unique_ptr<QPushButton> up_Button;
    unique_ptr<QTextBrowser> up_textBrowser;
};

#endif // MAINWIDGET_H


