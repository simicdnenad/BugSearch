#include <QApplication>
#include <QTextBrowser>

#include "cmainwidget.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    CMainWidget window;
#ifdef NOTDEF
    QTextBrowser textBrowser(&window);
    textBrowser.append("text Browser!");
    // textBrowser.show();

    QPushButton button("Search for Bug",&window);
    button.setGeometry(10,10,200,30);
    //QIcon icon ("download.png");
    //button.setIcon(icon);
    // button.show();
#endif

    window.show();
    return app.exec();
}
