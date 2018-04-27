#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QAction>
#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QSystemTrayIcon>
#include <QApplication>

#include "actiontab.h"
#include "saveable.h"

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

    QAction* m_AddTabAction;
    QAction* m_RemoveTabAction;

    QSystemTrayIcon* m_TrayIcon;

    QVector<ActionTab*> m_ActionTabs;

    QString m_SaveFile = QApplication::applicationDirPath() + "/exeqt-save.xml";

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
    void setupActions();
    void setupSignalsAndSlots();

    void setupUI();
    void setupTrayIcon();

    void setupTabs();
    void insertTab(ActionTab*);
    void addTab(ActionTab*);

    void openGroupConfigureMenu();
    void reloadTabs();

    void saveToFile();
    void loadFromFile();

    int getTabCount();
    int getSelectedTabIndex();

    void writeBundle(Bundle bundle, QXmlStreamWriter& writer);
    void readBundle(Bundle& bundle, const QString& str);

private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onTabSelected(int index);

    void onAddTab();
    void onRemoveTab();

    void onConfigure();
    void onConfigureActionGroups();
    void onQuit();
};

#endif // MAINWIDGET_H
