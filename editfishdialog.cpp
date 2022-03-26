#include "editfishdialog.h"
#include "ui_editfishdialog.h"
#include "viewfishdialog.h"
#include "fishmodel.h"
#include "configuration.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>
#include <QSqlRecord>

EditFishDialog::EditFishDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditFishDialog)
{
    ui->setupUi(this);

    fishModel = new FishModel(this, db);
    fishModel->setTable("Species");
    if ( ! fishModel->select() ) {
        qCritical() << "No connection between FishModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(fishModel);
    mapper->addMapping(ui->editName, FishModel::FishName);
    mapper->addMapping(ui->editCFactor, FishModel::FishCFactor);
    mapper->addMapping(ui->editPath, FishModel::FishImage);
    mapper->addMapping(ui->editShortcut, FishModel::FishShortcut);

    if ( id != -1 ) {           // Edit an exisiting fish
        for ( int row = 0; row < fishModel->rowCount(); ++row ) {
            QSqlRecord record = fishModel->record(row);
            if ( record.value(FishModel::FishId).toInt() == id ) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {                    // Add a new fish
        int row = fishModel->rowCount();
        qDebug() << "EditFishDialog: add new fish, row count =" << row;
        bool result = fishModel->insertRow(row);
        qDebug() << "EditFishDialog: new row added" << ( result ? "successful" : "not successful");
        mapper->setCurrentIndex(row);
    }

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(onButtonSelect()));

    qDebug() << "EditFishDialog: new fish dialog created; id = " << id;
}

/*
 * Destructor deletes the edit fish dialog user interface.
 */
EditFishDialog::~EditFishDialog()
{
    delete ui;
    delete fishModel;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditFishDialog::onButtonBoxAccepted()
{
    bool result = mapper->submit();
    fishModel->updateRow(mapper->currentIndex());       // All views must be updated

    qDebug() << "EditFishDialog: fish edited/added. Submit" << ( result ? "successful" : "not successful");

    this->close();
}

/*
 * Method handles the select Button. A path to a fish image shall be choosen.
 */
void EditFishDialog::onButtonSelect()
{
    QString absolutePathName = QFileDialog::getOpenFileName(this, tr("Select Image"),
                                                    Configuration::getPathFishImageFiles(),
                                                    tr("Image Files (*.png *.jpg)"));
    QDir dir(Configuration::getPathFishImageFiles());
    QString relativePathName = dir.relativeFilePath(absolutePathName);
    ui->editPath->setText(relativePathName);
}

