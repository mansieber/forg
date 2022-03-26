#include "datetimedelegate.h"

#include <QtDebug>
#include <QDateTime>

/*
 * Constructor saves the column of the table view which will be
 * get the paint delegation.
 */
DateTimeDelegate::DateTimeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

/*
 * Paint the date and time in a cell of a table using a self defined style.
 */
void DateTimeDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateTime dateTime =
        QDateTime::fromString((index.model()->data(index, Qt::DisplayRole)).toString(),
                              "yyyy-MM-ddThh:mm:ss.zzz");
    QString sDateTime = dateTime.date().toString("dd.MM.yy") + " " + dateTime.time().toString("hh.mm");
    painter->save();
    if ( option.state & QStyle::State_Selected )
        painter->fillRect(option.rect, option.palette.highlight());
    painter->setPen(option.state & QStyle::State_Selected ?
                    option.palette.highlightedText().color() :
                    option.palette.windowText().color());
    painter->drawText(option.rect, sDateTime, Qt::AlignTop | Qt::AlignCenter);
    painter->restore();
}
