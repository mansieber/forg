#include "locationmodel.h"

#include <QtDebug>

LocationModel::LocationModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if ( ! index.isValid() ) {
        qWarning("LocationModel: invalid index!");
        return QVariant();
    } else {
        return QSqlTableModel::data(index, role);
    }
}

/*
 * Update a row in the table view (e.g. because data changed).
 */
void LocationModel::updateRow(int row)
{
    qDebug() << "LocationModel: Update view; row : " << row;
}
