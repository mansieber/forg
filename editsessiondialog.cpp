#include "editsessiondialog.h"
#include "ui_editsessiondialog.h"
#include "sessionmodel.h"
#include "sessionproxymodel.h"

#include <QMessageBox>
#include <QtDebug>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>

EditSessionDialog::EditSessionDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSessionDialog)
{
    ui->setupUi(this);

    ui->comboWeather->addItems(SessionModel::weather);

    sessionModel = new SessionModel(this, db);
    sessionModel->setTable("Session");
    sessionModel->setRelation(SessionProxyModel::SessionLocationId, QSqlRelation("Location", "id", "name"));
    QSqlTableModel *locationModel = sessionModel->relationModel(SessionProxyModel::SessionLocationId);
    ui->comboLocation->setModel(locationModel);
    ui->comboLocation->setModelColumn(locationModel->fieldIndex("name"));

    if ( ! sessionModel->select() ) {
        qCritical() << "No connection between SessionModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(sessionModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->editStartTime, SessionProxyModel::SessionStartTime);
    mapper->addMapping(ui->editEndTime, SessionProxyModel::SessionEndTime);
    mapper->addMapping(ui->comboLocation, SessionProxyModel::SessionLocationId);
    mapper->addMapping(ui->comboWeather, SessionProxyModel::SessionWeather);
    mapper->addMapping(ui->editTimeHighTide, SessionProxyModel::SessionHighTime);
    mapper->addMapping(ui->editPressure, SessionProxyModel::SessionAirPressure);
    mapper->addMapping(ui->editPressureChange, SessionProxyModel::SessionPressureChange);
    mapper->addMapping(ui->editAirTemperature, SessionProxyModel::SessionAirTemperature);
    mapper->addMapping(ui->editWaterTemperature, SessionProxyModel::SessionWaterTemperature);
    mapper->addMapping(ui->editWindStrength, SessionProxyModel::SessionWindStrength);
    mapper->addMapping(ui->spinBoxWindDirection, SessionProxyModel::SessionWindDirection);
    mapper->addMapping(ui->spinBoxRating, SessionProxyModel::SessionRating);
    mapper->addMapping(ui->editDescription, SessionProxyModel::SessionDescription);

    if ( id != -1 ) {           // Edit an exisiting session
        for ( int row = 0; row < sessionModel->rowCount(); ++row ) {
            QSqlRecord record = sessionModel->record(row);
            if ( record.value(SessionProxyModel::SessionId).toInt() == id ) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {                    // Add a new session
        int row = sessionModel->rowCount();
        qDebug() << "EditSessionDialog: add new session, row count =" << row;
        bool result = sessionModel->insertRow(row);
        qDebug() << "EditSessionDialog: new row added" << ( result ? "successful" : "not successful");
        mapper->setCurrentIndex(row);
    }

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));

    qDebug() << "EditSessionDialog: new session dialog created; id = " << id;
}

/*
 * Destructor deletes the edit session dialog user interface.
 */
EditSessionDialog::~EditSessionDialog()
{
    delete ui;
    delete sessionModel;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditSessionDialog::onButtonBoxAccepted()
{
    bool result = mapper->submit();
    sessionModel->updateRow(mapper->currentIndex());       // All views must be updated

    qDebug() << "EditSessionDialog: session edited/added. Submit" << ( result ? "successful" : "not successful");

    this->close();
}

