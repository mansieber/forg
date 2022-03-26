#ifndef CATCHMODEL_H
#define CATCHMODEL_H

#include <QSqlRelationalTableModel>
#include <QSqlDatabase>

class CatchModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    enum {
        CatchId = 0,
        CatchTime = 1,
        CatchSpeciesId = 2,
        CatchLength = 3,
        CatchWeight = 4,
        CatchImage = 5,
        CatchSessionId = 6,
        CatchBaitId = 7,
        CatchLatitude = 8,
        CatchLongitude = 9,
        CatchDescription = 10
    };
    CatchModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateRow(int row);
};

#endif // CATCHMODEL_H
