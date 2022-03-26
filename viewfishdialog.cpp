#include "viewfishdialog.h"
#include "ui_viewfishdialog.h"
#include "fishmodel.h"
#include "imagedelegate.h"
#include "configuration.h"

#include <QtDebug>
#include <QModelIndex>
#include <QSqlRecord>

ViewFishDialog::ViewFishDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFishDialog)
{
    this->db = db;

    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addFish()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editFish()));
    connect(ui->fishTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editFish(QModelIndex)));

    fishModel = new FishModel(this, db);
    fishModel->setEditStrategy(QSqlTableModel::OnRowChange);
    fishModel->setTable("Species");
    fishModel->setHeaderData(FishModel::FishName, Qt::Horizontal, tr("Name"));
    fishModel->setHeaderData(FishModel::FishImage, Qt::Horizontal, tr("Image"));
    fishModel->setHeaderData(FishModel::FishCFactor, Qt::Horizontal, tr("Corp. factor"));
    fishModel->setHeaderData(FishModel::FishShortcut, Qt::Horizontal, tr("Shortcut"));
    if ( ! fishModel->select() ) {
        qCritical() << "No connection between FishModel and database!";
    }
    ui->fishTable->setModel(fishModel);
    ui->fishTable->setItemDelegateForColumn(FishModel::FishImage, imageDelegate = new ImageDelegate);
    imageDelegate->setAlternateImage(":/images/nofish.png");
    imageDelegate->setImagePath(Configuration::getPathFishImageFiles());
    imageDelegate->setPreferredSize(128, 64);
    ui->fishTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fishTable->setColumnHidden(FishModel::FishId, true);
    ui->fishTable->sortByColumn(FishModel::FishName, Qt::AscendingOrder);
    ui->fishTable->resizeColumnsToContents();
    ui->fishTable->resizeRowsToContents();
}

/*
 * Destructor
 */
ViewFishDialog::~ViewFishDialog()
{
    delete imageDelegate;
    delete ui;
}

/*
 * Create new edit fish dialog for adding a fish to the fish table and show it.
 */
void ViewFishDialog::addFish()
{
    qDebug() << "ViewFishDialog: add fish called.";
    EditFishDialog editFishDialog(db, -1, this);
    editFishDialog.exec();
    fishModel->select();
    ui->fishTable->resizeRowsToContents();
}

/*
 * Create new edit fish dialog for editing fish information and show it.
 * @param index references the selected index of the fish model
 */
void ViewFishDialog::editFish(QModelIndex index)
{
    qDebug() << "ViewFishDialog: edit fish called.";
    if ( index.isValid() ) {
        QSqlRecord record = fishModel->record(index.row());
        int fishId = record.value(FishModel::FishId).toInt();
        EditFishDialog editFishDialog(db, fishId, this);
        editFishDialog.exec();
        fishModel->select();
        ui->fishTable->resizeRowsToContents();
    } else
        qDebug() << "ViewFishDialog: invalid index!";
}

/*
 * Create new fish dialog for editing fish information and show it.
 */
void ViewFishDialog::editFish()
{
    qDebug() << "ViewFishDialog: edit fish called.";
    QModelIndex index = ui->fishTable->currentIndex();      // get the selected index
    if ( index.isValid() )
        editFish(index);
    else
        qDebug() << "ViewFishDialog: no item selected!";
}

