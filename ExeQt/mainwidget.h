#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QAction>
#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>

#include "actiontab.h"
#include "saveable.h"
#include "requestmanager.h"
#include "constants.h"

namespace Ui {
	class MainWidget;
}

class MainWidget : public QWidget, public Saveable
{
	Q_OBJECT

private:
	friend class GroupConfigure;

private:
	Ui::MainWidget* ui;

	QAction* m_ConfigureAction;
	QAction* m_ConfigureGroupsAction;
	QAction* m_QuitAction;

	QAction* m_SyncAction;
	QAction* m_SettingsAction;

	QAction* m_AddTabAction;
	QAction* m_RemoveTabAction;

	QSystemTrayIcon* m_TrayIcon;

	QVector<ActionTab*> m_ActionTabs;

	RequestManager* m_RequestManager;

	bool m_ShouldQuit;

	QString m_SaveFile = QApplication::applicationDirPath() + QString("/") + Constants::SAVE_FILE_NAME;

public:
	explicit MainWidget(QWidget* parent = nullptr);
	~MainWidget();

	QIcon getTabIcon(int index);

	void addNewActionGroup();
	void removeActionGroup(int index);

	QString getTagName() const override;

	bool checkBundle(const Bundle&) const override;

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
	void onSettingsSelected();
	void onQuit();

	void onRequestFinished(QNetworkReply* reply, bool timedOut);
};

#endif // MAINWIDGET_H
