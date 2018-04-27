#include "mainwidget.h"

#include <QApplication>
#include <QMessageBox>

#include <qtsingleapplication.h>

int main(int argc, char* argv[])
{
    try
    {
        QtSingleApplication instance(argc, argv);
        instance.setQuitOnLastWindowClosed(false);

        if (instance.sendMessage("Are you running, sweetheart?"))
            return 0;

        MainWidget w;
        instance.setActivationWindow(&w);

        return instance.exec();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(nullptr, QObject::tr("Fatal Error"), QObject::tr("The application encountered an error from which it could not recover!\n") + QString(e.what()));
    }

    return -1;
}
