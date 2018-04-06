#include "menuaction.h"

MenuAction::MenuAction(Action* action, QObject* parent) :
    QAction(Action::getActionIcon(action->getType()), action->getName(), parent), m_Action { action }
{
    connect(this, SIGNAL(triggered()), this, SLOT(onTrigger()));
}

void MenuAction::onTrigger()
{
    m_Action->execute();
}
