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

public:
	static bool writeToFile(const QString& filePath, const QString& text);
	static bool readFromFile(const QString& filePath, QString& outText);
};

#endif // SAVEABLE_H
