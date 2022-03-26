#include "baiteditdelegate.h"
// l #include "editbaitdialog.h"
#include "baitmodel.h"

#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QComboBox>
#include <QtDebug>

/*
 * Constructor
 */
BaitEditDelegate::BaitEditDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
    theDialog = dynamic_cast<EditBaitDialog*>(parent);
    prefImageHeight = 128;
    prefImageWidth = 256;
}

/*
 * Draw a string or image in a table cell if drawing (or redrawing) is necessary.
 */
void BaitEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == BaitModel::BaitImage ) {
//        QString fName = qvariant_cast<QString>(index.data());
        QString fName = theDialog->getImagePathName();
//        QString fName = imagePath;
        painter->save();
        if ( option.state & QStyle::State_Selected ) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        if ( ! QFile::exists(fName) ) {
            fName = ":/images/nobait.png";
        }
        QImage image(fName);
        QPixmap pixmap = QPixmap::fromImage(image);
        painter->drawPixmap(option.rect, pixmap);
        painter->restore();
        qDebug() << "BaitEditDelegate: image drawn via paint()";
    } else {
        qDebug() << "BaitEditDelegate: paint() redirected";
        QStyledItemDelegate::paint(painter, option, index);
    }
}

/*
 * Create an editor for editing of the image reference.
 */
QWidget *BaitEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    if ( index.column() == BaitModel::BaitImage ) {
        qDebug() << "BaitEditDelegate: createEditor() for image editing";
        return QStyledItemDelegate::createEditor(parent, option, index);
    } else {
        qDebug() << "BaitEditDelegate: createEditor() redirected";
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

/*
 * Fill the widget with data of the model.
 */
void BaitEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str;
    QPixmap pixmap, scaledPixmap;
    int indexColor, indexType;
    QComboBox *comboBox;
    QLabel *imageLabel;
    switch ( index.column() ) {
    case BaitModel::BaitColor :
        comboBox = dynamic_cast<QComboBox*>(editor);             // pointer to combo box of bait color selection
        str = (index.model()->data(index, Qt::EditRole)).toString();
        if ( ( indexColor = comboBox->findText(str) ) >= 0 ) {
//            qDebug() << "BaitEditDelegate: set editor data for color. index found : " << str << ", index : " << indexColor;
            comboBox->setCurrentIndex(indexColor);
        } else  {
            qDebug() << "BaitEditDelegate: set editor data for color. index not found!";
        }
        break;
    case BaitModel::BaitType :
        comboBox = dynamic_cast<QComboBox*>(editor);             // pointer to combo box of bait type selection
        str = (index.model()->data(index, Qt::EditRole)).toString();
        if ( ( indexType = comboBox->findText(str) ) >= 0 ) {
//            qDebug() << "BaitEditDelegate: set editor data for type. index found : " << str << ", index : " << indexType;
            comboBox->setCurrentIndex(indexType);
        } else  {
            qDebug() << "BaitEditDelegate: set editor data for type. index not found!";
        }
        break;
    case BaitModel::BaitImage :
        imageLabel = dynamic_cast<QLabel*>(editor);             // pointer to label of bait image
        str = (index.model()->data(index, Qt::EditRole)).toString();
//        imagePath = (index.model()->data(index, Qt::EditRole)).toString();
        theDialog->setImagePathName(str);
        if ( ! QFile::exists(str) ) {
            pixmap.load(":/images/nobait.png");
        } else {
            pixmap.load(str);
        }
        scaledPixmap = pixmap.scaled(prefImageWidth, prefImageHeight);
        imageLabel->setPixmap(scaledPixmap);
        qDebug() << "BaitEditDelegate: set editor data for image. model data is : " << str;
        break;
    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

/*
 * Write the widget data to the model.
 */
void BaitEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug() << "BaitEditDelegate: setModelData() called; column index : " << index.column();
    QComboBox *comboBox;
    QLabel *imageLabel;
    switch ( index.column() ) {
    case BaitModel::BaitColor:
        comboBox = dynamic_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        qDebug() << "BaitEditDelegate: bait color set to " << comboBox->currentText();
        break;
    case BaitModel::BaitType:
        comboBox = dynamic_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        qDebug() << "BaitEditDelegate: bait type set to " << comboBox->currentText();
        break;
    case BaitModel::BaitImage:
        imageLabel = dynamic_cast<QLabel*>(editor);
        imageLabel->setText(theDialog->getImagePathName());
//        imageLabel->setText(imagePath);
        model->setData(index, imageLabel->text(), Qt::EditRole);
        qDebug() << "BaitEditDelegate: bait image set to " << imageLabel->text();
        break;
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

/*
 * Return the preferred size of the image which shall be drawn in the image column of the table.
 */
QSize BaitEditDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.column() == BaitModel::BaitImage ) {
        return QSize(prefImageWidth, prefImageHeight);
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

/*
 * Context menue of bait image activated.
 */
//void BaitEditDelegate::onImageContextMenu()
//{
//    qDebug() << "BaitEditDialog: image context menu activated.";
//}

