#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QList>
#include <QString>

class SessionModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    static QList<QString> weather;
    static QList<QString> winddirections;
    static QList<QString> ratings;
    SessionModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateRow(int row);
};

#endif // SESSIONMODEL_H
