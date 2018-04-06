#include "actiontab.h"
#include "ui_actiontab.h"

#include <QDebug>
#include <QListWidgetItem>

#include "commandaction.h"
#include "menuaction.h"

#define NAME_PROPERTY           "name"
#define ICON_PROPERTY           "icon"
#define ACTION_COUNT_PROPERTY   "actionCount"

ActionTab::ActionTab(const QString& name, const QIcon& icon, QWidget* parent) :
    QWidget(parent), ui(new Ui::ActionTab),
    m_Name { name }, m_Icon { icon }
{
    ui->setupUi(this);

    setupUI();
    setupActions();
    setupSignalsAndSlots();
}

ActionTab::~ActionTab()
{
    delete ui;
}

QString ActionTab::getTagName() const
{
    return QString("actionTab");
}

void ActionTab::readProperties(Bundle& bundle)
{
    m_Name = bundle.get(NAME_PROPERTY);
    m_Icon = QIcon(bundle.get(ICON_PROPERTY));
    int actionCount = bundle.get(ACTION_COUNT_PROPERTY).toInt();

    for (int i = 0; i < actionCount; ++i)
    {
        ActionItem* item = new ActionItem(this);
        item->readProperties(bundle.childAt(i));
        addAction(item);
    }
}

void ActionTab::writeProperties(Bundle& bundle)
{
    bundle.add(NAME_PROPERTY, m_Name);
    bundle.add(ICON_PROPERTY, m_Icon.name());
    bundle.add(ACTION_COUNT_PROPERTY, QString::number(m_ActionItems.size()));

    for (ActionItem* item : m_ActionItems)
    {
        Bundle child(item->getTagName());
        item->writeProperties(child);

        bundle.addChild(child);
    }
}

void ActionTab::setupActions()
{
    m_AddAction = new QAction(tr("Add"), this);
    connect(m_AddAction, SIGNAL(triggered()), this, SLOT(onActionAdd()));

    m_RemoveAction = new QAction(tr("Remove"), this);
    connect(m_RemoveAction, SIGNAL(triggered()), this, SLOT(onActionRemove()));

    m_EditAction = new QAction(tr("Edit"), this);
    connect(m_EditAction, SIGNAL(triggered()), this, SLOT(onActionEdit()));

    addGhostAction();
}

void ActionTab::setupSignalsAndSlots()
{
    connect(ui->btnAdd, SIGNAL(clicked()), m_AddAction, SLOT(trigger()));
    connect(ui->btnRemove, SIGNAL(clicked()), m_RemoveAction, SLOT(trigger()));
    connect(ui->btnEdit, SIGNAL(clicked()), m_EditAction, SLOT(trigger()));
}

void ActionTab::setupUI()
{
    setupTrayIcon();
    ui->actionList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ActionTab::setupTrayIcon()
{
    m_TrayMenu = new QMenu();

    m_TrayIcon = new QSystemTrayIcon(this);
    m_TrayIcon->setContextMenu(m_TrayMenu);
    m_TrayIcon->setIcon(m_Icon);
    m_TrayIcon->show();
}

void ActionTab::addAction(ActionItem* action)
{
    m_ActionItems.push_back(action);
    ui->actionList->addItem(action->getName());

    updateActions();
}

void ActionTab::removeAction(ActionItem* action)
{
    for (int i = 0; i < m_ActionItems.size(); ++i)
        if (m_ActionItems[i] == action)
            m_ActionItems.erase(m_ActionItems.begin() + i);

    updateActions();
}

void ActionTab::updateActions()
{
    ui->actionList->clear();
    m_TrayMenu->clear();

    for (ActionItem* actionItem : m_ActionItems)
    {
        Action* action = actionItem->getAction();
        ui->actionList->addItem(new QListWidgetItem(Action::getActionIcon(action->getType()), action->getName()));

        MenuAction* menuAct = new MenuAction(action, this);
        m_TrayMenu->addAction(menuAct);
    }
}

void ActionTab::addGhostAction()
{
    m_GhostAction = new QAction("No Actions");
    m_GhostAction->setEnabled(false);
    m_TrayMenu->addAction(m_GhostAction);
}

void ActionTab::removeGhostAction()
{
    if (m_GhostAction)
    {
        m_TrayMenu->removeAction(m_GhostAction);
        m_GhostAction->deleteLater();

        m_GhostAction = nullptr;
    }
}

void ActionTab::onActionAdd()
{
    ActionItem* action = new ActionItem(this);
    if (!action->exec())
        return;

    removeGhostAction();
    addAction(action);
}

void ActionTab::onActionRemove()
{
    int line = ui->actionList->currentRow();
    if (line == -1)
        return;

    ActionItem* action = m_ActionItems[line];
    removeAction(action);
}

void ActionTab::onActionEdit()
{
    int line = ui->actionList->currentRow();
    if (line == -1)
        return;

    ActionItem* action = m_ActionItems[line];
    action->exec();

    if (m_TrayIcon)
        delete m_TrayIcon;

    setupTrayIcon();
    updateActions();

    qDebug() << m_ActionItems.size();
}
