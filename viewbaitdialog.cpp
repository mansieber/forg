#include "viewbaitdialog.h"
#include "ui_viewbaitdialog.h"
#include "baitmodel.h"
#include "imagedelegate.h"
#include "configuration.h"

#include <QtDebug>
#include <QModelIndex>
#include <QSqlRecord>

ViewBaitDialog::ViewBaitDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewBaitDialog)
{
    this->db = db;

    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addBait()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editBait()));
    connect(ui->baitTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editBait(QModelIndex)));

    baitModel = new BaitModel(this, db);
    baitModel->setEditStrategy(QSqlTableModel::OnRowChange);
    baitModel->setTable(BAIT_TABLE);
    baitModel->setHeaderData(BaitModel::BaitName, Qt::Horizontal, tr("Name"));
    baitModel->setHeaderData(BaitModel::BaitSize, Qt::Horizontal, tr("Size"));
    baitModel->setHeaderData(BaitModel::BaitColor, Qt::Horizontal, tr("Color"));
    baitModel->setHeaderData(BaitModel::BaitType, Qt::Horizontal, tr("Type"));
    baitModel->setHeaderData(BaitModel::BaitImage, Qt::Horizontal, tr("Image"));
    if ( ! baitModel->select() ) {
        qCritical() << "No connection between BaitModel and database!";
    }
    ui->baitTable->setModel(baitModel);
    ui->baitTable->setItemDelegateForColumn(BaitModel::BaitImage, imageDelegate = new ImageDelegate);
    imageDelegate->setAlternateImage(":/images/nobait.png");
    imageDelegate->setImagePath(Configuration::getPathBaitImageFiles());
    imageDelegate->setPreferredSize(128, 64);
    ui->baitTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->baitTable->setColumnHidden(BaitModel::BaitId, true);
    ui->baitTable->sortByColumn(BaitModel::BaitName, Qt::AscendingOrder);
    ui->baitTable->resizeColumnsToContents();
    ui->baitTable->resizeRowsToContents();

}

/*
 * Destructor
 */
ViewBaitDialog::~ViewBaitDialog()
{
    delete imageDelegate;
    delete ui;
}

/*
 * Create new edit bait dialog for adding a bait to the bait table and show it.
 */
void ViewBaitDialog::addBait()
{
    qDebug() << "ViewBaitDialog: add bait called.";
    EditBaitDialog editBaitDialog(db, -1, this);
    editBaitDialog.exec();
    baitModel->select();
}

/*
 * Create new edit bait dialog for editing bait information and show it.
 * @param index references the selected index of the bait model
 */
void ViewBaitDialog::editBait(QModelIndex index)
{
    qDebug() << "ViewBaitDialog: edit bait called.";
    if ( index.isValid() ) {
        QSqlRecord record = baitModel->record(index.row());
        int baitId = record.value(BaitModel::BaitId).toInt();
        EditBaitDialog editBaitDialog(db, baitId, this);
        editBaitDialog.exec();
        baitModel->select();
    } else
        qDebug() << "ViewBaitDialog: invalid index!";
}

/*
 * Create new edit bait dialog for editing bait information and show it.
 */
void ViewBaitDialog::editBait()
{
    qDebug() << "ViewBaitDialog: edit bait called.";
    QModelIndex index = ui->baitTable->currentIndex();      // get the selected index
    if ( index.isValid() )
        editBait(index);
    else
        qDebug() << "ViewBaitDialog: no item selected!";
}

