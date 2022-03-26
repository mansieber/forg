#include "fishmodel.h"

#include <QtDebug>

FishModel::FishModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

QVariant FishModel::data(const QModelIndex &index, int role) const
{
    if ( ! index.isValid() ) {
        qWarning("FishModel: invalid index!");
        return QVariant();
    } else {
        return QSqlTableModel::data(index, role);
    }
}

/*
 * Update a row in the table view (e.g. because data changed).
 */
void FishModel::updateRow(int row)
{
    qDebug() << "FishModel: Update view; row : " << row;
}
