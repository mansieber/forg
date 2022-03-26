#include "selectionwatersdialog.h"
#include "ui_selectionwatersdialog.h"

#include "sessionproxymodel.h"
#include "locationmodel.h"
#include "listwidgetiditem.h"

#include <QtDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDateTime>

SelectionWatersDialog::SelectionWatersDialog(QSqlDatabase db, int *year, QList<int> *ids, bool singleSelection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectionWatersDialog)
{
    this->db = db;

    ui->setupUi(this);

    QSqlQuery query;
    pYear = year;
    pLocationIds = ids;


    // Fill the year selection combo box
    query.exec("SELECT * FROM Session");
    QList<int> years;
    while ( query.next() ) {
        QDateTime timeStart = query.value(SessionProxyModel::SessionStartTime).toDateTime();
        int year = timeStart.date().year();
        if ( ! years.contains(year) ) {
            years.append(year);
        }
    }
    if ( ! years.isEmpty() ) {
        std::sort(years.begin(), years.end());
        ui->comboSelectYear->addItem(tr("all"));
        for ( int i = years.count() - 1; i >= 0; i-- ) {
            QString strYear;
            strYear.setNum(years[i]);
            ui->comboSelectYear->addItem(strYear);
        }
    }

    // Fill the waters selection list widget
    QList<ListWidgetIdItem *> waters;
    query.exec("SELECT * FROM Location");
    while ( query.next() ) {
        int locationId = query.value(LocationModel::LocationId).toInt();
        QString locationName = query.value(LocationModel::LocationName).toString();
        new ListWidgetIdItem(locationId, locationName, ui->listWaters);
    }
    if ( singleSelection ) {
        ui->listWaters->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->dialogButtonBox, SIGNAL(rejected()), this, SLOT(onButtonBoxRejected()));
}

SelectionWatersDialog::~SelectionWatersDialog()
{
    delete ui;
}

/*
 * Ok button pressed.
 */
void SelectionWatersDialog::onButtonBoxAccepted()
{
    *pYear = (ui->comboSelectYear->currentText()).toInt();
    QList<QListWidgetItem *> selectedItems = ui->listWaters->selectedItems();
    for ( int i = 0; i < selectedItems.size(); i++ ) {
        int locationId = static_cast<ListWidgetIdItem *> (selectedItems.at(i))->id();
        pLocationIds->append(locationId);
    }
    this->close();
}

/*
 * Cancel button pressed. Return year -1 to indicate cancelation.
 */
void SelectionWatersDialog::onButtonBoxRejected()
{
    qDebug() << "Cancel pressed";
    *pYear = -1;
    this->close();
}
