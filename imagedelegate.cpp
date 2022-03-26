/*
 * Class is responsible for drawing strings and images in any kind of view table.
 */
#include "imagedelegate.h"

#include <QFile>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

/*
 * Constructor
 */
ImageDelegate::ImageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    // Set default values
    preferredSize.setHeight(64);
    preferredSize.setWidth(128);
    imagePath = "";
    alternateImage = ":/noimage.png";
}

/*
 * Draw an image in a table cell if drawing (or redrawing) is necessary.
 */
void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString fName = imagePath + QDir::separator() + qvariant_cast<QString>(index.data());
    painter->save();
    if ( option.state & QStyle::State_Selected ) {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    QImage image(fName);
    if ( image.isNull() ) {
        qDebug() << "ImageDelegate: no image found for row " << index.row();
        fName = alternateImage;
        image.load(fName);
    }

    /*
    if ( ! QFile::exists(fName) ) {
        qDebug() << "ImageDelegate: no image found for row " << index.row();
        fName = alternateImage;
    }
    qDebug() << "ImageDelegate: using image: " << fName;
    QImage image(fName);
*/
    QPixmap pixmap = QPixmap::fromImage(image);
    painter->drawPixmap(option.rect, pixmap);
    painter->restore();
}

/*
 * Return the preferred size of the image which shall be drawn in the image column of the table.
 */
QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return preferredSize;
}

/*
 * Set the preferred size of the image.
 */
void ImageDelegate::setPreferredSize(int width, int height)
{
    preferredSize.setWidth(width);
    preferredSize.setHeight(height);
}

/*
 * Set the image path of the images related to the data model.
 */
void ImageDelegate::setImagePath(QString path)
{
    imagePath = path;
}

/*
 * Set the alternate image which shall be shown if no image is available in the data model.
 */
void ImageDelegate::setAlternateImage(QString name)
{
    alternateImage = name;
}
