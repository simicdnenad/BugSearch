#include <QtWidgets>
#include "mainwidget.h"

// Constructor for main widget
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    up_Button.reset(new QPushButton(tr("Fix Me!")));
    up_textBrowser.reset(new QTextBrowser());

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(up_Button.get(),0,0);
    mainLayout->addWidget(up_textBrowser.get(),1,0);
    setLayout(mainLayout);
    setWindowTitle(tr("Connecting buttons to processes.."));
}

// Destructor
MainWidget::~MainWidget()
{   
}


