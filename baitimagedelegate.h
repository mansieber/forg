#ifndef BAITIMAGEDELEGATE_H
#define BAITIMAGEDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

class BaitImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit BaitImageDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    int prefImageWidth;
    int prefImageHeight;
};

#endif // BAITIMAGEDELEGATE_H
