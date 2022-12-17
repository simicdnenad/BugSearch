#include <QtWidgets>
#include "mainwidget.h"

// Constructor for main widget
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    up_Button.reset(new QPushButton(tr("List Test Files.")));
    up_textBrowser.reset(new QTextBrowser());
    up_Process.reset(new QProcess());

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(up_Button.get(),0,0);
    mainLayout->addWidget(up_textBrowser.get(),1,0);
    setLayout(mainLayout);
    setWindowTitle(tr("Listing test directory content..."));

    // Connect the Buttons "released" signal to MainWidget's onButtonReleased method.
    connect(up_Button.get(), SIGNAL(released()), this, SLOT(onButtonReleased()));
    connect(up_Process.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(onCaptureProcessOutput()));
}

// Handler for button click
void MainWidget::onButtonReleased()
{
    // clear the text in the textBrowser
    up_textBrowser->clear();
    up_textBrowser->append(tr("Running command:"));

    // Set up our process to write to stdout and run our command
    up_Process->setCurrentWriteChannel(QProcess::StandardOutput); // Set the write channel
    up_Process->start("find ../manual_test/ -name \"*.nfo\"  "); // Start the program
}

void MainWidget::onCaptureProcessOutput()
{
    // Determine whether the object that sent the signal was a pointer to a process
    QProcess* process = qobject_cast<QProcess*>(sender());
    // If so, append the output to the textbrowser
    if (process)
        up_textBrowser->append(process->readAllStandardOutput());
}

// Destructor
MainWidget::~MainWidget()
{   
}


