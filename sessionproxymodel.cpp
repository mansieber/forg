#include "sessionproxymodel.h"

SessionProxyModel::SessionProxyModel(QObject *parent) :
    QIdentityProxyModel(parent)
{
}

QVariant SessionProxyModel::data(const QModelIndex &index, int role) const
{
    if ( role == Qt::DisplayRole ) {
        if ( index.column() == SessionCatches ) {
            return "Catches";
        } else {
//            return QVariant();
            return QIdentityProxyModel::data(index, role);
        }
    } else
        return QIdentityProxyModel::data(index, role);
}


