#include "catchmodel.h"

#include <QtDebug>

CatchModel::CatchModel(QObject *parent, QSqlDatabase db)
    : QSqlRelationalTableModel(parent, db)
{
}

QVariant CatchModel::data(const QModelIndex &index, int role) const
{
    if ( ! index.isValid() ) {
        qWarning("CatchModel: invalid index!");
        return QVariant();
    } else {
        return QSqlRelationalTableModel::data(index, role);
    }
}

/*
 * Update a row in the table view (e.g. because data changed).
 */
void CatchModel::updateRow(int row)
{
    qDebug() << "CatchModel: Update view; row : " << row;
}


