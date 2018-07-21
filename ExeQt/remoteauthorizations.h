/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef REMOTEAUTHORIZATIONS_H
#define REMOTEAUTHORIZATIONS_H

#include <QDialog>

namespace Ui {
class RemoteAuthorizations;
}

class RemoteAuthorizations : public QDialog
{
	Q_OBJECT

private:
	Ui::RemoteAuthorizations* ui;

public:
	explicit RemoteAuthorizations(QWidget* parent = nullptr);
	~RemoteAuthorizations();

private:
	void setupUI();
	void setupDialogButtons();
};

#endif // REMOTEAUTHORIZATIONS_H
