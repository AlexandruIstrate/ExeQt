/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ADDTAB_H
#define ADDTAB_H

#include <QWidget>

namespace Ui {
class AddTab;
}

class AddTab : public QWidget
{
	Q_OBJECT

private:
	Ui::AddTab *ui;

public:
	explicit AddTab(QWidget* parent = nullptr);
	~AddTab();

private:
	void setupSignalsAndSlots();

signals:
	void createActionGroup();

private slots:
	void onAdd();
};

#endif // ADDTAB_H
