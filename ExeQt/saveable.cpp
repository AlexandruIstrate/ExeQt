/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "saveable.h"

#include <QDebug>
#include <QFile>

#include <QMessageBox>

bool Saveable::checkProperty(const Bundle &bundle, const QString &propertyName) const
{
	if (!bundle.get(propertyName).isEmpty())
		return true;

//	QMessageBox::critical(nullptr, QWidget::tr("XML Parse Error"), QWidget::tr("Property \"") + propertyName + QWidget::tr("\" not found for tag \"") + getTagName() + QWidget::tr("\"."));
	return false;
}

bool Saveable::checkBundle(const Bundle&) const
{
//	if (bundle.getName() != getTagName())
//	{
//		QMessageBox::critical(nullptr, QWidget::tr("Invalid Tag Name"), QWidget::tr(QString("The tag name is invalid for this item (\"%1\")!").arg(getTagName()).toStdString().c_str()));
//		return false;
//	}

	return true;
}

void Saveable::initBundle()
{
	// No-Op
}
