#ifndef SORTEDCOMBOBOXDELEGATE_H
#define SORTEDCOMBOBOXDELEGATE_H

#include <QSqlRelationalDelegate>

/*
 * Class represents a delegate which provides a sorted combo box for foreign keys.
 */
class SortedComboBoxDelegate : public QSqlRelationalDelegate
{
public:
    SortedComboBoxDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) Q_DECL_OVERRIDE;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // SORTEDCOMBOBOXDELEGATE_H
