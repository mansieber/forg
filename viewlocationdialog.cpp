#include "viewlocationdialog.h"
#include "ui_viewlocationdialog.h"
#include "locationmodel.h"
#include "imagedelegate.h"
#include "configuration.h"

#include <QtDebug>
#include <QModelIndex>
#include <QSqlRecord>

ViewLocationDialog::ViewLocationDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewLocationDialog)
{
    this->db = db;

    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addLocation()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editLocation()));
    connect(ui->locationTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editLocation(QModelIndex)));

    locationModel = new LocationModel(this, db);
    locationModel->setEditStrategy(QSqlTableModel::OnRowChange);
    locationModel->setTable(LOCATION_TABLE);
    locationModel->setHeaderData(LocationModel::LocationName, Qt::Horizontal, tr("Name"));
    locationModel->setHeaderData(LocationModel::LocationLatitudeNW, Qt::Horizontal, tr("Latitude NW"));
    locationModel->setHeaderData(LocationModel::LocationLongitudeNW, Qt::Horizontal, tr("Longitude NW"));
    locationModel->setHeaderData(LocationModel::LocationLatitudeSE, Qt::Horizontal, tr("Latitude SE"));
    locationModel->setHeaderData(LocationModel::LocationLongitudeSE, Qt::Horizontal, tr("Longitude SE"));
    locationModel->setHeaderData(LocationModel::LocationImage, Qt::Horizontal, tr("Image"));
    locationModel->setHeaderData(LocationModel::LocationDescription, Qt::Horizontal, tr("Description"));
    if ( ! locationModel->select() ) {
        qCritical() << "No connection between LocationModel and database!";
    }
    ui->locationTable->setModel(locationModel);
    ui->locationTable->setItemDelegateForColumn(LocationModel::LocationImage, imageDelegate = new ImageDelegate(this));
    imageDelegate->setAlternateImage(":/images/nolocation.png");
    imageDelegate->setImagePath(Configuration::getPathLocationImageFiles());
    imageDelegate->setPreferredSize(200, 100);
    ui->locationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->locationTable->setColumnHidden(LocationModel::LocationId, true);
    ui->locationTable->setColumnHidden(LocationModel::LocationLatitude, true);
    ui->locationTable->setColumnHidden(LocationModel::LocationLongitude, true);
    ui->locationTable->sortByColumn(LocationModel::LocationName, Qt::AscendingOrder);
    ui->locationTable->resizeColumnsToContents();
    ui->locationTable->resizeRowsToContents();
    ui->locationTable->setColumnWidth(LocationModel::LocationDescription, 320);
}

/*
 * Destructor
 */
ViewLocationDialog::~ViewLocationDialog()
{
    delete imageDelegate;
    delete ui;
}

/*
 * Create new edit location dialog for adding a location to the location table and show it.
 */
void ViewLocationDialog::addLocation()
{
    qDebug() << "ViewLocationDialog: add location called.";
    EditLocationDialog editLocationDialog(db, -1, this);
    editLocationDialog.exec();
    locationModel->select();
    ui->locationTable->resizeRowsToContents();
}

/*
 * Create new edit location dialog for editing location information and show it.
 * @param index references the selected index of the location model
 */
void ViewLocationDialog::editLocation(QModelIndex index)
{
    qDebug() << "ViewLocationDialog: edit location called.";
    if ( index.isValid() ) {
        QSqlRecord record = locationModel->record(index.row());
        int locationId = record.value(LocationModel::LocationId).toInt();
        EditLocationDialog editLocationDialog(db, locationId, this);
        editLocationDialog.exec();
        locationModel->select();
        ui->locationTable->resizeRowsToContents();
    } else
        qDebug() << "ViewLocationDialog: invalid index!";
}

/*
 * Create new location dialog for editing location information and show it.
 */
void ViewLocationDialog::editLocation()
{
    qDebug() << "ViewLocationDialog: edit location called.";
    QModelIndex index = ui->locationTable->currentIndex();      // get the selected index
    if ( index.isValid() )
        editLocation(index);
    else
        qDebug() << "ViewLocationDialog: no item selected!";
}

