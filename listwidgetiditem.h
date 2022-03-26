#ifndef LISTWIDGETIDITEM_H
#define LISTWIDGETIDITEM_H

#include <QListWidgetItem>
#include <QListWidget>

class ListWidgetIdItem : public QListWidgetItem
{
private:
    int identifier;
public:
    ListWidgetIdItem(const int id, const QString &text, QListWidget *parent);
    int id();
};

#endif // LISTWIDGETIDITEM_H
