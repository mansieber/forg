#include "sortedcomboboxdelegate.h"

#include <QtWidgets>

SortedComboBoxDelegate::SortedComboBoxDelegate(QObject *parent)
    : QSqlRelationalDelegate(parent)
{
}

void SortedComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QSqlRelationalDelegate::paint(painter, option, index);
}

QSize SortedComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    return QSqlRelationalDelegate::sizeHint(option, index);
}

bool SortedComboBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index)
{
    return QSqlRelationalDelegate::editorEvent(event, model, option, index);
}

QWidget *SortedComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    return QSqlRelationalDelegate::createEditor(parent, option, index);
}


