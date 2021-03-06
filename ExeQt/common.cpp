/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "common.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

bool Common::writeToFile(const QString& filePath, const QString& text)
{
	QFile file(filePath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream stream(&file);
	stream << text;

	file.close();

	return true;
}

bool Common::readFromFile(const QString& filePath, QString& outText)
{
	QFile inputFile(filePath);

	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream in(&inputFile);
	QString text;

	while (!in.atEnd())
	{
	   text.append(in.readLine());
	}

	inputFile.close();
	outText = text;

	return true;
}

QString Common::getSaveFilePath()
{
	QString saveLocation = SettingsRegistry::instance()->get(Settings::SAVE_PATH).toString();
	QFileInfo fileInfo(saveLocation);

	if (fileInfo.isDir())
	{
		saveLocation += (QDir::separator() + Constants::DEFAULT_SAVE_FILE_NAME);
	}

	return QDir::cleanPath(saveLocation);
}
