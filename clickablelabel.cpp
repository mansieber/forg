#include "clickablelabel.h"

#include <QFile>
#include <QtDebug>

/*
 * Constructor of class. Path name of image is passed to constructor.
 */
ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
}

void ClickableLabel::paint(QPainter *painter, const QRect &rect, const QPalette &palette) const
{
    painter->save();
    QImage image( ( ! QFile::exists(pathName) ) ? ":/images/nobait.png" : pathName );
    QPixmap pixmap = QPixmap::fromImage(image);
    painter->drawPixmap(rect, pixmap);
    painter->restore();
    qDebug() << "ClickableLabel: image drawn via paint()";
}

/*
 * Method allows to set the pathname of the image file.
 */
void ClickableLabel::setPathName(QString pName)
{
    pathName = pName;
}

/*
 * Method is called if instance of class is clicked with the mouse.
 */
void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

