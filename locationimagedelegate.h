#ifndef LOCATIONIMAGEDELEGATE_H
#define LOCATIONIMAGEDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

class LocationImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LocationImageDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    int prefImageWidth;
    int prefImageHeight;
};

#endif // LOCATIONIMAGEDELEGATE_H
