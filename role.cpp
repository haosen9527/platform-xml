#include "role.h"
#include "ui_role.h"
#include <QDesktopServices>
#include "treeview.h"
//#include <file_list.h>

using namespace std;

Role::Role(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Role)
{
    ui->setupUi(this);
    this->setWindowTitle("角色开发者平台");
    this->setWindowIcon(QIcon("./开发.png"));
    //set width
    ui->treeWidget->setColumnWidth(0, 350);
    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    //list drag
    ui->listWidget->setDragEnabled(true);
    //init check_box
    ui->checkBox->setChecked(true);
    ui->checkBox->setHidden(true);
    ui->openpackage->setHidden(true);
    ui->pushButton_clear->setHidden(true);

    // dialog
    //create_new_xml *child_new_xml=new create_new_xml();

//    create_new_xml *child_new_xml=new create_new_xml();

//    connect(ui->create_new_file, SIGNAL(clicked()), child_new_xml, SLOT(create_xml()));
//    connect(child_new_xml, SIGNAL(sendString(QString)), this, SLOT(lineEditGetTem(QString)));


    //load sql
    /***********************************************************************************************/
    QStringList ID_NUM=actor_sql.query_ID("/_ARMER/_P_ID/string()","./","default.dbxml");
    if(ID_NUM.isEmpty())
    {
        QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("当前路径下无默认数据库(default.dbxml)文件！"), QMessageBox::Ok);
        tmpMsgBox.exec();
    }
    int size=ID_NUM.size();
    //SQL show list
    for(int i=0;i<size;i++)
    {
        if(ID_NUM[i]=="templetxml")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget_sql);
            pItem->setText(ID_NUM[i]);  // 文本
            ui->listWidget_sql->addItem(pItem);
        }
    }
    //template
    string text="templetxml";
    QString X=actor_sql.getDocument("./","default.dbxml",text);
    temp_memry.setContent(X);
    //template load
    //openXML_mem(temp_memry);
    /***********************************************************************************************/

    connect(ui->pushButton_open, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->button_ok,SIGNAL(clicked()),this,SLOT(ok()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showRightMenu(QPoint)));
    connect(ui->listWidget_sql,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(view_showRightMenu()));
    connect(ui->openpackage,SIGNAL(clicked()),this,SLOT(active_add()));
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

    //connect
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(openEditor(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(closeEditor()));

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(get_item(QListWidgetItem*)));
}

int count_flog=1;
int bool_flog=0;
int file_title=0;
QStringList Refresh;

QListWidgetItem* test;
int nID;
int temp=0;
QString top;
QStringList P_value;
QString Now_Tagname;
QDomDocument file_dom_temp;
vector <QDomDocument> dom_list;
vector <QDomDocument> refresh_list;
vector <QString> package_name_list;

QStringList temp_tagname;
//sql template load
void Role::openXML_mem(QDomDocument dom)
{

            ui->treeWidget->clear();
            QDomElement docElem = dom.documentElement();
            listDom(docElem,NULL);
}


/********************
 * open xml file
 ********************/
void Role::openXML(QString fileName)
{
    QString text="ACTOR";
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        if (dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();

            file_title++;

            dom_list.push_back(dom);

            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0,text);
            item->setBackgroundColor(0,QColor(200, 93, 12,255));
            //item->setText(1,QString("%1").arg(file_title));
            item->setBackgroundColor(1,QColor(200, 93, 12,255));

            listDom(docElem, NULL);
        }
    }
    file.close();
}
void Role::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "c:/", tr("XML Files (*.xml)"));

    qDebug()<<"file"<<fileName;

    if(!check(fileName))
    {
        QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("不是标准的角色文件(未包含_ACTOR标签)！"), QMessageBox::Ok);
        tmpMsgBox.exec();

        return;
    }

    if(!fileName.isEmpty())
    {
        openXML(fileName );
    }
}
bool Role::check(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        if (!dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();
            qDebug()<<"test:"<<docElem.tagName();

            if(docElem.tagName()=="_ACTOR")
            {
                return true;
            }
            return false;
        }
    }

    file.close();
    }

//查重
QStringList topic_list;

void Role::listDom(QDomElement& docElem, QTreeWidgetItem * pItem)
{
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        if(docElem.tagName()=="_LARGER_THAN"||docElem.tagName()=="_SMALLER_THAN")
        {
            qDebug()<<"docElem.text()"<<docElem.text();
        }
        else
        {
            pItem->setText (1, docElem.text());
        }
        qDebug()<<"docElem.text()"<<docElem.tagName();
        //cancel BIND tagname
//        if(docElem.tagName()=="_BIND")
//        {
//            topic_list.push_back(docElem.text());
//        }
        if(docElem.tagName()=="_P_NAME")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
             pItem->setData(Qt::UserRole,file_title);  // 用户数据
            pItem->setText(docElem.text());  // 文本

            package_name_list.push_back(docElem.text());

            ui->listWidget->addItem(pItem);
        }
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()!="_LARGER_THAN"&& docElem.tagName()!="_SMALLER_THAN")
        {
            if( !element.isNull())
            {
                QTreeWidgetItem *item;
                if( pItem )
                {
                    item = new QTreeWidgetItem(pItem);
                }
                else
                {
                    item = new QTreeWidgetItem(ui->treeWidget);
                }

                if(element.firstChildElement().tagName()=="_LARGER_THAN"||element.firstChildElement().tagName()=="_SMALL_THAN")
                {
                    QString add_fuhao;
                    item->setText(0, element.tagName());
                    if(element.firstChildElement().tagName()=="_LARGER_THAN")
                    {
                        add_fuhao=">"+element.firstChildElement().text();
                    }
                    else if(element.firstChildElement().tagName()=="_SMALLER_THAN")
                    {
                        add_fuhao="<"+element.firstChildElement().text();
                    }
                    item->setText(1,add_fuhao);
                    element.removeChild(node);
                }

                    item->setText(0, element.tagName());

                listDom(element, item);
                if( pItem)
                {
                    pItem->addChild(item);
                }
                else
                {
                    ui->treeWidget->addTopLevelItem(item);
                }
            }
        }
        node = node.nextSibling();
    }
    return;
}
void Role::on_treeWidget_clicked(const QModelIndex &index)
{
    QString name,info;
    if(index.column() == 0)
    {
        name = index.data().toString();
        info = index.sibling(index.row(),1).data().toString();
    }
    else
    {
        name = index.sibling(index.row(),0).data().toString();
        info = index.data().toString();
    }
    QString str1,str2,str3;
    //寻找顶层
    top=QStringLiteral("%1").arg(index.parent().data().toString());

    str1=QStringLiteral("%1").arg(index.parent().data().toString());
    str3 = QStringLiteral("%1").arg(name);

    str2 = QStringLiteral("%1").arg(info);

}
/*******************
*生成文件
********************/
void Role::ok()
{
    QTreeWidgetItem *rootItem = ui->treeWidget->invisibleRootItem(); //树的根节点
    int countGroup = rootItem->childCount();

    if(countGroup <=0)//无内容提示
    {
        QMessageBox WARNING_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("无内容"), QMessageBox::Ok);
        WARNING_MsgBox.exec();
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As XML File"), "./custom_xml.xml", tr("XML Files (*.xml)"));

    QFile db(fileName);
    if (!db.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    //只写方式打开，并清空以前的信息
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("_ACTOR");
    doc.appendChild(root); //添加根元素

    for (int i = 0; i < countGroup; i++)
    {
        QTreeWidgetItem *groupitem = rootItem->child(i);

        QString groupname = groupitem->text(0);

        QDomElement elementGrp = doc.createElement(groupname);
        if(groupitem->text(0)!="ACTOR")
        {
            root.appendChild(elementGrp);
        }
        int countDev = groupitem->childCount();
        //  角色名字
        if(groupitem->text(1)!="")
        {
            QDomText strMacNode = doc.createTextNode(groupitem->text(1));
            elementGrp.appendChild(strMacNode);
        }
        for (int j = 0; j < countDev; j++)
        {
//            QTreeWidgetItem *deviceitem = groupitem->child(j);
//            QString devicename = deviceitem->text(0);
//            QDomElement elementDev;

////            if(deviceitem->text(0)=="bind" ||deviceitem->text(0)=="BIND")
////            {
////                devicename="_P_BIND";
////            }
//            elementDev = doc.createElement(devicename);
//            elementGrp.appendChild(elementDev);

//            if(deviceitem->text(1)!="")
//            {
//                if(!devicename.startsWith("_P_"))
//                {
//                    devicename=QString("_P_")+devicename;
//                }
//                elementDev = doc.createElement(devicename);
//                elementGrp.appendChild(elementDev);

//                QDomText strMacNodeText_1 = doc.createTextNode(deviceitem->text(1));
//                elementDev.appendChild(strMacNodeText_1);
//            }
//            else
//            {
////                elementDev = doc.createElement(devicename);
////                elementGrp.appendChild(elementDev);
//            }
            QTreeWidgetItem *deviceitem = groupitem->child(j);
            //Device项属性name
            QString devicename = deviceitem->text(0);
            QDomElement elementDev = doc.createElement(devicename);
            elementGrp.appendChild(elementDev);

            if(deviceitem->text(1)!="")
            {
                QDomText strMacNodeText = doc.createTextNode(deviceitem->text(1));
                elementDev.appendChild(strMacNodeText);
            }

            int countChn = deviceitem->childCount();
            for (int k = 0; k< countChn; k++)
            {
                QTreeWidgetItem *channelitem = deviceitem->child(k);
                QString channelname = channelitem->text(0);
                QDomElement elementChn;

                elementChn = doc.createElement(channelname);
                elementDev.appendChild(elementChn);

                //如果这一级存在 text(1)
                if(channelitem->text(1)!="")
                {
                    if(!channelname.startsWith("_P_"))
                    {
                        channelname=QString("_P_")+channelname;
                    }
                    QDomText strMacNodeText = doc.createTextNode(channelitem->text(1));
                    elementChn.appendChild(strMacNodeText);
                }


                int countChm = channelitem->childCount();
                for(int m=0;m < countChm;m++)
                {
                    QTreeWidgetItem *channelitem_m = channelitem->child(m);
                    QString channelname_m = channelitem_m->text(0);

                    QString fuhao=channelitem_m->text(1);

                    QDomElement channe_element;

                    channe_element = doc.createElement(channelname_m);
                    elementChn.appendChild(channe_element);

                    if(channelitem_m->text(1)!="")
                    {
                        if(!channelname_m.startsWith("_P_"))
                        {
                            channelname_m=QString("_P_")+channelname_m;
                        }
                        if(fuhao.startsWith(">"))
                        {

//                            QDomElement elementChm = doc.createElement(channelname_m);
//                            elementChn.appendChild(elementChm);

                            QDomElement con = doc.createElement("_LARGER_THAN");
                            channe_element.appendChild(con);
                            fuhao=fuhao.mid(1, fuhao.length()-1);

                            QDomText strMacNodeText = doc.createTextNode(fuhao);
                            con.appendChild(strMacNodeText);
                        }
                        else if(fuhao.startsWith("<"))
                        {

//                            QDomElement elementChm = doc.createElement(channelname_m);
//                            elementChn.appendChild(elementChm);

                            QDomElement con1 = doc.createElement("_SMALLER_THAN");
                            channe_element.appendChild(con1);
                            fuhao=fuhao.mid(1, fuhao.length()-1);

                            QDomText strMacNodeText = doc.createTextNode(fuhao);
                            con1.appendChild(strMacNodeText);
                        }
                        else
                        {
//                            QDomElement elementChm = doc.createElement(channelname_m);
//                            elementChn.appendChild(elementChm);

                            QDomText strMacNodeText = doc.createTextNode(channelitem_m->text(1));
                            channe_element.appendChild(strMacNodeText);
                        }
                    }
//                    else
//                    {
//                        //qDebug()<<"-----------------------"<<channelname_m<<"--------------------------";
//                        channe_element = doc.createElement(channelitem_m->text(0));
//                        elementChn.appendChild(channe_element);
//                    }

                   int in_channe =channelitem_m->childCount();
                   for(int n=0;n<in_channe;n++)
                   {
                       QTreeWidgetItem *channeliten = channelitem_m->child(n);
                       QString channelname_n = channeliten->text(0);
                       QDomElement elementChn_n;
                       qDebug()<<"-----------------------"<<channelname_n<<"--------------------------";

                       elementChn_n = doc.createElement(channelname_n);
                       channe_element.appendChild(elementChn_n);

                       if(channeliten->text(1)!="")
                       {
                           if(!channelname_n.startsWith("_P_"))
                           {
                               channelname_n=QString("_P_")+channelname_n;
                           }
                           QDomText strMacNodeText = doc.createTextNode(channeliten->text(1));
                           elementChn_n.appendChild(strMacNodeText);

                       }

                       int inn_channe=channeliten->childCount();
                       for(int o=0;o<inn_channe;o++)
                       {
                           QTreeWidgetItem *channelite_o =channeliten->child(o);
                           QString channename_o=channelite_o->text(0);
                           QDomElement elementChn_o;
                           if(channelite_o->text(1)!="")
                           {
                               if(!channename_o.startsWith("_P_"))
                               {
                                   channename_o=QString("_P_")+channename_o;
                               }
                               QDomText str_last_nodetext=doc.createTextNode(channelite_o->text(1));
                               elementChn_n.appendChild(str_last_nodetext);
                           }
                           else
                           {
                               elementChn_o=doc.createElement(channename_o);
                               elementChn_n.appendChild(elementChn);
                           }
                       }

                   }
                }
            }
        }
    }

    QTextStream out(&db);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    db.close();

    QMessageBox tmpMsgBox(QMessageBox::Information, QStringLiteral("提示"), QStringLiteral("保存Xml文件成功！"), QMessageBox::Ok);
    tmpMsgBox.exec();

}

/*************
 * clear op
**************/
void Role::cancel()
{
    ui->treeWidget->clear();
    ui->listWidget->clear();
    dom_list.clear();
    refresh_list.clear();
    //文件计数置位
    file_title=0;
}
Role::~Role()
{
    delete ui;
}

/*************
 * right menu
**************/
int stateflog=1;

void Role::showRightMenu(QPoint pos)
{
    if(stateflog==1)
    {
        QMenu * m_grpMenu = new QMenu(ui->treeWidget);
        QMenu * m_addAreaMenu = new QMenu(ui->treeWidget);

        m_grpMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");
        m_addAreaMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");

        QSignalMapper *pMapper = new QSignalMapper();

        QAction *m_addAreaAction = new QAction(tr("add _PLATFORM"),this);
        QAction *m_addDev = new QAction(tr("add _SENSOR"),this);
        QAction *m_memberDelete = new QAction(tr("delete"),this);

        connect(m_addAreaAction,SIGNAL(triggered()),this,SLOT(addplatform()));
        connect(m_addDev,SIGNAL(triggered()),this,SLOT(addDev()));

        connect(m_memberDelete,SIGNAL(triggered()),this,SLOT(del()));

        //追综鼠标  点击其他空白区域取消当前
        QTreeWidgetItem *item=ui->treeWidget->itemAt(pos);


        if(item == NULL)
        {
            ui->treeWidget->clearSelection();
            if (m_addAreaMenu->isEmpty())
            {
                //add
                m_addAreaMenu->addAction(m_addAreaAction);
                m_addAreaMenu->addAction(m_addDev);
            }
            //current pos
            m_addAreaMenu->exec(QCursor::pos());
        }
        else if(ui->treeWidget->currentItem()->childCount() > 0 || ui->treeWidget->currentItem()->childCount() == 0)
        {
            //current
            Now_Tagname=ui->treeWidget->currentItem()->text(0);

            P_value.clear();

            foundTagname(Now_Tagname);

            if (m_grpMenu->isEmpty())
            {
                for(int i=0;i<P_value.size();i++)
                {
                    QAction *mulu = new QAction("添加 "+P_value[i]);
                    m_grpMenu->addAction(mulu);

                    //动态添加鼠标右键目录
                    connect(mulu, SIGNAL(triggered()), pMapper, SLOT(map()));

                    pMapper->setMapping(mulu, P_value[i]);
                }
                m_grpMenu->addAction(m_memberDelete);

                connect(pMapper, SIGNAL(mapped (const QString &)), this, SLOT(doClicked(const QString &)));
            }
            m_grpMenu->exec(QCursor::pos());
        }
    }
    else
    {

        QMenu * m_grpMenu = new QMenu(ui->treeWidget);
        QMenu * m_memberMenu = new QMenu(ui->treeWidget);
        QMenu * m_addAreaMenu = new QMenu(ui->treeWidget);

        QAction *m_grpAddOneMember = new QAction(tr("增加子目录"),this);
        QAction *m_memberDelete = new QAction(tr("删除"),this);
        QAction *m_addAreaAction = new QAction(tr("增加_PLATFORM"),this);
        QAction *m_addDev = new QAction(tr("增加_SENSOR"),this);
        QAction *m_grpFlush = new QAction(tr("刷新列表"),this);

        m_grpMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");
        m_addAreaMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");
        m_memberMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");

        connect(m_addAreaAction,SIGNAL(triggered()),this,SLOT(addplatform()));
        connect(m_addDev,SIGNAL(triggered()),this,SLOT(addDev()));
        connect(m_grpAddOneMember,SIGNAL(triggered()),this,SLOT(gripadd()));
        connect(m_memberDelete,SIGNAL(triggered()),this,SLOT(del()));

        QTreeWidgetItem *item=ui->treeWidget->currentItem();

        if(item == NULL)
        {
            //取消鼠标选中状态
            ui->treeWidget->clearSelection();
            if (m_addAreaMenu->isEmpty())
            {
                //增加区域
                m_addAreaMenu->addAction(m_addAreaAction);
                m_addAreaMenu->addAction(m_addDev);
                m_addAreaMenu->addAction(m_grpFlush);
            }
            //菜单出现的位置为当前鼠标的位置
            m_addAreaMenu->exec(QCursor::pos());
        }
        else if(ui->treeWidget->currentItem()->childCount() > 0)
        {
            //qDebug()<<"当前点击2"<<ui->treeWidget_custom->currentItem()->text(0);
            if (m_grpMenu->isEmpty())
            {
                m_grpMenu->addAction(m_addAreaAction);
                m_grpMenu->addAction(m_addDev);
                m_grpMenu->addAction(m_grpAddOneMember);
                m_grpMenu->addAction(m_memberDelete);
                m_grpMenu->addAction(m_grpFlush);
            }
            m_grpMenu->exec(QCursor::pos());
        }
        else if(ui->treeWidget->currentItem()->childCount() == 0)
        {
            if (m_memberMenu->isEmpty())
            {
                //删除设备
                m_memberMenu->addAction(m_grpAddOneMember);
                m_memberMenu->addAction(m_memberDelete);
            }
            m_memberMenu->exec(QCursor::pos());
        }

    }
}
//添加相对应的选项
void Role::doClicked(const QString& Tag_name)
{
    QTreeWidgetItem* item=ui->treeWidget->currentItem();
    QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<Tag_name);
    item->addChild(A);
}

void Role::addplatform()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget);
    newgroup->setText(0, QStringLiteral("_PLATFORM"));
}
void Role::addDev()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget);
    newgroup->setText(0, QStringLiteral("_SENSOR"));
}
void Role::gripadd()
{
    QTreeWidgetItem* item=ui->treeWidget->currentItem();
    QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<"Acostomname");
    item->addChild(A);
}
void Role::del()
{
    QTreeWidgetItem* curritem=ui->treeWidget->currentItem();
    if (curritem != NULL)
    {
        //全部项都可删除
        QTreeWidgetItem *curritemparent = curritem->parent();
        if (curritemparent == NULL)
        {
            int index = ui->treeWidget->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget->takeTopLevelItem(index);
            }
        }
        else
        {
            int countChild = curritemparent->childCount();
            for (int i = 0; i < countChild; i++)
            {
                if (curritemparent->child(i) == curritem)
                {
                    curritemparent->takeChild(i);
                    break;
                }
            }
        }
        //只可删除组
        QString strItemType = curritem->whatsThis(0);
        if (strItemType == QStringLiteral("Group"))
        {
            int index = ui->treeWidget->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget->takeTopLevelItem(index);
            }
        }

    }
}
//获取parent message
QStandardItem* Role::getTopParent(QStandardItem* item)
{
    QStandardItem* secondItem = item;
    while(item->parent()!= 0)
    {
        secondItem = item->parent();
        item = secondItem;
    }
    if(secondItem->index().column() != 0)
    {
        QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeWidget->model());
        secondItem = model->itemFromIndex(secondItem->index().sibling(secondItem->index().row(),0));
    }
    return secondItem;
}
QModelIndex Role::getTopParent(QModelIndex itemIndex)
{
    QModelIndex secondItem = itemIndex;
    while(itemIndex.parent().isValid())
    {
        secondItem = itemIndex.parent();
        itemIndex = secondItem;
    }
    if(secondItem.column() != 0)
    {
        secondItem = secondItem.sibling(secondItem.row(),0);
    }
    return secondItem;
}
/*******************
 * edit tree
 * ******************/
QTreeWidgetItem *temitem;
int temColumn;

void Role::openEditor(QTreeWidgetItem *item, int column)
{
    ui->treeWidget->openPersistentEditor(item,column);// 设置某一item可以编辑
    temitem = item;
    temColumn = column;
}

// 当修改完以后或者选中其他列的时候，关闭可编辑状态
void Role::closeEditor()
{
    if(temitem != NULL)
    {
        if(temitem->text(0)=="_BIND" || temitem->text(0)=="BIND" ||temitem->text(0)=="bind")
        {
            if(top != "_SENSOR")
            {
                QMessageBox WAR_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("不能在此添加bind"), QMessageBox::Ok);
                WAR_MsgBox.exec();
            }
            else if( -1!= topic_list.indexOf(temitem->text(1)))
            {
                QMessageBox WARNING_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("请重新命名bind名"), QMessageBox::Ok);
                WARNING_MsgBox.exec();
            }
        }
        // 设置某一item不可编辑
         ui->treeWidget->closePersistentEditor(temitem,temColumn);
    }
    //归位
    temitem=NULL;
    temColumn=0;
}


/******************
*table list
*******************/
void Role::active_add()
{
    openFile_top();
}

void Role::openXML_top(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        if (dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();
            listDom_top(docElem, NULL);
        }
    }
    file.close();
}

void Role::openFile_top()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml)"));
    if(!check(fileName))
    {
        QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("不是标准的角色文件(未包含_ACTOR标签)！"), QMessageBox::Ok);
        tmpMsgBox.exec();
        return;
    }
    if(!fileName.isEmpty())
    {
        openXML_top( fileName );
    }
}
//
void Role::listDom_top(QDomElement& docElem, QTreeWidgetItem * pItem)
{
}

int listnum=0;


void Role::get_item(QListWidgetItem* item)
{
}
//删除
void Role::on_pushButton_clicked()
{
    QListWidgetItem *del_item=ui->listWidget->currentItem();

    if(del_item!=NULL)
    {

        int del_NO=del_item->data(Qt::UserRole).toInt();

        file_title--;

        dom_list.erase(dom_list.begin()+del_NO-1);

        qDebug()<<"..........................."<<del_NO-1<<".......................";

        ui->listWidget->clear();
        ui->treeWidget->clear();
        file_title=0;
        for(int i=0;i<dom_list.size();i++)
        {
            file_title++;
            QString text="ACTOR";
            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0,text);
            item->setBackgroundColor(0,QColor(200, 93, 12,255));
            //item->setText(1,QString("%1").arg(file_title));
            item->setBackgroundColor(1,QColor(200, 93, 12,255));

            QDomElement  re =dom_list[i].documentElement();
            listDom_re(re,NULL);
        }
    }
    else
    {
        return;
    }
}
//refresh
void Role::listDom_re(QDomElement& docElem, QTreeWidgetItem * pItem)
{
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        if(docElem.tagName()=="_LARGER_THAN"||docElem.tagName()=="_SMALLER_THAN")
        {
            qDebug()<<"docElem.text()"<<docElem.text();
        }
        else
        {
            pItem->setText (1, docElem.text());
        }
        qDebug()<<"docElem.text()"<<docElem.tagName();

        if(docElem.tagName()=="_P_NAME")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
             pItem->setData(Qt::UserRole,file_title);  // 用户数据
            pItem->setText(docElem.text());  // 文本

            package_name_list.push_back(docElem.text());

            ui->listWidget->addItem(pItem);
        }
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()!="_LARGER_THAN"&& docElem.tagName()!="_SMALLER_THAN")
        {
            if( !element.isNull())
            {
                QTreeWidgetItem *item;
                if( pItem )
                {
                    item = new QTreeWidgetItem(pItem);
                }
                else
                {
                    item = new QTreeWidgetItem(ui->treeWidget);
                }

                if(element.firstChildElement().tagName()=="_LARGER_THAN"||element.firstChildElement().tagName()=="_SMALL_THAN")
                {
                    QString add_fuhao;
                    item->setText(0, element.tagName());
                    if(element.firstChildElement().tagName()=="_LARGER_THAN")
                    {
                        add_fuhao=">"+element.firstChildElement().text();
                    }
                    else if(element.firstChildElement().tagName()=="_SMALLER_THAN")
                    {
                        add_fuhao="<"+element.firstChildElement().text();
                    }
                    item->setText(1,add_fuhao);
                    element.removeChild(node);
                }

                    item->setText(0, element.tagName());

                listDom(element, item);
                if( pItem)
                {
                    pItem->addChild(item);
                }
                else
                {
                    ui->treeWidget->addTopLevelItem(item);
                }
            }
        }
        node = node.nextSibling();
    }
    return;
}


//clear list
void Role::on_pushButton_clear_clicked()
{
    ui->listWidget->clear();
}
//添加
void Role::on_pushButton_add_clicked()
{
    QListWidgetItem *listitem=ui->listWidget->currentItem();
    if(listitem==NULL)
    {
        return;
    }
    else if(file_title!=0)
    {
        int dom_num=listitem->data(Qt::UserRole).toInt();
        add_show_tree(dom_list[dom_num-1]);
    }
    else
    {
        return;
    }
}

void Role::add_show_tree(QDomDocument dom)
{
    QDomElement doc_element=dom.documentElement();
    file_title++;

    dom_list.push_back(dom);

    QString text="ACTOR";

    QTreeWidgetItem *item;
    item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,text);
    item->setBackgroundColor(0,QColor(200, 93, 12,255));
    //item->setText(1,QString("%1").arg(file_title));
    item->setBackgroundColor(1,QColor(200, 93, 12,255));

    listDom(doc_element,NULL);
}


void Role::view_showRightMenu()
{
    QMenu * RightMenu = new QMenu(ui->listWidget_sql);
    QAction *cat = new QAction(tr("查看"),this);
    RightMenu->addAction(cat);
    RightMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");

    connect(cat,SIGNAL(triggered()),this,SLOT(cat_template()));

    RightMenu->exec(QCursor::pos());

}

//show template
void Role::cat_template()
{
    treeview *tree_template=new treeview();

     tree_template->openXML_mem(temp_memry);

    tree_template->show();



   //openXML_mem(temp_memry);
}

void Role::on_listWidget_sql_itemDoubleClicked(QListWidgetItem *item_sql)
{

    QListWidgetItem* item =ui->listWidget_sql->currentItem();

    ui->label_edit->setText("编辑数据库中"+item->text());

    string text=item->text().toStdString();

    std::cout<<"text:"<<text<<std::endl;

    QString X=actor_sql.getDocument("./","default_actor.dbxml",text);

    QDomDocument memry;
    memry.setContent(X);
    //双击显示树形结构
    openXML_mem(memry);
}

void Role::on_action_templte_triggered()
{
    on_listWidget_sql_itemDoubleClicked(NULL);
}
int Role::onStateChanged(int state)
{

    if (state == Qt::Checked) // "选中"
    {
            ui->checkBox->setText("使用模板");
            stateflog=1;
    }
    else
    {
        stateflog=2;
    }
}

//find Now_Tagname
void Role::foundTagname(QString Tagname)
{
    open_temp_XML(temp_memry,Tagname);
}


//read template
void Role::open_temp_XML(QDomDocument dom,QString Tagname)
{
            QDomElement docElem = dom.documentElement();
            listDom_temp(docElem);
}

void Role::listDom_temp(QDomElement docElem)
{

    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
    }
    if(node.parentNode().toElement().tagName()==Now_Tagname)
    {
        if(P_value.indexOf(node.toElement().tagName())!=-1)
        {
            return;
        }
        else{
            while(!node.toElement().isNull())
            {
                P_value.push_back(node.toElement().tagName());
                node = node.nextSibling();
            }
        }
    }
    //while(!node.nextSibling().isNull())
    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if( !element.isNull() )
        {
            listDom_temp(element);
        }
        node = node.nextSibling();
    }
    return;
}
//meun tool
void Role::on_action_new_actor_triggered()
{
    openFile();
}
void Role::on_action_openpackage_triggered()
{
    openFile_top();
}

void Role::on_action_HELP_hovered()
{
    ui->action_HELP->setText("查看帮助文档");
}

void Role::on_create_new_file_clicked()
{
    create_new_xml *child_new_xml=new create_new_xml();

    connect(ui->create_new_file, SIGNAL(clicked()), child_new_xml, SLOT(create_xml()));
    connect(child_new_xml, SIGNAL(sendString(QString)), this, SLOT(lineEditGetTem(QString)));

    child_new_xml->show();
}


void Role::onItemSelectionChanged(QPoint pos)
{
}

void Role::create_xml()
{

    qDebug()<<"\\\\\\\\\\"<<"test";
    //tiltle
    QTreeWidgetItem *item_root;
    item_root = new QTreeWidgetItem(ui->treeWidget);
    item_root->setText(0,"ACTOR");
    item_root->setBackgroundColor(0,QColor(200, 93, 12,255));
    //item->setText(1,QString("%1").arg(file_title));
    item_root->setBackgroundColor(1,QColor(200, 93, 12,255));

    //create new_file
//    QTreeWidgetItem *item;
//    item = new QTreeWidgetItem(ui->treeWidget);
//    item->setText(0,"_P_NAME");
//    item->setText(1, "please input name");
}

void Role::lineEditGetTem(QString str)
{
    file_title++;

    QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
     pItem->setData(Qt::UserRole,file_title);  // 用户数据
    pItem->setText(str);  // 文本

    QDomDocument q;

    dom_list.push_back(q);
    package_name_list.push_back(str);

    ui->listWidget->addItem(pItem);

    QTreeWidgetItem *item_root;
    item_root = new QTreeWidgetItem(ui->treeWidget);
    item_root->setText(0,"ACTOR");
    item_root->setBackgroundColor(0,QColor(200, 93, 12,255));
    //item->setText(1,QString("%1").arg(file_title));
    item_root->setBackgroundColor(1,QColor(200, 93, 12,255));


    QTreeWidgetItem *item;
    item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,"_P_NAME");
    item->setText(1, str);
}


QStringList templet_tag_lib;

QStringList new_tag_lib;




//查找新标签
bool Role::has_new_tagname(QDomDocument templet,QDomDocument temp)
{
    //open_check_xml("./base.templetxml");
//    open_check_xml_only(temp_memry);
    open_check_xml_only(templet);
    templet_tag_lib =temp_tagname;
    temp_tagname.clear();

    //open_check_xml_only("./dbxml_xml.xml");
    open_check_xml_only(temp);
    new_tag_lib=temp_tagname;
    temp_tagname.clear();

        for(int j=0;j<new_tag_lib.size();j++)
        {
            if(-1==templet_tag_lib.indexOf(new_tag_lib[j]))
            {
                return true;
            }
        }
    return false;
}
//独立dbxml存储
void Role::open_check_xml_only(QDomDocument dom)
{
        QDomElement docElem = dom.documentElement();
        check_new_tagname(docElem);
}

//打开 两个对比的xml文件
void Role::open_check_xml(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        //question
        if (dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();
            check_new_tagname(docElem);
        }
    }
    file.close();
}
//模板标签的存储
void Role::check_new_tagname(QDomElement& docElem)
{
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        temp_tagname.push_back(docElem.tagName());
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if( !element.isNull() )
        {
            temp_tagname.push_back(element.tagName());

            check_new_tagname(element);
        }
        node = node.nextSibling();
    }
    return;
}





