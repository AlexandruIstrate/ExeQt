#include "saveable.h"

#include <QDebug>
#include <QFile>

#include <QMessageBox>

bool Saveable::checkProperty(const Bundle &bundle, const QString &propertyName) const
{
	if (!bundle.get(propertyName).isEmpty())
		return true;

	QMessageBox::critical(nullptr, QWidget::tr("XML Parse Error"), QWidget::tr("Property \"") + propertyName + QWidget::tr("\" not found for tag \"") + getTagName() + QWidget::tr("\"."));
	return false;
}

bool Saveable::checkBundle(const Bundle& bundle) const
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

bool Saveable::writeToFile(const QString& filePath, const QString& text)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream stream(&file);
	stream << text;

	file.close();

	return true;
}

bool Saveable::readFromFile(const QString& filePath, QString& outText)
{
	QFile inputFile(filePath);
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream in(&inputFile);
	QString text;

	while (!in.atEnd())
	   text.append(in.readLine());

	inputFile.close();
	outText = text;

	return true;
}
