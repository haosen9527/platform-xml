#ifndef PLATFORMCLIENT_H
#define PLATFORMCLIENT_H

#include <QMainWindow>
#include <QtXml/QDomDocument>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QWidget>
#include <QComboBox>
#include <QDirModel>
#include <QMap>
#include <QIcon>
#include <QStandardItemModel>
#include <QtXml/QDomComment>
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

#include <dbxml_sql.h>
#include "setting.h"


namespace Ui {
class platformclient;
}

class platformclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit platformclient(QWidget *parent = 0);
    ~platformclient();
    void listDom(QDomElement& docElem);
    void listDom_custom(QDomElement& docElem, QTreeWidgetItem* PItem);
    bool openXML(QString fileName);


    void check_new_tagname(QDomElement& docElem);
    void open_check_xml(QString fileName);

    void load_templet();

    QMenu* menu[100];
    QAction* act[100];

private:
    Ui::platformclient *ui;

    //*****
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

        //预加载数据库
         dbxml_sql w;

         //设计标志位
         int check_save=0;
         QString file_Name;
         QString file_Path;

         //检索新标签
         QStringList temp_tagname;
         QStringList templet_tag_lib;
         QStringList new_tag_lib;

         setting set_btn;

private slots:

    void openFile();
    void slotSaveAs();

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);

    bool check(QString fileName);
    void on_Cancel_clicked();
    void on_merge_clicked();

    void addplatform();
    void addDev();
    void gripadd();
    void del();

    void del_sql();
    void edit_sql();
    void showRightMenu(QPoint pos);
    void list_showRightMenu(QPoint pos);
    void onGrpMenuTriggered(QAction *action);
    void onMemberMenuTriggered(QAction *action);
    void onAddAreaMenuTriggered(QAction *action);

    //可编辑tree
    void openEditor(QTreeWidgetItem *item, int column);
    void closeEditor();

    //复选框
    int onStateChanged(int state);

    //模板
    void open_temp_XML(QDomDocument dom, QString Tagname);
    void listDom_temp(QDomElement docElem);

    //独立检索标签
    void open_check_xml_only(QDomDocument dom);

    void doClicked(const QString& Tag_name);

    //found
    void foundTagname(QString Tagname);

    //合并
    bool new_merge();
    bool merge();
    //check mach
    bool has_new_tagname(QDomDocument ,QDomDocument);

    void on_action_New_Folder_triggered();
    void on_action_New_File_triggered();
    void on_save_as_triggered();
    void on_action_D_triggered();
    void on_refresh_triggered();
    void on_treeWidget_custom_clicked(const QModelIndex &index);
    void on_pushButton_clicked();

    void on_openfile_triggered();

    void on_pushButtonOpen_2_clicked();
    void on_edit_triggered();
    void on_Help_triggered();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_pushButtonOpen_clicked();

    //数据库相关
    void slotSaveAs_dbxml();
    void openXML_mem(QDomDocument dom);
    void on_setting_triggered();

    void lineEditGetTem(QString,int);
};

#endif // PLATFORMCLIENT_H
