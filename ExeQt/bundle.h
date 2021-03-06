/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef BUNDLE_H
#define BUNDLE_H

#include <QString>
#include <QList>
#include <QMap>

typedef QMap<QString, QString> ValueMap;

class Bundle
{
private:
	QString m_Name;

	ValueMap m_Values;
	QList<Bundle> m_Children;

public:
	Bundle(const QString& name = QString());
	Bundle(const Bundle&);

	inline const QString& getName() const { return m_Name; }
	inline void setName(const QString& name) { m_Name = name; }

	inline const ValueMap& getValues() const { return m_Values; }

	inline const QList<Bundle>& getChildren() const { return m_Children; }
	inline void setChildren(const QList<Bundle> children) { m_Children = children; }

	inline int getChildrenCount() const { return m_Children.size(); }

public:
	void add(const QString& key, QString value);
	QString get(const QString& key) const;

	void addChild(Bundle bundle);
	void setChildAt(int index, const Bundle& child);

	Bundle& childAt(int index);
	const Bundle& childAt(int index) const;

	bool hasKey(const QString& key) const;
	bool hasChild(const Bundle&) const;

	QString toText() const;
	QString toXML() const;

	bool saveToFile(const QString& filePath) const;

public:
	bool operator==(const Bundle& other) const;
	bool operator!=(const Bundle& other) const;

	friend QDataStream& operator<<(QDataStream&, const Bundle&);

public:
	static Bundle from(const Bundle&);
	static Bundle fromXML(const QString& xml);
	static Bundle fromFile(const QString& fileName);

	static Bundle mergeBundles(const Bundle&, const Bundle&);
};

#endif // BUNDLE_H
