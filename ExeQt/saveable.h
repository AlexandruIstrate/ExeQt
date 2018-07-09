/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef SAVEABLE_H
#define SAVEABLE_H

#include <QtXml>
#include <QString>

#include "bundle.h"

class Saveable
{
public:
	bool checkProperty(const Bundle& bundle, const QString& propertyName) const;

	virtual QString getTagName() const = 0;

	virtual bool checkBundle(const Bundle&) const;

	virtual void readProperties(Bundle&) = 0;
	virtual void writeProperties(Bundle&) = 0;

protected:
	virtual void initBundle();
};

#endif // SAVEABLE_H
