#include "editlocationdialog.h"
#include "ui_editlocationdialog.h"
#include "viewlocationdialog.h"
#include "locationmodel.h"
#include "configuration.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>
#include <QSqlRecord>

EditLocationDialog::EditLocationDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLocationDialog)
{
    ui->setupUi(this);

    locationModel = new LocationModel(this, db);
    locationModel->setTable("Location");
    if ( ! locationModel->select() ) {
        qCritical() << "No connection between LocationModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(locationModel);
    mapper->addMapping(ui->editName, LocationModel::LocationName);
    mapper->addMapping(ui->editLatitudeNW, LocationModel::LocationLatitudeNW);
    mapper->addMapping(ui->editLongitudeNW, LocationModel::LocationLongitudeNW);
    mapper->addMapping(ui->editLatitudeSE, LocationModel::LocationLatitudeSE);
    mapper->addMapping(ui->editLongitudeSE, LocationModel::LocationLongitudeSE);
    mapper->addMapping(ui->editPath, LocationModel::LocationImage);
    mapper->addMapping(ui->editDescription, LocationModel::LocationDescription);

    if ( id != -1 ) {           // Edit an exisiting location
        for ( int row = 0; row < locationModel->rowCount(); ++row ) {
            QSqlRecord record = locationModel->record(row);
            if ( record.value(LocationModel::LocationId).toInt() == id ) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {                    // Add a new location
        int row = locationModel->rowCount();
        qDebug() << "EditLocationDialog: add new location, row count =" << row;
        bool result = locationModel->insertRow(row);
        qDebug() << "EditLocationDialog: new row added" << ( result ? "successful" : "not successful");
        mapper->setCurrentIndex(row);
    }

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(onButtonSelect()));

    qDebug() << "EditLocationDialog: new location dialog created; id = " << id;
}

/*
 * Destructor deletes the edit location dialog user interface.
 */
EditLocationDialog::~EditLocationDialog()
{
    delete ui;
    delete locationModel;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditLocationDialog::onButtonBoxAccepted()
{
    bool result = mapper->submit();
    locationModel->updateRow(mapper->currentIndex());       // All views must be updated

    qDebug() << "EditLocationDialog: location edited/added. Submit" << ( result ? "successful" : "not successful");

    this->close();
}

/*
 * Method handles the select Button. A path to a location image shall be choosen.
 */
void EditLocationDialog::onButtonSelect()
{
    QString absolutePathName = QFileDialog::getOpenFileName(this, tr("Select Image"),
                                                    Configuration::getPathLocationImageFiles(),
                                                    tr("Image Files (*.png *.jpg)"));
    QDir dir(Configuration::getPathLocationImageFiles());
    QString relativePathName = dir.relativeFilePath(absolutePathName);
    ui->editPath->setText(relativePathName);
}

