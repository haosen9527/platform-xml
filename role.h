#ifndef ROLE_H
#define ROLE_H

#include <QMainWindow>
#include <QtXml>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QWidget>
#include <QComboBox>
#include <QDirModel>
#include <QMap>
#include <QIcon>
#include <QStandardItemModel>
#include <QDomComment>
#include <QContextMenuEvent>
#include <QDirModel>
#include <QFile>
#include <QFileDialog>
#include <QStandardItem>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <stdlib.h>
#include <vector>
#include <QApplication>
#include <QStringList>
#include <set>
#include <QDesktopServices>

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
#include "dbxml_sql.h"
#include <file_list.h>

#include "create_new_xml.h"

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

   // User dom_list[100];

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

    //模板存储
    QDomDocument temp_memry;

    //数据库初始化
    dbxml_sql actor_sql;

   // create_new_xml *child_new_xml=new create_new_xml;

private slots:

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);
    bool check(QString fileName);

    int onStateChanged(int state);

    void openXML_mem(QDomDocument dom);
    void get_item(QListWidgetItem* item);
    void active_add();
    void openFile();
    void openFile_top();
    void ok();
    void cancel();
    void showRightMenu(QPoint pos);
    void cat_template();
    void doClicked(const QString& Tag_name);
    void addplatform();
    void addDev();
    void gripadd();
    void del();
    void openEditor(QTreeWidgetItem *item, int column);
    void closeEditor();

    void on_treeWidget_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_add_clicked();
    void view_showRightMenu();
    void on_listWidget_sql_itemDoubleClicked(QListWidgetItem *item_sql);
    void on_action_templte_triggered();

    void foundTagname(QString Tagname);
    //读模板
    void open_temp_XML(QDomDocument dom,QString Tagname);
    void listDom_temp(QDomElement docElem);

    void on_action_new_actor_triggered();
    void on_action_openpackage_triggered();
    void on_action_HELP_hovered();


    void add_show_tree(QDomDocument dom);
    void listDom_re(QDomElement& docElem, QTreeWidgetItem * pItem);
    void on_create_new_file_clicked();
    void onItemSelectionChanged(QPoint pos);

    void create_xml();
    void lineEditGetTem(QString str);

    bool has_new_tagname(QDomDocument ,QDomDocument );
    void open_check_xml_only(QDomDocument);

    void open_check_xml(QString);
    void check_new_tagname(QDomElement& docElem);


};

#endif // ROLE_H
