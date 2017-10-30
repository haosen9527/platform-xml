#include "role.h"
#include "ui_role.h"

using namespace std;

Role::Role(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Role)
{
    ui->setupUi(this);

    this->setWindowTitle("角色开发者平台");
    //设置列宽
    ui->treeWidget->setColumnWidth(0, 200);
    ui->treeWidget->setAlternatingRowColors(true);//上色
    ui->treeWidget->setAutoFillBackground(true);
    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    //list可拖动
    ui->listWidget->setDragEnabled(true);

    //左边 文件浏览
    dirModel=new QDirModel;
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirModel->index("/home/haosen/桌面"));

    //ui->treeView->setRootIndex(dirModel->index(QDir::currentPath()));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);


    connect(ui->pushButton_open, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->button_ok,SIGNAL(clicked()),this,SLOT(ok()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showRightMenu()));
    connect(ui->treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(view_showRightMenu()));
    connect(ui->openpackage,SIGNAL(clicked()),this,SLOT(active_add()));

    //活动图标显示按钮

    //设置treewidget可编辑
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(openEditor(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(change(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this,SLOT(closeEditor(QTreeWidgetItem*,int)));
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(get_item(QListWidgetItem*)));
}

int count_flog=1;
int bool_flog=0;
//QString role[10];
QString path[10]="";

QStringList Refresh;

QListWidgetItem* test;
int nID;
int temp=0;
QString top;

/********************
 * 打开文件
 ********************/
void Role::openXML(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom("WCM");
        if (dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();
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
        openXML( fileName );
    }
}
bool Role::check(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom("WCM");
        //????
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
        pItem->setText (1, docElem.text());
        //topic test
        qDebug()<<"docElem.text()"<<docElem.tagName();
        if(docElem.tagName()=="_BIND")
        {
            topic_list.push_back(docElem.text());
        }
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if( !element.isNull() )
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
            item->setText(0, element.tagName());
            listDom(element, item);
            if( pItem )
            {
                pItem->addChild(item);
            }
            else
            {
                ui->treeWidget->addTopLevelItem(item);
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
        root.appendChild(elementGrp);

        int countDev = groupitem->childCount();
        //  角色名字
        if(groupitem->text(1)!="")
        {
            QDomText strMacNode = doc.createTextNode(groupitem->text(1));
            elementGrp.appendChild(strMacNode);
        }
        for (int j = 0; j < countDev; j++)
        {
            QTreeWidgetItem *deviceitem = groupitem->child(j);
            QString devicename = deviceitem->text(0);
            QDomElement elementDev;

            if(deviceitem->text(0)=="bind" ||deviceitem->text(0)=="BIND")
            {
                devicename="_P_BIND";
            }

            if(deviceitem->text(1)!="")
            {
                if(!devicename.startsWith("_P_"))
                {
                    devicename=QString("_P_")+devicename;
                }
                elementDev = doc.createElement(devicename);
                elementGrp.appendChild(elementDev);

                QDomText strMacNodeText_1 = doc.createTextNode(deviceitem->text(1));
                elementDev.appendChild(strMacNodeText_1);
            }
            else
            {
                elementDev = doc.createElement(devicename);
                elementGrp.appendChild(elementDev);
            }
            int countChn = deviceitem->childCount();
            for (int k = 0; k < countChn; k++)
            {
                QTreeWidgetItem *channelitem = deviceitem->child(k);
                QString channelname = channelitem->text(0);
                QDomElement elementChn = doc.createElement(channelname);
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
                    if(channelitem_m->text(1)!="")
                    {
                        //如果有_P_不用添加
                        if(!channelname_m.startsWith("_P_"))
                        {
                            channelname_m=QString("_P_")+channelname_m;
                        }
                        //判断如果前面为大于
                        if(fuhao.startsWith(">"))
                        {

                            QDomElement elementChm = doc.createElement(channelname_m);
                            elementChn.appendChild(elementChm);


                            QDomElement con = doc.createElement("_LARGER_THAN");
                            elementChm.appendChild(con);
                            //截取符号输出
                            fuhao=fuhao.mid(1, fuhao.length()-1);

                            QDomText strMacNodeText = doc.createTextNode(fuhao);
                            con.appendChild(strMacNodeText);
                        }
                        //判断
                        else if(fuhao.startsWith("<"))
                        {

                            QDomElement elementChm = doc.createElement(channelname_m);
                            elementChn.appendChild(elementChm);

                            QDomElement con1 = doc.createElement("_SMALLER_THAN");
                            elementChm.appendChild(con1);
                            //截取符号输出
                            fuhao=fuhao.mid(1, fuhao.length()-1);

                            QDomText strMacNodeText = doc.createTextNode(fuhao);
                            con1.appendChild(strMacNodeText);
                        }
                        else
                        {
                            QDomElement elementChm = doc.createElement(channelname_m);
                            elementChn.appendChild(elementChm);


                            QDomText strMacNodeText = doc.createTextNode(channelitem_m->text(1));
                            elementChm.appendChild(strMacNodeText);
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
 * 清空操作
**************/
void Role::cancel()
{
    ui->treeWidget->clear();
}
Role::~Role()
{
    delete ui;
}

/*************
 * 右键菜单
**************/
void Role::showRightMenu()
{
    QMenu * m_grpMenu = new QMenu(ui->treeWidget);
    QMenu * m_memberMenu = new QMenu(ui->treeWidget);
    QMenu * m_addAreaMenu = new QMenu(ui->treeWidget);

    QAction *m_grpAddOneMember = new QAction(tr("增加子目录"),this);
    QAction *m_memberDelete = new QAction(tr("删除"),this);
    QAction *m_addAreaAction = new QAction(tr("增加平台"),this);
    QAction *m_addDev = new QAction(tr("增加传感器"),this);
    QAction *m_grpFlush = new QAction(tr("刷新列表"),this);

    connect(m_addAreaAction,SIGNAL(triggered()),this,SLOT(addplatform()));
    connect(m_addDev,SIGNAL(triggered()),this,SLOT(addDev()));
    connect(m_grpAddOneMember,SIGNAL(triggered()),this,SLOT(gripadd()));
    connect(m_memberDelete,SIGNAL(triggered()),this,SLOT(del()));

    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    if(item == NULL)
    {
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
        if (m_grpMenu->isEmpty())
        {
            m_grpMenu->addAction(m_addAreaAction);
            m_grpMenu->addAction(m_addDev);

            //增加子区域
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
void Role::addplatform()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget);
    newgroup->setText(0, QStringLiteral("PLATFORM"));
}
void Role::addDev()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget);
    newgroup->setText(0, QStringLiteral("SENSOR"));
}
void Role::gripadd()
{
    QTreeWidgetItem* item=ui->treeWidget->currentItem();
    //qDebug()<<"123";
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
 * 设置可编辑tree
 * ******************/
//(后期改)全局变量
QTreeWidgetItem *temitem;
int temColumn;

void Role::change(QTreeWidgetItem *item, int column)
{
    temitem = item;
    temColumn = column;
}

void Role::openEditor(QTreeWidgetItem *item, int column)
{
    ui->treeWidget->openPersistentEditor(item,column);// 设置某一item可以编辑
}
// 当修改完以后或者选中其他列的时候，关闭可编辑状态
void Role::closeEditor(QTreeWidgetItem *item, int column)
{
    if(item != NULL)
    {
        if(item->text(0)=="_BIND" || item->text(0)=="BIND" ||item->text(0)=="bind")
        {
            if(top != "_SENSOR")
            {
                QMessageBox WAR_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("不能在此添加bind"), QMessageBox::Ok);
                WAR_MsgBox.exec();
            }
            else if( -1!= topic_list.indexOf(item->text(1)))
            {
                QMessageBox WARNING_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("请重新命名bind名"), QMessageBox::Ok);
                WARNING_MsgBox.exec();
            }
        }
        ui->treeWidget->closePersistentEditor(temitem,temColumn);// 设置某一item不可编辑
    }
}


/******************
*table list
*
*******************/
void Role::active_add()
{
    openFile_top();
    // count_flog++;
}

void Role::openXML_top(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom("WCM");
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "c:/", tr("XML Files (*.xml)"));
    if(!check(fileName))
    {
        QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("不是标准的角色文件(未包含_ACTOR标签)！"), QMessageBox::Ok);
        tmpMsgBox.exec();
        return;
    }
    path[count_flog]=fileName;
    if(!fileName.isEmpty())
    {
        openXML_top( fileName );
    }
}
//动态导入添加
void Role::listDom_top(QDomElement& docElem, QTreeWidgetItem * pItem)
{
    QDomNode node = docElem.firstChild();

    if(node.toElement().isNull())
    {
        pItem->setText (1, docElem.text());
        if(docElem.tagName()=="_NAME")
        {
            count_flog++;
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            pItem->setData(Qt::UserRole, count_flog);  // 用户数据
            pItem->setText(docElem.text());  // 文本
            ui->listWidget->addItem(pItem);

        }
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if( !element.isNull() )
        {
            QTreeWidgetItem *item;
            if( pItem )
            {
                item = new QTreeWidgetItem(pItem);
            }
            else
            {
                item = new QTreeWidgetItem();
            }
            listDom_top(element, item);
            if( pItem )
            {
                pItem->addChild(item);
            }
        }

        node = node.nextSibling();
    }
    return;
}

int listnum=0;


void Role::get_item(QListWidgetItem* item)
{
    nID = item->data(Qt::UserRole).toInt()-1;  // 获取用户数据

   // qDebug()<<"nID===:"<<nID;
}
//删除
void Role::on_pushButton_clicked()
{
    QListWidgetItem *test1 =ui->listWidget->currentItem();
    if(test1==NULL || ui->listWidget->count()==0)
    {
        QMessageBox WARNING_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("未选中删除选项"), QMessageBox::Ok);
        WARNING_MsgBox.exec();
    }

   listnum=test1->data(Qt::UserRole).toInt()-1;
   QString pathname=path[listnum];

    temp=ui->listWidget->count();
    if(temp>=1)
    {
        ui->listWidget->removeItemWidget(test1);
        delete test1;
    }

    Refresh.removeAt(Refresh.indexOf(pathname));
    //刷新
    ui->treeWidget->clear();


    for(int i=0;i<Refresh.size();i++)
    {
        openXML(Refresh[i]);
    }
}
//清空
void Role::on_pushButton_2_clicked()
{
    count_flog=1;
    ui->listWidget->clear();
}
//int rightlist=0;
//添加
void Role::on_pushButton_3_clicked()
{
    temp=ui->listWidget->count();

   // qDebug()<<"nID:"<<nID;

    if(temp>=1)
    {
        openXML(path[nID]);
        Refresh.push_back(path[nID]);
    }
    else
    {
        return;
    }
}


void Role::view_showRightMenu()
{
    qDebug()<<"进入";
    QMenu * RightMenu = new QMenu(ui->treeView);

    QAction *add_next = new QAction(tr("添加"),this);

    RightMenu->addAction(add_next);

    RightMenu->exec(QCursor::pos());
}
