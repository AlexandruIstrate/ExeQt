/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef SINGLEINSTANCEAPPLICATION_H
#define SINGLEINSTANCEAPPLICATION_H

#include <QApplication>
#include <QSystemSemaphore>
#include <QWidget>

class SingleInstanceApplication
{
private:
	QApplication m_App;
	QSystemSemaphore m_Semaphore;

	void (*m_OnStart) (void);
	void (*m_OnRunning) (void);

public:
	SingleInstanceApplication(int argc, char* argv[]);

	void setOnStart(void (*func) (void));
	void setOnInstanceRunning(void (*func) (void));

	int exec(QWidget*);
};

#endif // SINGLEINSTANCEAPPLICATION_H
