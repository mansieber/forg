#include "editbaitdialog.h"
#include "ui_editbaitdialog.h"
#include "viewbaitdialog.h"
#include "baitmodel.h"
#include "configuration.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>
#include <QSqlRecord>

EditBaitDialog::EditBaitDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBaitDialog)
{
    ui->setupUi(this);

    ui->comboColor->addItems(BaitModel::colors);
    ui->comboType->addItems(BaitModel::types);

    baitModel = new BaitModel(this, db);
    baitModel->setTable("Bait");
    if ( ! baitModel->select() ) {
        qCritical() << "No connection between BaitModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(baitModel);
    mapper->addMapping(ui->editName, BaitModel::BaitName);
    mapper->addMapping(ui->editSize, BaitModel::BaitSize);
    mapper->addMapping(ui->comboColor, BaitModel::BaitColor);
    mapper->addMapping(ui->comboType, BaitModel::BaitType);
    mapper->addMapping(ui->editPath, BaitModel::BaitImage);

    if ( id != -1 ) {               // Edit an existing bait
        for ( int row = 0; row < baitModel->rowCount(); ++row ) {
            QSqlRecord record = baitModel->record(row);
            if ( record.value(BaitModel::BaitId).toInt() == id ) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {                        // Add a new bait
        int row = baitModel->rowCount();
        qDebug() << "EditBaitDialog: add new bait, row count =" << row;
        bool result = baitModel->insertRow(row);
        qDebug() << "EditBaitDialog: new row added" << ( result ? "successful" : "not successful");
        mapper->setCurrentIndex(row);
    }

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(onButtonSelect()));

    qDebug() << "EditBaitDialog: new bait dialog created; id = " << id;
}

/*
 * Destructor deletes the edit bait dialog user interface.
 */
EditBaitDialog::~EditBaitDialog()
{
    delete ui;
    delete baitModel;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditBaitDialog::onButtonBoxAccepted()
{
    bool result = mapper->submit();
    baitModel->updateRow(mapper->currentIndex());       // All views must be updated

    qDebug() << "EditBaitDialog: new bait added. Submit" << ( result ? "successful." : "not successful!");

    this->close();
}

/*
 * Method handles the select Button. A path to a bait image shall be choosen.
 */
void EditBaitDialog::onButtonSelect()
{
    QString absolutePathName = QFileDialog::getOpenFileName(this, tr("Select Image"),
                                                    Configuration::getPathBaitImageFiles(),
                                                    tr("Image Files (*.png *.jpg)"));
    QDir dir(Configuration::getPathBaitImageFiles());
    QString relativePathName = dir.relativeFilePath(absolutePathName);
    ui->editPath->setText(relativePathName);
}
