/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include <QPushButton>

#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"

ClientInfoDialog::ClientInfoDialog(const QString& name, const QString& id, const QString& address, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::ClientInfoDialog)
{
	ui->setupUi(this);
	setupUI();

	setName(name);
	setID(id);
	setAddress(address);
}

ClientInfoDialog::~ClientInfoDialog()
{
	delete ui;
}

void ClientInfoDialog::setName(const QString& name)
{
	ui->lblName->setText(name);
}

void ClientInfoDialog::setID(const QString& id)
{
	ui->lblID->setText(id);
}

void ClientInfoDialog::setAddress(const QString& address)
{
	ui->lblAddress->setText(address);
}

void ClientInfoDialog::setupUI()
{
	setupDialogButtons();
}

void ClientInfoDialog::setupDialogButtons()
{
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setIcon(QIcon(":/assets/images/button-icons/ok.png"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setIcon(QIcon(":/assets/images/button-icons/cancel.png"));
}
