#ifndef ACTIONTAB_H
#define ACTIONTAB_H

#include <QWidget>
#include <QAction>
#include <QString>
#include <QVector>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QModelIndex>

#include "actionitem.h"
#include "saveable.h"
#include "appicon.h"

namespace Ui {
class ActionTab;
}

class ActionTab : public QWidget, public Saveable
{
    Q_OBJECT

private:
    Ui::ActionTab* ui;
    QSystemTrayIcon* m_TrayIcon;
    QMenu* m_TrayMenu;

    QAction* m_AddAction;
    QAction* m_RemoveAction;
    QAction* m_EditAction;

    QAction* m_GhostAction;

    QString m_Name;
    AppIcon m_Icon;

    QVector<ActionItem*> m_ActionItems;

public:
    ActionTab(const QString& name, const AppIcon &icon, QWidget* parent = nullptr);
    ~ActionTab();

    inline const QString& getName() const { return m_Name; }
    void setName(const QString&);

    inline const AppIcon& getIcon() const { return m_Icon; }
    void setIcon(const AppIcon&);

    QString getTagName() const override;

    bool checkBundle(const Bundle&) const override;

    void readProperties(Bundle&) override;
    void writeProperties(Bundle&) override;

private:
    void setupActions();
    void setupSignalsAndSlots();

    void setupUI();
    void setupTrayIcon();

    void addAction(ActionItem*);
    void removeAction(ActionItem*);

    void updateActions();
    void addGhostAction();
    void removeGhostAction();

private slots:
    void onActionAdd();
    void onActionRemove();
    void onActionEdit();

    void onDoubleClick();
};

#endif // ACTIONTAB_H
