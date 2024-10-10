#include <QApplication>
#include <QTextBrowser>

#include "cmainwidget.h"

#include <linux/limits.h>
#include <memory>

/**
 * @brief Retreives path of the logo icon
 *
 * Takes absolute path of apps binary file, parse and modify it to the path of logo icon.
 *
 * @return shared_ptr<char[]> keeps the path to the icon's .png file
 */
std::shared_ptr<char[]> icoPath(const char* strBinPath) {
    std::shared_ptr<char[]> pCwd (new char[PATH_MAX]);

    std::string strPath(strBinPath);
    // go back one directory and add icon's path
    std::size_t found = strPath.find_last_of("/");
    strPath = strPath.substr(0,found);
    found = strPath.find_last_of("/");
    strPath = strPath.substr(0,found);
    strPath += "/BugSearch_GUI/download.png";
    strcpy(pCwd.get(), strPath.c_str());

    return pCwd;
}


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(icoPath(argv[0]).get()));

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
