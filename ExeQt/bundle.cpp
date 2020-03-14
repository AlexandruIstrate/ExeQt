/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "bundle.h"

#include <QDebug>
#include <QFile>

#include <QDomDocument>
#include <QXmlStreamWriter>

#include "common.h"

Bundle::Bundle(const QString& name) : m_Name { name }
{
}

Bundle::Bundle(const Bundle& other) : m_Name { other.m_Name }, m_Values { other.m_Values }, m_Children { other.m_Children }
{
}

void Bundle::add(const QString& key, QString value)
{
	m_Values.insert(key, value);
}

QString Bundle::get(const QString& key) const
{
	return m_Values.value(key);
}

void Bundle::addChild(Bundle bundle)
{
	m_Children.append(bundle);
}

void Bundle::setChildAt(int index, const Bundle& child)
{
	m_Children[index] = child;
}

Bundle& Bundle::childAt(int index)
{
	return m_Children[index];
}

const Bundle& Bundle::childAt(int index) const
{
	return m_Children.at(index);
}

bool Bundle::hasKey(const QString& key) const
{
	return m_Values.contains(key);
}

bool Bundle::hasChild(const Bundle& child) const
{
	return m_Children.contains(child);
}

QString Bundle::toText() const
{
	QString text = QString("Bundle : %1 { ").arg(m_Name);

	for (ValueMap::const_iterator iter = m_Values.constBegin(); iter != m_Values.constEnd(); ++iter)
	{
		text.append(QString("%1 : %2; ").arg(iter.key(), iter.value()));
	}

	text.append("}");

	return text;
}

void writeXMLBundle(Bundle bundle, QXmlStreamWriter& writer)
{
	QMap<QString, QString> map = bundle.getValues();

	for (QMap<QString, QString>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i)
	{
		writer.writeAttribute(i.key(), i.value());
	}

	for (int i = 0; i < bundle.getChildrenCount(); ++i)
	{
		const Bundle& child = bundle.childAt(i);

		writer.writeStartElement(child.getName());
		writeXMLBundle(child, writer);
		writer.writeEndElement();
	}
}

QString Bundle::toXML() const
{
	QString str;

	QXmlStreamWriter writer(&str);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement(getName());

	writeXMLBundle(*this, writer);

	writer.writeEndDocument();

	return str;
}

bool Bundle::saveToFile(const QString& filePath) const
{
	QFile file(filePath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream stream(&file);
	stream << toXML();

	file.close();

	return true;
}

bool Bundle::operator==(const Bundle& other) const
{
	return (m_Name == other.m_Name) && (m_Values == other.m_Values)/* && (m_Children == other.m_Children)*/;
}

bool Bundle::operator!=(const Bundle& other) const
{
	return !(*this == other);
}

QDataStream& operator<<(QDataStream& stream, const Bundle& bundle)
{
	stream << QString("Bundle : {%1}").arg(bundle.getName());
	return stream;
}

Bundle Bundle::from(const Bundle& source)
{
	Bundle ret;
	ret.m_Name = source.m_Name;
	ret.m_Values = source.m_Values;

	return ret;
}

void processBundle(Bundle& bundle, QDomElement& node)
{
	QDomNamedNodeMap map = node.attributes();

	for (int i = 0; i < map.length(); ++i)
	{
		QDomNode attrib = map.item(i);
		bundle.add(attrib.nodeName(), attrib.nodeValue());
	}

	QDomNodeList nodes = node.childNodes();

	for (int i = 0; i < nodes.size(); ++i)
	{
		QDomElement elem = nodes.at(i).toElement();
		Bundle child(elem.tagName());

		processBundle(child, elem);
		bundle.addChild(child);
	}
}

Bundle Bundle::fromXML(const QString& xml)
{
	QDomDocument doc("doc");
	doc.setContent(xml);

	QDomElement docRoot = doc.documentElement();

	Bundle bundle(docRoot.tagName());
	processBundle(bundle, docRoot);

	return bundle;
}

Bundle Bundle::fromFile(const QString& fileName)
{
	QString text;

	if (!Common::readFromFile(fileName, text))
	{
		text = "<mainwidget><mainwidget/>";
	}

	return Bundle::fromXML(text);
}

Bundle Bundle::mergeBundles(const Bundle& bundle1, const Bundle& bundle2)
{
	Bundle result(bundle1);

	for (const Bundle& bundle : bundle2.getChildren())
	{
		if (!result.hasChild(bundle))
		{
			result.addChild(bundle);
			continue;
		}

		for (int i = 0; i < result.getChildrenCount(); ++i)
		{
			const Bundle& resultBundle = result.childAt(i);

			if (bundle == resultBundle)
			{
				result.setChildAt(i, mergeBundles(resultBundle, bundle));
				break;
			}
		}
	}

	return result;
}
