#ifndef FISHIMAGEDELEGATE_H
#define FISHIMAGEDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

class FishImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit FishImageDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    int prefImageWidth;
    int prefImageHeight;
};

#endif // FISHIMAGEDELEGATE_H
