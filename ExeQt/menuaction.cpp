/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

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
