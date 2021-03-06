/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef APPICON_H
#define APPICON_H

#include <QIcon>
#include <QString>

struct AppIcon
{
	QIcon icon;
	QString name;

	AppIcon(const QIcon& icon, const QString& name) : icon { icon }, name { name } {}
	AppIcon() {}

	friend bool operator==(const AppIcon& left, const AppIcon& right)
	{
		return left.name == right.name;
	}

	friend bool operator!=(const AppIcon& left, const AppIcon& right)
	{
		return !(left == right);
	}
};

#endif // APPICON_H
