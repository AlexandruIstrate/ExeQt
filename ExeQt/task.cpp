/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "task.h"

#include <QDebug>

// Task

Task::Task()
{
}

void Task::runTask()
{
	emit run();
}

// Task Manager

TaskManager* TaskManager::s_Instance = nullptr;

TaskManager::~TaskManager()
{
	for (Task* task : m_Tasks)
	{
		delete task;
	}
}

void TaskManager::init()
{
	s_Instance = new TaskManager();
}

void TaskManager::terminate()
{
	delete s_Instance;
}

void TaskManager::submitTask(Task* task)
{
	m_Tasks.append(task);
}

void TaskManager::removeTask(Task* task)
{
	m_Tasks.removeOne(task);
}

void TaskManager::run()
{
	for (Task* task : m_Tasks)
	{
		task->runTask();
	}
}
