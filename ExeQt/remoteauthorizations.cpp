/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "remoteauthorizations.h"
#include "ui_remoteauthorizations.h"

#include <QDebug>

#include "remoteauthtab.h"
#include "authconnecttab.h"

RemoteAuthorizations::RemoteAuthorizations(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::RemoteAuthorizations)
{
	ui->setupUi(this);
	setupUI();
}

RemoteAuthorizations::~RemoteAuthorizations()
{
	delete ui;
}

void RemoteAuthorizations::setupUI()
{
	setupDialogButtons();

	ui->tabGrpAuth->addTab(new AuthConnectTab(this), QIcon(":/assets/images/connect.png"), tr("Connect"));
	ui->tabGrpAuth->addTab(new RemoteAuthTab(RemoteAuthTab::AccessType::ACCESSES_OTHER, this), QIcon(":/assets/images/other.png"), tr("Connected To"));
	ui->tabGrpAuth->addTab(new RemoteAuthTab(RemoteAuthTab::AccessType::ACCESSES_THIS, this), QIcon(":/assets/images/current.png"), tr("Connected From"));
}

void RemoteAuthorizations::setupDialogButtons()
{
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setIcon(QIcon(":/assets/images/button-icons/ok.png"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setIcon(QIcon(":/assets/images/button-icons/cancel.png"));
}
