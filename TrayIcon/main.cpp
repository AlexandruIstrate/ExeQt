#include "mainwidget.h"
#include "singleinstanceapplication.h"

#include <QApplication>
#include <QMessageBox>

void onInstanceRunning()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("The application is already running.\n"
                   "Only one instance of the application is allowed to run.");
    msgBox.exec();
}

int main(int argc, char *argv[])
{
    try
    {
        SingleInstanceApplication sng(argc, argv);
        sng.setOnInstanceRunning(onInstanceRunning);

        MainWidget w;
        return sng.exec(&w);
    }
    catch (...)
    {
        QMessageBox::critical(nullptr, "Fatal Error", "A fatal error was encountered and the app had to be stopped!");
        return -1;
    }
}
