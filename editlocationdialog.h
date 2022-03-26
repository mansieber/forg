#ifndef EDITLOCATIONDIALOG_H
#define EDITLOCATIONDIALOG_H

#include "locationmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlDatabase>

namespace Ui {
    class EditLocationDialog;
}

class EditLocationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLocationDialog(QSqlDatabase db, int id, QWidget *parent = 0);
    ~EditLocationDialog();

private slots:
    void onButtonBoxAccepted();
    void onButtonSelect();

private:
    Ui::EditLocationDialog *ui;
    LocationModel *locationModel;
    QDataWidgetMapper *mapper;
};

#endif // EDITLOCATIONDIALOG_H
