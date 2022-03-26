#include "catchesdelegate.h"
#include "sessionproxymodel.h"
#include "catchmodel.h"
#include "fishmodel.h"

#include <QtDebug>
#include <QSqlQuery>

CatchesDelegate::CatchesDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

/*
 * Paint a string representing the catches in a cell of a table using a self defined style.
 */
void CatchesDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QMap<int, int> catches;
    QString content("");
    QSqlQuery query;
    QString sQueryCatches = "SELECT * FROM Catch WHERE session_id = %1";
    QString sQuerySpecies = "SELECT * FROM Species WHERE id = %1";
    int sessionId = index.model()->data(index.model()->index(index.row(), SessionProxyModel::SessionId)).toInt();
    query.exec(sQueryCatches.arg(sessionId));
    while ( query.next() ) {
        int speciesId = query.value(CatchModel::CatchSpeciesId).toInt();
        if ( catches.contains(speciesId) ) {
            catches[speciesId] = catches[speciesId] + 1;
        } else {
            catches.insert(speciesId, 1);
        }
    }
    if ( catches.isEmpty() ) {
        content += '-';
    } else {
        QMapIterator<int, int> i(catches);
        while ( i.hasNext() ) {
            i.next();
            query.exec(sQuerySpecies.arg(i.key()));
            QString shortcut = query.next() ? query.value(FishModel::FishShortcut).toString() : "?";
            content += QString::number(i.value());
            content += ' ';
            content += shortcut;
            if ( i.hasNext() ) {
                content += ", ";
            }
        }
    }
    painter->save();
    if ( option.state & QStyle::State_Selected )
        painter->fillRect(option.rect, option.palette.highlight());
    painter->setPen(option.state & QStyle::State_Selected ?
                    option.palette.highlightedText().color() :
                    option.palette.windowText().color());
    painter->drawText(option.rect, content, Qt::AlignTop | Qt::AlignCenter);
    painter->restore();
}
