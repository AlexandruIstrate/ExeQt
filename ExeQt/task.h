/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QList>

class Task : public QObject
{
	Q_OBJECT

private:
	friend class TaskManager;

public:
	Task();

private:
	void runTask();

signals:
	void run();
};

typedef QList<Task*> TaskList;

class TaskManager : public QObject
{
	Q_OBJECT

private:
	static TaskManager* s_Instance;

private:
	TaskList m_Tasks;

public:
	~TaskManager();

	static inline TaskManager* instance() { return s_Instance; }

	static void init();
	static void terminate();

public:
	void submitTask(Task*);
	void removeTask(Task*);

	void run();
};

#endif // TASK_H
