#ifndef EDITSESSIONDIALOG_H
#define EDITSESSIONDIALOG_H

#include "sessionmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlDatabase>

namespace Ui {
    class EditSessionDialog;
}

class EditSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSessionDialog(QSqlDatabase db, int id, QWidget *parent = 0);
    ~EditSessionDialog();

private slots:
    void onButtonBoxAccepted();

private:
    Ui::EditSessionDialog *ui;
    SessionModel *sessionModel;
    QSqlTableModel *locationModel;
    QDataWidgetMapper *mapper;
};

#endif // EDITSESSIONDIALOG_H
