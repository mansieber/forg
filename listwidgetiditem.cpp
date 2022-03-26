#include "listwidgetiditem.h"

ListWidgetIdItem::ListWidgetIdItem(const int id, const QString &text, QListWidget *parent) :
    QListWidgetItem(text, parent, 1000)
{
    identifier = id;
}

int ListWidgetIdItem::id()
{
    return identifier;
}

