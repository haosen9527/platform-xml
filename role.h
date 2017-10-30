#ifndef ROLE_H
#define ROLE_H

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
class Role;
}

class Role : public QMainWindow
{
    Q_OBJECT

public:
    explicit Role(QWidget *parent = 0);
    ~Role();

    void listDom(QDomElement& docElem, QTreeWidgetItem* PItem);
    void listDom_top(QDomElement& docElem, QTreeWidgetItem * pItem);
    void openXML(QString fileName);
    void openXML_top(QString fileName);

    QDirModel *dirModel;

private:
    Ui::Role *ui;

    //grp menu
    QMenu *m_grpMenu;
    //child menu
    QMenu *m_memberMenu;
    QMenu *m_addAreaMenu;

    //grp action
    QAction *m_grpAddOneMember;
    QAction *m_grpDeleteGrp;
    QAction *m_grpAddOneGrp;
    //child action
    QAction *m_memberDelete;
    QAction *m_memberMove;
    QAction *m_memberSyncTime;
    //action
    QAction *m_addAreaAction;
    QAction *m_grpFlush;
    QAction *m_addDev;

private slots:

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);
    bool check(QString fileName);
    void get_item(QListWidgetItem* item);
    void active_add();
    void openFile();
    void openFile_top();
    void ok();
    void cancel();
    void showRightMenu();
    void addplatform();
    void addDev();
    void gripadd();
    void del();
    void change(QTreeWidgetItem *item, int column);
    void openEditor(QTreeWidgetItem *item, int column);
    void closeEditor(QTreeWidgetItem *item, int column);

    void on_treeWidget_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void view_showRightMenu();
};

#endif // ROLE_H
