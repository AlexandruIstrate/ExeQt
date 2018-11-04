/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>
#include <QPushButton>

#include <QJsonDocument>
#include <QJsonObject>

#include "constants.h"
#include "common.h"
#include "saveable.h"
#include "mainwidget.h"
#include "authmanager.h"
#include "mergedialog.h"

LoginDialog::LoginDialog(QWidget* parent) :
	QDialog(parent), ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::setupSignalsAndSlots()
{
	connect(ui->btnLogIn, &QPushButton::clicked, this, &LoginDialog::onLogIn);
	connect(AuthManager::instance(), &AuthManager::doneLogin, this, &LoginDialog::onLoginDone);
	connect(AuthManager::instance(), &AuthManager::doneSync, this, &LoginDialog::onSyncDone);
}

void LoginDialog::onLogIn()
{
	ui->lblState->setText("Logging in...");
	AuthManager::instance()->authenticate(ui->edtUser->text(), ui->edtPassword->text());
}

void LoginDialog::onLoginDone()
{
	ui->lblState->setText("Synchronizing actions...");
	AuthManager::instance()->syncActions();
}

void LoginDialog::onSyncDone()
{
	ui->lblState->setText("Done");
	accept();
}
