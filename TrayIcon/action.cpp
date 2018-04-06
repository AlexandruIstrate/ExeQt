#include "action.h"
#include "commandaction.h"
#include "applicationaction.h"
#include "linkaction.h"

#define NAME_PROPERTY "name"
#define TYPE_PROPERTY "type"

Action::Action(const QString& name, Type type, QWidget* parent)
    : QWidget { parent }, m_Type { type }, m_Name { name }
{

}

Action::~Action()
{

}

QString Action::getTagName() const
{
    return QString("action");
}

void Action::readProperties(Bundle& bundle)
{
    m_Name = bundle.get(NAME_PROPERTY).toInt();
    m_Type = (Type) bundle.get(TYPE_PROPERTY).toInt();
}

void Action::writeProperties(Bundle& bundle)
{
    bundle.add(NAME_PROPERTY, m_Name);
    bundle.add(TYPE_PROPERTY, QString::number((int) m_Type));
}

Action* Action::create(Type type, QWidget* parent)
{
    switch (type)
    {
    case Type::COMMAND:
        return new CommandAction("", parent);

    case Type::APPLICATION:
        return new ApplicationAction("", parent);

    case Type::LINK:
        return new LinkAction("", parent);

    default:
        return nullptr;
    }
}

QString Action::getTypeName(Action::Type type)
{
    switch (type) {
    case Type::COMMAND:
        return tr("Command Action");

    case Type::APPLICATION:
        return tr("Application Action");

    case Type::LINK:
        return tr("Link Action");

    default:
        return tr("Unknown Action");
    }
}

QIcon Action::getActionIcon(Action::Type type)
{
    switch (type) {
    case Type::COMMAND:
        return QIcon(":/assets/images/action-types/terminal.png");

    case Type::APPLICATION:
        return QIcon(":/assets/images/action-types/applications.png");

    case Type::LINK:
        return QIcon(":/assets/images/action-types/link.png");

    default:
        return QIcon();
    }
}
