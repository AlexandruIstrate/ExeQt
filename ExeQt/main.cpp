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

void setupIcons()
{
	IconManager* im = IconManager::instance();
	im->registerIcon("Not Found", ":/assets/images/action-custom-icons/notfound.png");
	im->registerIcon("File", ":/assets/images/action-custom-icons/file.png");
	im->registerIcon("Web", ":/assets/images/action-custom-icons/web.png");
	im->registerIcon("Shell", ":/assets/images/action-custom-icons/shell.png");
	im->registerIcon("Monitor", ":/assets/images/action-custom-icons/monitor.png");
	im->registerIcon("Keyboard", ":/assets/images/action-custom-icons/keyboard.png");
	im->registerIcon("Mouse", ":/assets/images/action-custom-icons/mouse.png");
}

int main(int argc, char* argv[])
{
	try
	{
		SettingsRegistry::init();
		AuthManager::init();
		IconManager::init();

		StyleManager::init();
		StyleManager::instance()->setStyleFile(STYLE_FILE);

		QtSingleApplication instance(argc, argv);
		if (instance.sendMessage(WAKE_MESSAGE))
		{
			onInstanceRunning();
			return 0;
		}

		instance.setQuitOnLastWindowClosed(false);
		instance.setStyle(StyleManager::instance()->getStyle());

		setupIcons();

		QWidget* window = SettingsRegistry::instance()->get(Settings::SHOW_MAIN_PAGE).toBool() ?
					static_cast<QWidget*>(new MainPage()) :
					static_cast<QWidget*>(new MainWidget());
		window->show();

		instance.setActivationWindow(window);
		int result = instance.exec();

		IconManager::terminate();
		SettingsRegistry::terminate();
		AuthManager::terminate();
		StyleManager::terminate();

		delete window;
		return result;
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(nullptr, QObject::tr("Fatal Error"), QObject::tr("The application encountered an error from which it could not recover!\n") + QString(e.what()));
		return -1;
	}
}
