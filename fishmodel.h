#ifndef FISHMODEL_H
#define FISHMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QList>
#include <QString>

class FishModel : public QSqlTableModel
{
    Q_OBJECT

public:
    enum {
        FishId = 0,
        FishName = 1,
        FishImage = 2,
        FishCFactor = 3,
        FishShortcut = 4
    };
    FishModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateRow(int row);
};

#endif // FISHMODEL_H
