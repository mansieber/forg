#ifndef DATETIMEDELEGATE_H
#define DATETIMEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class DateTimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DateTimeDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DATETIMEDELEGATE_H
