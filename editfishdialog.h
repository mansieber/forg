#ifndef EDITFISHDIALOG_H
#define EDITFISHDIALOG_H

#include "fishmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlDatabase>

namespace Ui {
    class EditFishDialog;
}

class EditFishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditFishDialog(QSqlDatabase db, int id, QWidget *parent = 0);
    ~EditFishDialog();

private slots:
    void onButtonBoxAccepted();
    void onButtonSelect();

private:
    Ui::EditFishDialog *ui;
    FishModel *fishModel;
    QDataWidgetMapper *mapper;
};
#endif // EDITFISHDIALOG_H
