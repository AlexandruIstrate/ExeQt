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
	QAction* m_ActionContinue;

public:
	explicit MainPage(QWidget* parent = nullptr);
	~MainPage();

protected:
	void closeEvent(QCloseEvent*) override;

private:
	void setupActions();
	void setupSignalsAndSlots();

	void loadSettings();

private slots:
	void onSync();
	void onContinue();
};

#endif // MAINPAGE_H
