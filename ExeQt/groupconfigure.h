#ifndef GROUPCONFIGURE_H
#define GROUPCONFIGURE_H

#include <QDialog>
#include <QAction>
#include <QVector>

#include "actiontab.h"
#include "mainwidget.h"

namespace Ui {
class GroupConfigure;
}

class GroupConfigure : public QDialog
{
    Q_OBJECT

private:
    Ui::GroupConfigure* ui;

    QAction* m_ActionAdd;
    QAction* m_ActionRemove;
    QAction* m_ActionEdit;

    QVector<ActionTab*>& m_ActionTabs;

public:
    GroupConfigure(MainWidget* parent = nullptr);
    ~GroupConfigure();

private:
    void setupActions();
    void setupSignalsAndSlots();
    void setupUI();

    int getSelectedItem();
    void showActionGroups();

private slots:
    void onAdd();
    void onRemove();
    void onEdit();
};

#endif // GROUPCONFIGURE_H
