#ifndef BUNDLE_H
#define BUNDLE_H

#include <QString>
#include <QVector>
#include <QMap>

class Bundle
{
private:
    QString m_Name;

    QMap<QString, QString> m_Values;
    QVector<Bundle> m_Children;

public:
    Bundle(const QString& name);
    Bundle();
    ~Bundle();

    inline const QString& getName() const { return m_Name; }

    inline const QVector<Bundle>& getChildren() const { return m_Children; }
    inline const QMap<QString, QString>& getValues() const { return m_Values; }

    inline int getChildrenCount() const { return m_Children.size(); }

    void add(const QString& key, QString value);
    QString get(const QString& key) const;

    void addChild(Bundle bundle);
    Bundle& childAt(int index);
};

#endif // BUNDLE_H
