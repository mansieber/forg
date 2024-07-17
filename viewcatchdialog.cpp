#include "viewcatchdialog.h"
#include "ui_viewcatchdialog.h"
#include "catchmodel.h"
#include "fishmodel.h"
#include "sessionmodel.h"
#include "baitmodel.h"
#include "imagedelegate.h"
#include "datetimedelegate.h"
#include "configuration.h"

#include <QtDebug>
#include <QModelIndex>
#include <QSqlRecord>

ViewCatchDialog::ViewCatchDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewCatchDialog)
{
    this->db = db;

    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addCatch()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editCatch()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCatch()));
    connect(ui->catchTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCatch(QModelIndex)));

    catchModel = new CatchModel(this, db);
    catchModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    catchModel->setEditStrategy(QSqlTableModel::OnRowChange);
    catchModel->setTable(CATCH_TABLE);
    catchModel->setRelation(CatchModel::CatchSpeciesId,
                            QSqlRelation(SPECIES_TABLE, SPECIES_COLUMN_ID, SPECIES_COLUMN_NAME));
    catchModel->setRelation(CatchModel::CatchSessionId,
                            QSqlRelation(SESSION_TABLE, SESSION_COLUMN_ID, SESSION_COLUMN_STARTTIME));
    catchModel->setRelation(CatchModel::CatchBaitId,
                            QSqlRelation(BAIT_TABLE, BAIT_COLUMN_ID, BAIT_COLUMN_NAME));
    catchModel->setEditStrategy(QSqlRelationalTableModel::OnRowChange);

    catchModel->setHeaderData(CatchModel::CatchTime, Qt::Horizontal, tr("Time"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchSpeciesId, Qt::Horizontal, tr("Species"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchLength , Qt::Horizontal, tr("Length [cm]"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchWeight , Qt::Horizontal, tr("Weight [g]"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchImage, Qt::Horizontal, tr("Image"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchSessionId , Qt::Horizontal, tr("Session"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchBaitId , Qt::Horizontal, tr("Bait"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchLatitude , Qt::Horizontal, tr("Latitude"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchLongitude , Qt::Horizontal, tr("Longitude"), Qt::DisplayRole);
    catchModel->setHeaderData(CatchModel::CatchDescription , Qt::Horizontal, tr("Description"), Qt::DisplayRole);
    if ( ! catchModel->select() ) {
        qCritical() << "No connection between CatchModel and database!";
    }
    ui->catchTable->setModel(catchModel);
    ui->catchTable->setItemDelegateForColumn(CatchModel::CatchImage, imageDelegate = new ImageDelegate(this));
    ui->catchTable->setItemDelegateForColumn(CatchModel::CatchTime, new DateTimeDelegate);
    ui->catchTable->setItemDelegateForColumn(CatchModel::CatchSessionId, new DateTimeDelegate);

    imageDelegate->setAlternateImage(":/images/nofish.png");
    imageDelegate->setImagePath(Configuration::getPathCatchImageFiles());
    imageDelegate->setPreferredSize(128, 64);
    ui->catchTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->catchTable->setColumnHidden(CatchModel::CatchId, true);
    ui->catchTable->sortByColumn(CatchModel::CatchTime, Qt::AscendingOrder);
    ui->catchTable->resizeColumnsToContents();
    ui->catchTable->resizeRowsToContents();
}

/*
 * Destructor
 */
ViewCatchDialog::~ViewCatchDialog()
{
    delete imageDelegate;
    delete ui;
}

/*
 * Create new edit catch dialog for adding a catch to the catch table and show it.
 */
void ViewCatchDialog::addCatch()
{
    qDebug() << "ViewCatchDialog: add catch called.";
    EditCatchDialog editCatchDialog(db, -1, this);
    editCatchDialog.exec();
    catchModel->select();
    ui->catchTable->resizeRowsToContents();
}

/*
 * Create new edit catch dialog for editing catch information and show it.
 * @param index references the selected index of the catch model
 */
void ViewCatchDialog::editCatch(QModelIndex index)
{
    qDebug() << "ViewCatchDialog: edit catch called.";
    if ( index.isValid() ) {
        QSqlRecord record = catchModel->record(index.row());
        int catchId = record.value(CatchModel::CatchId).toInt();
        EditCatchDialog editCatchDialog(db, catchId, this);
        editCatchDialog.exec();
        catchModel->select();
        ui->catchTable->resizeRowsToContents();
    } else
        qDebug() << "ViewCatchDialog: invalid index!";
}

/*
 * Delete selected catch.
 */
void ViewCatchDialog::deleteCatch()
{
    qDebug() << "ViewCatchDialog: delete catch called.";
    QModelIndex index = ui->catchTable->currentIndex();      // get the selected index
    if ( index.isValid() ) {
        catchModel->removeRow(index.row());
        if ( ! catchModel->submit() ) {
            qCritical() << "ViewCatchDialog: submit to database failed!";
        } else {
            if ( ! catchModel->select() ) {
                qCritical() << "ViewCatchDialog: select failed!";
            }
        }
    } else
        qDebug() << "ViewCatchDialog: invalid index!";
}

/*
 * Create new catch dialog for editing catch information and show it.
 */
void ViewCatchDialog::editCatch()
{
    qDebug() << "ViewCatchDialog: edit catch called.";
    QModelIndex index = ui->catchTable->currentIndex();      // get the selected index
    if ( index.isValid() )
        editCatch(index);
    else
        qDebug() << "ViewCatchDialog: no item selected!";
}
