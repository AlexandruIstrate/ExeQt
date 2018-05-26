#ifndef BUNDLE_H
#define BUNDLE_H

#include <QString>
#include <QVector>
#include <QMap>

typedef QMap<QString, QString> ValueMap;

class Bundle
{
private:
	QString m_Name;

	ValueMap m_Values;
	QVector<Bundle> m_Children;

public:
	Bundle(const QString& name = QString());
	Bundle(const Bundle&);

	inline const QString& getName() const { return m_Name; }
	inline void setName(const QString& name) { m_Name = name; }

	inline const QVector<Bundle>& getChildren() const { return m_Children; }
	inline const ValueMap& getValues() const { return m_Values; }

	inline int getChildrenCount() const { return m_Children.size(); }

public:
	void add(const QString& key, QString value);
	QString get(const QString& key) const;

	void addChild(Bundle bundle);

	Bundle& childAt(int index);
	const Bundle& childAt(int index) const;

	void setChildAt(int index, const Bundle& child);

	bool equals(const Bundle&) const;
	bool strongEquals(const Bundle&) const;

	QString toXml();

public:
	static Bundle from(const Bundle&);
	static Bundle fromXml(const QString& xml);

	static Bundle getBundleDiff(const Bundle&, const Bundle&);
};

#endif // BUNDLE_H
