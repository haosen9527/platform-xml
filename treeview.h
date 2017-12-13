#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QDialog>
#include <QMainWindow>
#include <QtXml>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QComboBox>
#include <QDirModel>
#include <QMap>
#include <QIcon>
#include <QStandardItemModel>
#include <QDomComment>
#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QStandardItem>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <stdlib.h>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPoint>
#include <QCursor>
#include <QEvent>
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QHideEvent>
#include <QListWidget>

namespace Ui {
class treeview;
}

class treeview : public QDialog
{
    Q_OBJECT

public:
    explicit treeview(QWidget *parent = 0);
    ~treeview();

    void openXML_mem(QDomDocument dom);
    void listDom(QDomElement& docElem, QTreeWidgetItem * pItem);

private:
    Ui::treeview *ui;
};

#endif // TREEVIEW_H
