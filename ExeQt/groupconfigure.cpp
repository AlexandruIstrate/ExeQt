#include "groupconfigure.h"
#include "ui_groupconfigure.h"

#include <QDebug>
#include <QListWidgetItem>

#include "addgroupdialog.h"

GroupConfigure::GroupConfigure(MainWidget* parent) :
    QDialog(parent), ui(new Ui::GroupConfigure),
    m_ActionTabs { parent->m_ActionTabs }
{
    ui->setupUi(this);

    setupUI();
    setupActions();
    setupSignalsAndSlots();
}

GroupConfigure::~GroupConfigure()
{
    delete ui;
}

void GroupConfigure::setupActions()
{
    m_ActionAdd = new QAction(tr("Add"), this);
    connect(m_ActionAdd, SIGNAL(triggered()), this, SLOT(onAdd()));

    m_ActionRemove = new QAction(tr("Remove"), this);
    connect(m_ActionRemove, SIGNAL(triggered()), this, SLOT(onRemove()));

    m_ActionEdit = new QAction(tr("Edit"), this);
    connect(m_ActionEdit, SIGNAL(triggered()), this, SLOT(onEdit()));
}

void GroupConfigure::setupSignalsAndSlots()
{
    connect(ui->btnAdd, SIGNAL(clicked()), m_ActionAdd, SLOT(trigger()));
    connect(ui->btnRemove, SIGNAL(clicked()), m_ActionRemove, SLOT(trigger()));
    connect(ui->btnEdit, SIGNAL(clicked()), m_ActionEdit, SLOT(trigger()));
}

void GroupConfigure::setupUI()
{
    showActionGroups();
}

int GroupConfigure::getSelectedItem()
{
    return ui->lstGroups->currentRow();
}

void GroupConfigure::showActionGroups()
{
    ui->lstGroups->clear();

    for (ActionTab* tab : m_ActionTabs)
        new QListWidgetItem(tab->getIcon().icon, tab->getName(), ui->lstGroups);
}

void GroupConfigure::onAdd()
{
    MainWidget* mw = (MainWidget*) parent();
    mw->addNewActionGroup();

    showActionGroups();
}

void GroupConfigure::onRemove()
{
    int selectedItem = getSelectedItem();
    if (selectedItem == -1)
        return;

    MainWidget* mw = (MainWidget*) parent();
    mw->removeActionGroup(selectedItem);

    delete ui->lstGroups->takeItem(selectedItem);

    showActionGroups();
}

void GroupConfigure::onEdit()
{
    AddGroupDialog* addDialog = new AddGroupDialog(m_ActionTabs.at(getSelectedItem()));
    addDialog->deleteLater();

    showActionGroups();
}
