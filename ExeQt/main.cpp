/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "mainwidget.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>

#include <qtsingleapplication.h>

#include "mainpage.h"
#include "authmanager.h"
#include "stylemanager.h"
#include "settingsregistry.h"

#define WAKE_MESSAGE "Are you running, sweetheart?"

#define STYLE_FILE ":/style.qss"

static void onInstanceRunning()
{
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setText("The application is already running.\n"
				   "Allowed to run only one instance of the application.");
	msgBox.exec();
}

int main(int argc, char* argv[])
{
	try
	{
		SettingsRegistry::init();
		AuthManager::init();

		StyleManager::init();
		StyleManager::instance()->setStyleFile(STYLE_FILE);

		QtSingleApplication instance(argc, argv);
		if (instance.sendMessage(WAKE_MESSAGE))
		{
			onInstanceRunning();
			return 0;
		}

		instance.setQuitOnLastWindowClosed(false);

		MainPage mp;
		mp.show();

		instance.setActivationWindow(&mp);
		int result = instance.exec();

		SettingsRegistry::terminate();
		AuthManager::terminate();
		StyleManager::terminate();

		return result;
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(nullptr, QObject::tr("Fatal Error"), QObject::tr("The application encountered an error from which it could not recover!\n") + QString(e.what()));
		return -1;
	}
}
