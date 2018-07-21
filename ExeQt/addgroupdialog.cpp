/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "addgroupdialog.h"
#include "ui_addgroupdialog.h"

#include <QDebug>
#include <QPushButton>

#define IMAGE_ASSET_RES ":/assets/images/group-icons/512"

const QVector<QString> AddGroupDialog::s_IconNames { "Blue", "Bronze", "Gold", "Green", "Ivory", "Orange", "Purple", "Red", "Silver" };
QVector<QIcon> AddGroupDialog::s_Icons;

AddGroupDialog::AddGroupDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::AddGroupDialog)
{
	ui->setupUi(this);
	setupUI();
}

AddGroupDialog::AddGroupDialog(ActionTab* tab) : AddGroupDialog { (QWidget*) tab }
{
	ui->edtName->setText(tab->getName());
	ui->cmbIcon->setCurrentText(tab->getIcon().name);

	if (exec() != QDialog::DialogCode::Rejected)
	{
		tab->setName(m_Name);
		tab->setIcon(getIcon());
	}
}

AddGroupDialog::~AddGroupDialog()
{
	delete ui;
}

AppIcon AddGroupDialog::getIcon()
{
	int index = ui->cmbIcon->currentIndex();
	return AppIcon(ui->cmbIcon->itemIcon(index), s_IconNames[index]);
}

void AddGroupDialog::initIcons()
{
	for (const QString& name : s_IconNames)
		s_Icons.push_back(QIcon(QString(":/assets/images/group-icons/512") + name + QString(".png")));
}

void AddGroupDialog::addIcon(const QIcon& icon)
{
	s_Icons.push_back(icon);
}

QString AddGroupDialog::getIconName(const AppIcon& icon)
{
	for (const QString& iconName : s_IconNames)
		if (icon.name == iconName)
			return iconName;

	return QString();
}

AppIcon AddGroupDialog::getIconByName(const QString& name)
{
	for (int i = 0; i < s_IconNames.size(); ++i)
		if (name == s_IconNames[i])
			return AppIcon(s_Icons[i], name);

	return AppIcon(s_Icons[0], name);
}

void AddGroupDialog::setupUI()
{
	setupDialogButtons();
	setupIcons();
}

void AddGroupDialog::setupDialogButtons()
{
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setIcon(QIcon(":/assets/images/button-icons/ok.png"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setIcon(QIcon(":/assets/images/button-icons/cancel.png"));
}

void AddGroupDialog::addIcon(const QIcon& icon, const QString& name)
{
	ui->cmbIcon->addItem(icon, name);
}

void AddGroupDialog::setupIcons()
{
	for (const QString& name : s_IconNames)
		addIcon(QIcon(QString(IMAGE_ASSET_RES) + name + QString(".png")), name);
}

void AddGroupDialog::accept()
{
	m_Name = ui->edtName->text();

	int index = ui->cmbIcon->currentIndex();
	m_Icon = ui->cmbIcon->itemIcon(index);

	QDialog::accept();
}
