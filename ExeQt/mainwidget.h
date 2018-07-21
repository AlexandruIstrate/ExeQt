/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QAction>
#include <QWidget>
#include <QPoint>
#include <QList>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>

#include "actiontab.h"
#include "saveable.h"
#include "requestmanager.h"
#include "constants.h"

typedef QList<ActionTab*> ActionTabList;

namespace Ui {
	class MainWidget;
}

class RemoteControl;

class MainWidget : public QWidget, public Saveable
{
	Q_OBJECT

private:
	friend class GroupConfigure;

private:
	static MainWidget* s_Instance;

private:
	Ui::MainWidget* ui;

	QAction* m_ConfigureAction;
	QAction* m_ConfigureGroupsAction;
	QAction* m_QuitAction;

	QAction* m_SyncAction;
	QAction* m_RemoteControlAction;
	QAction* m_AuthAction;
	QAction* m_SettingsAction;

	QAction* m_AddTabAction;
	QAction* m_RemoveTabAction;

	QSystemTrayIcon* m_TrayIcon;

	ActionTabList m_ActionTabs;

	RequestManager* m_RequestManager;
	RemoteControl* m_RemoteControlDialog;

	bool m_ShouldQuit;

public:
	explicit MainWidget(QWidget* parent = nullptr);
	~MainWidget();

	inline static MainWidget* instance() { return s_Instance; }
	inline ActionTabList getActionTabs() const { return m_ActionTabs; }

	QIcon getTabIcon(int index);

	void addNewActionGroup();
	void removeActionGroup(int index);

	QString getTagName() const override;

	void readProperties(Bundle&) override;
	void writeProperties(Bundle&) override;

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void setupNetwork();
	void setupActions();
	void setupSignalsAndSlots();

	void setupUI();
	void setupTrayIcon();
	QMenu* createNetworkMenu();

	void setupTabs();
	void insertTab(ActionTab*);
	void addTab(ActionTab*);
	void removeTab(ActionTab*);

	void openGroupConfigureMenu();
	void reloadTabs();
	void removeTabs();

	void saveToFile();
	void loadFromFile();

	int getTabCount();
	int getSelectedTabIndex();

	QString parseResponse(const QString& json);

	void quitLater();

private slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
	void onTabSelected(int index);

	void onAddTab();
	void onRemoveTab();

	void onConfigure();
	void onConfigureActionGroups();
	void onSyncSelected();
	void onRemoteControlSelected();
	void onAuthorizationsSelected();
	void onSettingsSelected();
	void onQuit();

	void onRequestFinished(QNetworkReply* reply, bool timedOut);

	void onActionsCanUpdate();
};

#endif // MAINWIDGET_H
