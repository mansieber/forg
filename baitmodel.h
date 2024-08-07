#ifndef BAITMODEL_H
#define BAITMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QList>
#include <QString>

#define BAIT_TABLE "Bait"
#define BAIT_COLUMN_NAME "name"
#define BAIT_COLUMN_ID "id"

class BaitModel : public QSqlTableModel
{
    Q_OBJECT

public:
    enum {
        BaitId = 0,
        BaitName = 1,
        BaitSize = 2,
        BaitColor = 3,
        BaitType = 4,
        BaitImage = 5
    };
    static const QList<QString> colors;
    static const QList<QString> types;
    BaitModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateRow(int row);
};

#endif // BAITMODEL_H
