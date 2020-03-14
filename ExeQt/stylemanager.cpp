/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "stylemanager.h"

#include <QFile>

StyleManager* StyleManager::s_Instance = nullptr;

StyleManager::StyleManager()
{
}

bool StyleManager::setStyleFile(const QString& styleFilePath)
{
	QFile styleFile(styleFilePath);

	if (!styleFile.open(QFile::ReadOnly))
	{
		return false;
	}

	m_Style = QString::fromLatin1(styleFile.readAll());
	styleFile.close();

	return true;
}

void StyleManager::init()
{
	s_Instance = new StyleManager();
}

void StyleManager::terminate()
{
	delete s_Instance;
}
