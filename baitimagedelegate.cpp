/*
 * Class is responsible for drawing strings and images in bait tables.
 */
#include "baitimagedelegate.h"
#include "baitmodel.h"
#include "configuration.h"

#include <QFile>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

/*
 * Constructor
 */
BaitImageDelegate::BaitImageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    prefImageHeight = 64;
    prefImageWidth = 128;
}

/*
 * Draw a string or image in a table cell if drawing (or redrawing) is necessary.
 */
void BaitImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == BaitModel::BaitImage ) {
        QString fName = Configuration::getPathBaitImageFiles() + QDir::separator() + qvariant_cast<QString>(index.data());
        painter->save();
        if ( option.state & QStyle::State_Selected ) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        if ( ! QFile::exists(fName) ) {
            qDebug() << "BaitImageDelegate: no image found for row " << index.row();
            fName = ":/images/nobait.png";
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
QSize BaitImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == BaitModel::BaitImage ) {
        QString fName = qvariant_cast<QString>(index.data());
        return QSize(prefImageWidth, prefImageHeight);
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}
