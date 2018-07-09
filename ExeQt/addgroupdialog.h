/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QDialog>
#include <QString>
#include <QIcon>
#include <QVector>

#include "actiontab.h"
#include "appicon.h"

namespace Ui {
	class AddGroupDialog;
}

class AddGroupDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AddGroupDialog* ui;

	static const QVector<QString> s_IconNames;
	static QVector<QIcon> s_Icons;

	QString m_Name;
	QIcon m_Icon;

public:
	explicit AddGroupDialog(QWidget* parent = nullptr);
	explicit AddGroupDialog(ActionTab* tab);
	~AddGroupDialog();

	inline const QString& getName() const { return m_Name; }
	inline const QIcon& getIcon() const { return m_Icon; }

	AppIcon getIcon();

public:
	static void initIcons();

	static QString getIconName(const AppIcon& icon);
	static AppIcon getIconByName(const QString& name);

private:
	static void addIcon(const QIcon&);

private:
	void initUI();

	void addIcon(const QIcon& icon, const QString& name);
	void setupIcons();

public slots:
	void accept() override;
};

#endif // ADDGROUPDIALOG_H
