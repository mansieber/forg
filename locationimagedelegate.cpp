/*
 * Class is responsible for drawing strings and images in location tables.
 */
#include "locationimagedelegate.h"
#include "locationmodel.h"
#include "configuration.h"

#include <QFile>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

/*
 * Constructor
 */
LocationImageDelegate::LocationImageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    prefImageHeight =120;
    prefImageWidth = 200;
}

/*
 * Draw a string or image in a table cell if drawing (or redrawing) is necessary.
 */
void LocationImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == LocationModel::LocationImage ) {
        QString fName = Configuration::getPathLocationImageFiles() + QDir::separator() + qvariant_cast<QString>(index.data());
        painter->save();
        if ( option.state & QStyle::State_Selected ) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        if ( ! QFile::exists(fName) ) {
            qDebug() << "LocationImageDelegate: no image found for row " << index.row();
            fName = ":/images/nolocation.png";
        }
        QImage image(fName);
        QPixmap pixmap = QPixmap::fromImage(image);
        painter->drawPixmap(option.rect, pixmap);
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

/*
 * Return the preferred size of the image which shall be drawn in the image column of the table.
 */
QSize LocationImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == LocationModel::LocationImage ) {
        QString fName = qvariant_cast<QString>(index.data());
        return QSize(prefImageWidth, prefImageHeight);
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}
