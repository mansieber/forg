#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QPainter>

/*
 * Class is like a QLabel but can be clicked like a Button.
 */
class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = 0);
    void setPathName(QString pName);
    void paint(QPainter *painter, const QRect &rect, const QPalette &palette) const;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QString pathName;
};

// Q_DECLARE_METATYPE(ClickableLabel)

#endif // CLICKABLELABEL_H
