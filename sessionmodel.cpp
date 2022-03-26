#include "sessionmodel.h"

#include <QtDebug>

QList<QString> SessionModel::weather = QList<QString>() << QString(tr("clear")) << QString(tr("fair"))
                                                        << QString(tr("cloudy")) << QString(tr("rainy"))
                                                        << QString(tr("rain"));
QList<QString> SessionModel::winddirections = QList<QString>() << QString(tr("N")) << QString(tr("NNE"))
                                                               << QString(tr("NE")) << QString(tr("ENE"))
                                                               << QString(tr("E")) << QString(tr("ESE"))
                                                               << QString(tr("SE")) << QString(tr("SSE"))
                                                               << QString(tr("S")) << QString(tr("SSW"))
                                                               << QString(tr("SW")) << QString(tr("WSW"))
                                                               << QString(tr("W")) << QString(tr("WNW"))
                                                               << QString(tr("NW")) << QString(tr("NNW"));

SessionModel::SessionModel(QObject *parent, QSqlDatabase db)
    : QSqlRelationalTableModel(parent, db)
{
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if ( ! index.isValid() ) {
        qWarning("SessionModel: invalid index!");
        return QVariant();
    } else {
        return QSqlRelationalTableModel::data(index, role);
    }
}

/*
 * Update a row in the table view (e.g. because data changed).
 */
void SessionModel::updateRow(int row)
{
    qDebug() << "SessionModel: Update view; row : " << row;
}
