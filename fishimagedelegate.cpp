/*
 * Class is responsible for drawing strings and images in fish tables.
 */
#include "fishimagedelegate.h"
#include "fishmodel.h"
#include "configuration.h"

#include <QFile>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

/*
 * Constructor
 */
FishImageDelegate::FishImageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    prefImageHeight = 64;
    prefImageWidth = 128;
}

/*
 * Draw a string or image in a table cell if drawing (or redrawing) is necessary.
 */
void FishImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == FishModel::FishImage ) {
        QString fName = Configuration::getPathFishImageFiles() + QDir::separator() + qvariant_cast<QString>(index.data());
        painter->save();
        if ( option.state & QStyle::State_Selected ) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        if ( ! QFile::exists(fName) ) {
            qDebug() << "FishImageDelegate: no image found for row " << index.row();
            fName = ":/images/nofish.png";
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
QSize FishImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == FishModel::FishImage ) {
        QString fName = qvariant_cast<QString>(index.data());
        return QSize(prefImageWidth, prefImageHeight);
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}
