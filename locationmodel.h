#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

#define LOCATION_TABLE "Location"
#define LOCATION_COLUMN_NAME "name"
#define LOCATION_COLUMN_ID "id"

class LocationModel : public QSqlTableModel
{
    Q_OBJECT

public:
    enum {
        LocationId = 0,
        LocationName = 1,
        LocationLatitude = 2,       // Not longer in use
        LocationLongitude = 3,      // Not longer in use
        LocationImage = 4,
        LocationDescription = 5,
        LocationLatitudeNW = 6,
        LocationLongitudeNW = 7,
        LocationLatitudeSE = 8,
        LocationLongitudeSE = 9
    };
    LocationModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateRow(int row);
};

#endif // LOCATIONMODEL_H
