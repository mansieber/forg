#ifndef VIEWFISHDIALOG_H
#define VIEWFISHDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

#include "editfishdialog.h"
#include "fishmodel.h"
#include "imagedelegate.h"

namespace Ui {
    class ViewFishDialog;
}

class ViewFishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFishDialog(QSqlDatabase db, QWidget *parent = 0);
    ~ViewFishDialog();

private slots:
    void addFish();
    void editFish();
    void editFish(QModelIndex index);

private:
    Ui::ViewFishDialog *ui;
    ImageDelegate *imageDelegate;
    QSqlDatabase db;
    FishModel *fishModel;
    EditFishDialog *editFishDialog;
};

#endif // VIEWFISHDIALOG_H
