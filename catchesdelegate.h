#ifndef CATCHESDELEGATE_H
#define CATCHESDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class CatchesDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CatchesDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CATCHESDELEGATE_H
