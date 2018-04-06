#ifndef MENUACTION_H
#define MENUACTION_H

#include <QAction>
#include <QIcon>
#include <QString>

#include "action.h"

class MenuAction : public QAction
{
    Q_OBJECT

private:
    Action* m_Action;

public:
    MenuAction(Action* action, QObject* parent = nullptr);

private slots:
    void onTrigger();
};

#endif // MENUACTION_H
