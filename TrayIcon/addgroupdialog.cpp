#include "addgroupdialog.h"
#include "ui_addgroupdialog.h"

#include <QDebug>

QVector<QString> AddGroupDialog::s_IconNames { "Blue", "Bronze", "Gold", "Green", "Ivory", "Orange", "Purple", "Red", "Silver" };

AddGroupDialog::AddGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGroupDialog)
{
    ui->setupUi(this);
    initUI();
}

AddGroupDialog::AddGroupDialog(ActionTab* tab) :
    QDialog(tab),
    ui(new Ui::AddGroupDialog)
{
    ui->setupUi(this);
    initUI();

    ui->edtName->setText(tab->getName());

    if (exec() == -1)
        return;

    tab->setName(m_Name);
    tab->setIcon(m_Icon);
}

AddGroupDialog::~AddGroupDialog()
{
    delete ui;
}

void AddGroupDialog::initUI()
{
    setupIcons();
}

void AddGroupDialog::addIcon(const QIcon& icon, const QString& name)
{
    ui->cmbIcon->addItem(icon, name);
}

void AddGroupDialog::setupIcons()
{
    for (const QString& name : s_IconNames)
        addIcon(QIcon(QString(":/assets/images/group-icons/512") + name + QString(".png")), name);
}

void AddGroupDialog::accept()
{
    m_Name = ui->edtName->text();

    int index = ui->cmbIcon->currentIndex();
    m_Icon = ui->cmbIcon->itemIcon(index);

    QDialog::accept();
}
