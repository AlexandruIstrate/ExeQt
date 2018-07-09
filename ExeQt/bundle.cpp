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

Bundle& Bundle::childAt(int index)
{
	return m_Children[index];
}

const Bundle& Bundle::childAt(int index) const
{
	return m_Children.at(index);
}

void Bundle::setChildAt(int index, const Bundle& child)
{
	m_Children[index] = child;
}

bool Bundle::hasKey(const QString& key) const
{
	return m_Values.contains(key);
}

bool Bundle::equals(const Bundle& other) const
{
	return (m_Name == other.m_Name) && (m_Values == other.m_Values);
}

bool bundleEquals(const Bundle& bundle1, const Bundle& bundle2)
{
	if (!bundle1.equals(bundle2))
		return false;

	if (bundle1.getChildrenCount() != bundle2.getChildrenCount())
		return false;

	for (int i = 0; i < bundle1.getChildrenCount(); ++i)
	{
		const Bundle& b1 = bundle1.childAt(i);

		for (int j = 0; j < bundle2.getChildrenCount(); ++j)
		{
			const Bundle& b2 = bundle2.childAt(j);

			if (!b1.equals(b2) && j == bundle2.getChildrenCount() - 1)	// Reached the last item and we haven't found a match
				return false;
		}
	}

	return true;
}

bool Bundle::strongEquals(const Bundle& other) const
{
	return bundleEquals(*this, other);
}

void writeXMLBundle(Bundle bundle, QXmlStreamWriter& writer)
{
	QMap<QString, QString> map = bundle.getValues();
	for (QMap<QString, QString>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i)
		writer.writeAttribute(i.key(), i.value());

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
		return false;

	QTextStream stream(&file);
	stream << toXML();

	file.close();

	return true;
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
	Common::readFromFile(fileName, text);

	return Bundle::fromXML(text);
}

Bundle combineBundles(const Bundle& bundle1, const Bundle& bundle2)
{
	/*
	 *	1. Take each node of a bundle
	 *	2. Check to see if it exists in the other one (yes - compare further down with recursion; no - go to the next node)
	 */

	Bundle ret = Bundle::from(bundle1);

	for (const Bundle& b1 : bundle1.getChildren())
		ret.addChild(b1);

	for (const Bundle& b2 : bundle2.getChildren())
	{
		// Find correct node
		bool found = false;

		for (int i = 0; i < ret.getChildrenCount(); ++i)
		{
			const Bundle& b1 = ret.childAt(i);

			if (b1.strongEquals(b2))	// Found this node in the other tree
			{
				ret.setChildAt(i, combineBundles(b1, b2));

				found = true;
				break;
			}
		}

		if (!found)
			ret.addChild(b2);
	}

	return ret;
}

Bundle Bundle::getBundleDiff(const Bundle& bundle1, const Bundle& bundle2)
{
	return combineBundles(bundle1, bundle2);
}
