#ifndef COMMANDACTION_H
#define COMMANDACTION_H

#include <QWidget>
#include <QString>

#include "action.h"

namespace Ui {
    class CommandAction;
}

class CommandAction : public Action
{
    Q_OBJECT

private:
    Ui::CommandAction *ui;

    QString m_Command;

public:
    CommandAction(const QString& command, const QString& name = "", QWidget* parent = nullptr);
    CommandAction(const QString& name = "", QWidget* parent = nullptr);

    ~CommandAction();

    inline const QString& getCommand() const { return m_Command; }

    QString getTagName() const override;

    void readProperties(Bundle&) override;
    void writeProperties(Bundle&) override;

    void execute() override;

private:
    void setupSignalsAndSlots();

private slots:
    void onEdit(QString);
};

#endif // COMMANDACTION_H
