/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDialog>
#include <QAction>
#include <QSettings>

namespace Ui {
class MainPage;
}

class MainPage : public QDialog
{
	Q_OBJECT

private:
	Ui::MainPage* ui;

	QAction* m_ActionSync;
	QAction* m_ActionSettings;
	QAction* m_ActionContinue;

public:
	explicit MainPage(QWidget* parent = nullptr);
	~MainPage();

protected:
	void closeEvent(QCloseEvent*) override;

private:
	void setupActions();
	void setupSignalsAndSlots();

private slots:
	void onSync();
	void onOpenSettings();
	void onContinue();
};

#endif // MAINPAGE_H
