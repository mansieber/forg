#ifndef VIEWLOCATIONDIALOG_H
#define VIEWLOCATIONDIALOG_H

#include "editlocationdialog.h"
#include "locationmodel.h"
#include "imagedelegate.h"

#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
    class ViewLocationDialog;
}

class ViewLocationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewLocationDialog(QSqlDatabase db, QWidget *parent = 0);
    ~ViewLocationDialog();

private slots:
    void addLocation();
    void editLocation();
    void editLocation(QModelIndex index);

private:
    Ui::ViewLocationDialog *ui;
    ImageDelegate *imageDelegate;
    QSqlDatabase db;
    LocationModel *locationModel;
    EditLocationDialog *editLocationDialog;
};

#endif // VIEWLOCATIONDIALOG_H
