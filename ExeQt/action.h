#ifndef ACTION_H
#define ACTION_H

#include <QWidget>
#include <QAction>
#include <QString>
#include <QIcon>

#include "saveable.h"

class Action : public QWidget, public Saveable
{
public:
    enum class Type
    {
        COMMAND, APPLICATION, LINK
    };

protected:
    Type m_Type;
    QString m_Name;

public:
    Action(const QString& name, Type type, QWidget* parent = nullptr);
    virtual ~Action();

    inline void setName(const QString& name) { m_Name = name; }
    inline const QString& getName() const { return m_Name; }

    inline Type getType() const { return m_Type; }

    virtual QString getTagName() const override;

    virtual bool checkBundle(const Bundle&) const override;

    virtual void readProperties(Bundle&) override;
    virtual void writeProperties(Bundle&) override;

public:
    virtual void execute() = 0;

public:
    static Action* create(Type type, QWidget* parent = nullptr);
    static QString getTypeName(Type type);
    static QIcon getActionIcon(Type type);
};

#endif // ACTION_H
