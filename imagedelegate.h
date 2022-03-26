#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QSize>
#include <QString>

class ImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ImageDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setPreferredSize(int width, int height);
    void setImagePath(QString path);
    void setAlternateImage(QString name);

protected:
    QSize preferredSize;
    QString imagePath;
    QString alternateImage;
};

#endif // IMAGEDELEGATE_H
