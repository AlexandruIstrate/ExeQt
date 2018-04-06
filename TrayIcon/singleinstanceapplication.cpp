#include "singleinstanceapplication.h"

#include <QSharedMemory>
#include <QDebug>

SingleInstanceApplication::SingleInstanceApplication(int argc, char* argv[])
    : m_App { argc, argv }, m_Semaphore { "someid", 1 }, m_OnStart { nullptr }, m_OnRunning { nullptr }
{
    m_App.setQuitOnLastWindowClosed(false);
}

void SingleInstanceApplication::setOnStart(void (*func) (void))
{
    m_OnStart = func;
}

void SingleInstanceApplication::setOnInstanceRunning(void (*func) (void))
{
    m_OnRunning = func;
}

int SingleInstanceApplication::exec(QWidget* wdg)
{
    m_Semaphore.acquire();

#ifndef Q_OS_WIN32
    QSharedMemory unixSharedMemory("anotherid");
    if (unixSharedMemory.attach())
        unixSharedMemory.detach();
#endif

    QSharedMemory sharedMemory("anotherid");
    bool isRunning;

    if (sharedMemory.attach())
    {
        isRunning = true;
    }
    else
    {
        sharedMemory.create(1);
        isRunning = false;
    }

    m_Semaphore.release();

    if (isRunning)
    {
        if (m_OnRunning)
            m_OnRunning();

        qDebug();
        return 1;
    }

    if (m_OnStart)
        m_OnStart();

    return m_App.exec();
}
