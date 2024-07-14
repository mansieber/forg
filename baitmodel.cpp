#include "baitmodel.h"

#include <QtDebug>

const QList<QString> BaitModel::colors = QList<QString>() << QString(tr("blue")) << QString(tr("red"))
                                                        << QString(tr("green")) << QString(tr("yellow"))
                                                        << QString(tr("brown")) << QString(tr("black"))
                                                        << QString(tr("white")) << QString(tr("grey"))
                                                        <<QString(tr("gold")) << QString(tr("silver"))
                                                       << QString(tr("transparent"));
const QList<QString> BaitModel::types = QList<QString>() << QString(tr("softbait")) << QString(tr("hardbait"))
                                                       << QString(tr("fly")) << QString(tr("natural"));

BaitModel::BaitModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

QVariant BaitModel::data(const QModelIndex &index, int role) const
{
    if ( ! index.isValid() ) {
        qWarning("BaitModel: invalid index!");
        return QVariant();
    } else if ( ( index.column() == BaitModel::BaitSize || index.column() == BaitModel::BaitColor ||
                  index.column() == BaitModel::BaitType ) && ( role == Qt::TextAlignmentRole ) ) {
            return int(Qt::AlignCenter);
    } else {
        return QSqlTableModel::data(index, role);
    }
}

/*
 * Update a row in the table view (e.g. because data changed).
 */
void BaitModel::updateRow(int row)
{
    qDebug() << "BaitModel: Update view; row : " << row;
}
