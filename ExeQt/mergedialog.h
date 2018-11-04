/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/
#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>

#include "actiondisplaytree.h"

namespace Ui {
class MergeDialog;
}

class MergeDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::MergeDialog* ui;

	ActionDisplayTree m_LocalTree;
	ActionDisplayTree m_CloudTree;
	ActionDisplayTree m_MergedTree;

	Bundle m_LocalBundle;
	Bundle m_CloudBundle;
	Bundle m_MergedBundle;

public:
	MergeDialog(const Bundle& local, const Bundle& cloud, QWidget* parent = nullptr);
	~MergeDialog();

	inline const Bundle& getMergeResult() const { return m_MergedBundle; }

private:
	void setupUI();
	void setupSignalsAndSlots();

	void mergeBundles();

	void showMerged();
	void showLocalAndCloud();

private slots:
	void onConfirm();
	void onDeny();

	void onShowMergeChanged(bool checked);
};

#endif // MERGEDIALOG_H
