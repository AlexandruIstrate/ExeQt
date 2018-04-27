#ifndef APPLICATIONACTION_H
#define APPLICATIONACTION_H

#include <QWidget>
#include <QString>

#include "action.h"

namespace Ui {
    class ApplicationAction;
}

class ApplicationAction : public Action
{
    Q_OBJECT

private:
    Ui::ApplicationAction* ui;

    QString m_AppPath;

public:
    ApplicationAction(const QString& appPath, const QString& name = "", QWidget* parent = nullptr);
    ApplicationAction(const QString& name = "", QWidget* parent = nullptr);
    ~ApplicationAction();

    inline const QString& getAppPath() const { return m_AppPath; }

    QString getTagName() const override;

    void readProperties(Bundle&) override;
    void writeProperties(Bundle&) override;

    void execute() override;

private:
    void setupSignalsAndSlots();

protected:
    void initBundle() override;

private slots:
    void onTextChanged(QString);
    void onBrowse();
};

#endif // APPLICATIONACTION_H
