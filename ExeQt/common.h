/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <QString>

class Common
{
public:
	static bool writeToFile(const QString& filePath, const QString& text);
	static bool readFromFile(const QString& filePath, QString& outText);
};

#endif // COMMON_H
