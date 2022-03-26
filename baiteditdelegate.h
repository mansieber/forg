#ifndef BAITEDITDELEGATE_H
#define BAITEDITDELEGATE_H

#include "editbaitdialog.h"

#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include <QPainter>

class BaitEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit BaitEditDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

protected:
    int prefImageWidth;
    int prefImageHeight;

private:
    EditBaitDialog *theDialog;
};

#endif // BAITEDITDELEGATE_H
