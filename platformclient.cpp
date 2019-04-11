#include "platformclient.h"
#include "xml_merge.h"
#include "ui_platformclient.h"
#include "dbxml_sql.h"
#include <QDesktopServices>
#include <QAction>
#include <QtGui/qevent.h>

using namespace std;
platformclient::platformclient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::platformclient)
{
    ui->setupUi(this);
    this->setWindowTitle("平台开发者");
    this->setWindowIcon(QIcon("../platformclient_new/ICO/xml.png"));

    //set backgroudcolor
    this->setStyleSheet(set_btn.style_color);

    //set hidden head
    ui->treeWidget_custom->setHeaderHidden(true);
    //隐藏边框线
    ui->treeWidget_custom->setStyleSheet("border:none");
    ui->listWidget->setStyleSheet("border:none");
    ui->valuemessage->setStyleSheet("border:none");


    //关闭载入模板功能按钮
    ui->pushButton->setHidden(true);

    //设置列宽
    ui->treeWidget_custom->setColumnWidth(0,250);

    //预加载数据库
    QStringList ID_NUM=w.query_ID("/_ARMER/_P_ID/string()","../platformclient_new/","default.dbxml");
    if(ID_NUM.isEmpty())
        {
        QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("当前路径下无默认数据库(default.dbxml)文件！"), QMessageBox::Ok);
        tmpMsgBox.exec();
    }
    int size=ID_NUM.size();
    for(int i=0;i<size;i++)
    {
        if(ID_NUM[i]!="templetxml")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            pItem->setText(ID_NUM[i]);  // 文本
            ui->listWidget->addItem(pItem);
        }
    }
    //从数据库中导出模板
    string text="templetxml";
    QString X=w.getDocument("../platformclient_new/","default.dbxml",text);


    temp_memry.setContent(X);
    //默认载入模板显示
    openXML_mem(temp_memry);

    //复选框
    ui->checkBox->setChecked(true);
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->pushButtonASfile,SIGNAL(clicked()),this,SLOT(slotSaveAs()));
    //显示右键菜单
    connect(ui->treeWidget_custom,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showRightMenu(QPoint)));
    connect(ui->listWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(list_showRightMenu(QPoint)));

    //设置treewidget 某一列可编辑
    connect(ui->treeWidget_custom,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(openEditor(QTreeWidgetItem*,int)));
    connect(ui->treeWidget_custom,SIGNAL(itemSelectionChanged()),
            this,SLOT(closeEditor()));

    //ICO
    ui->pushButtonOpen_2->setIcon(QIcon("./images/baocun (2).png"));
    ui->pushButtonOpen->setIcon(QIcon("./images/开发 (1).png"));
    ui->Cancel->setIcon(QIcon("./images/refresh.png"));
    ui->merge->setIcon(QIcon("./images/saveas.png"));
}

QDomDocument docm;
QDomDocument docm_temp;
//当前界面编辑内容
string docstring;
string ID_value;
string merge_templet;
QStringList P_value;
QString Now_Tagname;
QString Path;
QString templet_path;
int isnull=0;
int stage_checkbox=0;
//list 临时存储
string current_name="default.dbxml";
string current_path="../platformclient_new/";

bool platformclient::openXML(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        if (dom.setContent(&file))
        {
            ui->treeWidget_custom->clear();
            docm_temp=dom;
            QDomElement docElem = dom.documentElement();
            if(docElem.tagName()!="_ARMER")
            {
                file.close();
                return false;
            }
            listDom_custom(docElem,NULL);
        }
    }
    file.close();
}

bool platformclient::check(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom;
        if (dom.setContent(&file))
        {
            QDomElement docElem = dom.documentElement();
            if(docElem.tagName()=="_ARMER")
            {
                return true;
                file.close();
            }
            return false;
            file.close();
        }
    }
    file.close();
}

void platformclient::openFile()
{
    QFileInfo file;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "c:/", tr("XML Files (*.xml)"));

    file=QFileInfo(fileName);

    file_Name=file.fileName();//文件名
    file_Path=file.filePath();//文件路径
    //checkbox初始状态
    stage_checkbox=0;
    ui->checkBox->setCheckState(Qt::Checked);

    if(!fileName.isEmpty())
    {
        Path=fileName;
        qDebug()<<"test"<<fileName;
        if(openXML(fileName)!=true)
        {
            QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("不是标准的平台文件(未包含_ARMER标签)！"), QMessageBox::Ok);
            tmpMsgBox.exec();
            return;
        }
        if(has_new_tagname(temp_memry,docm_temp)==true)
        {
            stage_checkbox=1;
            ui->checkBox->setCheckState(Qt::Unchecked);
        }
    }
    if(check_save==0)
    {
        ui->label_3->setText("编辑"+file_Name+"*");
    }
}
// save as file
void platformclient::slotSaveAs()
{
    check_save=1;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As XML File"), "./custom_xml.xml", tr("XML Files (*.xml)"));
    QTreeWidgetItem *rootItem = ui->treeWidget_custom->invisibleRootItem(); //树的根节点
    int countGroup = rootItem->childCount();
    if (countGroup <= 0)  //列表树没有项
    {
        return;
    }
    QFile db(fileName);
    if (!db.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    //只写方式打开，并清空以前的信息
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("_ARMER");
    doc.appendChild(root); //添加根元素

    for (int i = 0; i < countGroup; i++)
    {
        QTreeWidgetItem *groupitem = rootItem->child(i);
        QString groupname = groupitem->text(0);
        QDomElement elementGrp = doc.createElement(groupname);
        root.appendChild(elementGrp);

        //ID
        if(groupitem->text(1)!="")
        {
            QDomText strMacNodeText = doc.createTextNode(groupitem->text(1));
            elementGrp.appendChild(strMacNodeText);
        }
        int countDev = groupitem->childCount();
        for (int j = 0; j < countDev; j++)
        {
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
            for (int k = 0; k < countChn; k++)
            {
                QTreeWidgetItem *channelitem = deviceitem->child(k);
                QString channelname = channelitem->text(0);
                QDomElement elementChn = doc.createElement(channelname);
                elementDev.appendChild(elementChn);

                if(channelitem->text(1)!="")
                {
                    QDomText strMacNodeText = doc.createTextNode(channelitem->text(1));
                    elementChn.appendChild(strMacNodeText);
                }
                int countChm = channelitem->childCount();
                for(int m=0;m < countChm;m++)
                {
                    QTreeWidgetItem *channelitem_m = channelitem->child(m);
                    QString channelname_m = channelitem_m->text(0);
                    if(channelitem_m->text(1)!="")
                    {
                        //bool a = channelname_m.startsWith("_P_");
                        if(!channelname_m.startsWith("_P_"))
                        {
                            channelname_m=QString("_P_")+channelname_m;
                        }
                    }
                    QDomElement elementChm = doc.createElement(channelname_m);
                    elementChn.appendChild(elementChm);
                    //添加文本
                    if(channelitem_m->text(1)!="")
                    {
                        QDomText strMacNodeText = doc.createTextNode(channelitem_m->text(1));
                        elementChm.appendChild(strMacNodeText);
                    }
                }
            }
        }
    }

    QTextStream out(&db);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    db.close();

    qDebug()<<"doc:"<<doc.toString();

    QMessageBox tmpMsgBox(QMessageBox::Information, QStringLiteral("提示"), QStringLiteral("保存Xml文件成功！"), QMessageBox::Ok);
    tmpMsgBox.exec();

    if(check_save==1)
    {
        ui->label_3->setText("编辑"+file_Name);
    }

}

platformclient::~platformclient()
{
    delete ui;
}
//topic 内容值存入QStringList中
QStringList topic_list;
string new_ID;
void platformclient::listDom(QDomElement& docElem)
{
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        if(docElem.tagName()=="_P_ID")
        {
            new_ID=docElem.text().toStdString();
        }
    }
    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if( !element.isNull() )
        {
            listDom(element);
        }
        node = node.nextSibling();
    }
    return;
}
/************************
 * custom
 ***********************/
void platformclient::listDom_custom(QDomElement& docElem, QTreeWidgetItem * pItem)
{
    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        pItem->setText (1, docElem.text());
        if(docElem.tagName()=="_P_TOPIC")
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
                item = new QTreeWidgetItem(ui->treeWidget_custom);
            }
            item->setText(0, element.tagName());
            //listDom(element, item);
            listDom_custom(element, item);
            if( pItem )
            {
                pItem->addChild(item);
            }
            else
            {
                ui->treeWidget_custom->addTopLevelItem(item);
            }
        }
        node = node.nextSibling();
    }
    return;
}

//获取parent message

QStandardItem* platformclient::getTopParent(QStandardItem* item)
{
    QStandardItem* secondItem = item;
    while(item->parent()!= 0)
    {
        secondItem = item->parent();
        item = secondItem;
    }
    if(secondItem->index().column() != 0)
    {
        //修改
        QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeWidget_custom->model());
        secondItem = model->itemFromIndex(secondItem->index().sibling(secondItem->index().row(),0));
    }
    return secondItem;
}
QModelIndex platformclient::getTopParent(QModelIndex itemIndex)
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

//点击树查看详情
QString top;
//原树点击取消
//存储到数据库中
void platformclient::on_Cancel_clicked()
{
    QTreeWidgetItem *rootItem = ui->treeWidget_custom->invisibleRootItem(); //树的根节点
    int countGroup = rootItem->childCount();
    if (countGroup <= 0)  //列表树没有项
    {
        return;
    }
    else{
        //暂时保存下文档编辑器中的xml文件
        slotSaveAs_dbxml();

        w.on_pushButton_clicked(QString::fromStdString(current_name),QString::fromStdString(current_path),docstring,ID_value);


        if(has_new_tagname(temp_memry,docm)==true)
        {
            switch (QMessageBox::warning(this,tr("发现新标签提示"),tr("是否合并到数据库模板中"),tr("yes"),tr("NO"),0,1))
            {
            case 0:
                //保存
                if(new_merge()==true)
                {
                    //./new_add_templet.xml
                    w.on_pushButton_clicked(QString::fromStdString(current_name),QString::fromStdString(current_path),merge_templet,"templetxml");
                }
                break;
            case 1:
                break;
            }
        }
        //更新左侧列表
        ui->listWidget->clear();
        //查  打印
        QStringList ID_num=w.query_ID("/_ARMER/_P_ID/string()",current_path,current_name);

        int size=ID_num.size();
        //存在问题
        // qDebug()<<"size"<<size;
        for(int i=0;i<size;i++)
        {
            if(ID_num[i]!="templetxml")
            {
                QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
                // pItem->setText(w.query_ID("/_ARMER/_P_ID/string()",path2DbEnv,theContainer));  // 文本

                pItem->setText(ID_num[i]);

                ui->listWidget->addItem(pItem);
            }
        }
    }

}
int stateflog=1;
//右键菜单
void platformclient::showRightMenu(QPoint pos)
{
    //读文件
    if(stateflog==1)
    {
        QMenu * m_grpMenu = new QMenu(ui->treeWidget_custom);
        QMenu * m_addAreaMenu = new QMenu(ui->treeWidget_custom);

        m_grpMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");
        m_addAreaMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");

        QSignalMapper *pMapper = new QSignalMapper();

        QAction *m_addAreaAction = new QAction(tr("增加_PLATFORM"),this);
        QAction *m_addDev = new QAction(tr("增加_SENSOR"),this);
        QAction *m_memberDelete = new QAction(tr("删除"),this);

        connect(m_addAreaAction,SIGNAL(triggered()),this,SLOT(addplatform()));
        connect(m_addDev,SIGNAL(triggered()),this,SLOT(addDev()));

        connect(m_memberDelete,SIGNAL(triggered()),this,SLOT(del()));

        //追综鼠标  点击其他空白区域取消当前
        QTreeWidgetItem *item=ui->treeWidget_custom->itemAt(pos);

        if(item == NULL)
        {
            ui->treeWidget_custom->clearSelection();
            if (m_addAreaMenu->isEmpty())
            {
                //增加区域
                m_addAreaMenu->addAction(m_addAreaAction);
                m_addAreaMenu->addAction(m_addDev);
            }
            //菜单出现的位置为当前鼠标的位置
            m_addAreaMenu->exec(QCursor::pos());
        }
        else if(ui->treeWidget_custom->currentItem()->childCount() > 0 || ui->treeWidget_custom->currentItem()->childCount() == 0)
        {
            //当前标签名字
            Now_Tagname=ui->treeWidget_custom->currentItem()->text(0);

            P_value.clear();

            foundTagname(Now_Tagname);

            if (m_grpMenu->isEmpty())
            {
                for(int i=0;i<P_value.size();i++)
                {
                    QAction *mulu = new QAction("添加 "+P_value[i],m_grpMenu);
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

        QMenu * m_grpMenu = new QMenu(ui->treeWidget_custom);
        QMenu * m_memberMenu = new QMenu(ui->treeWidget_custom);
        QMenu * m_addAreaMenu = new QMenu(ui->treeWidget_custom);

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

        QTreeWidgetItem *item=ui->treeWidget_custom->itemAt(pos);

        if(item == NULL)
        {
            //取消鼠标选中状态
            ui->treeWidget_custom->clearSelection();
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
        else if(ui->treeWidget_custom->currentItem()->childCount() > 0)
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
        else if(ui->treeWidget_custom->currentItem()->childCount() == 0)
        {
            //qDebug()<<"当前点击3"<<ui->treeWidget_custom->currentItem()->text(0);
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
//list右键菜单
void platformclient::list_showRightMenu(QPoint pos)
{
    QMenu * m_grpMenu = new QMenu(ui->listWidget);


    m_grpMenu->setStyleSheet("selection-background-color:rgb(52, 101, 164);");

    QAction *m_memberedit = new QAction(tr("编辑"),this);
    QAction *m_memberDelete = new QAction(tr("删除"),this);

    connect(m_memberedit,SIGNAL(triggered()),this,SLOT(edit_sql()));
    connect(m_memberDelete,SIGNAL(triggered()),this,SLOT(del_sql()));

    m_grpMenu->addAction(m_memberedit);
    m_grpMenu->addAction(m_memberDelete);
    m_grpMenu->exec(QCursor::pos());
}

//edit
void platformclient::edit_sql()
{
    ui->treeWidget_custom->clear();

    QListWidgetItem* item =ui->listWidget->currentItem();
    isnull=1;

    ui->label_3->setText("编辑数据库中"+item->text());


    string text=item->text().toStdString();
    QString X=w.getDocument(current_path,current_name,text);


    QString filename = "temp.xml";        //当前文件夹中

    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        QMessageBox::warning(this,"file write","can't open",QMessageBox::Yes);

    }
    QTextStream in(&file);
    in<<X;
   // qDebug()<<"X:"<<X;

    //解决双击不准的问题
    file.close();

    openXML(filename);

    //删除文件
    QFile file_remove(filename);
    if (file_remove.exists())
    {
        file_remove.remove();
    }

}
void platformclient::del_sql()
{
    QListWidgetItem* item =ui->listWidget->currentItem();

    string text=item->text().toStdString();
    w.delete_one_sql(current_path,current_name,text);
    //刷新

    //更新左侧列表
    ui->listWidget->clear();
    //查  打印
    QStringList ID_num=w.query_ID("/_ARMER/_P_ID/string()",current_path,current_name);

    int size=ID_num.size();

   // qDebug()<<"size"<<size;
    for(int i=0;i<size;i++)
    {
        if(ID_num[i]!="templetxml")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            // pItem->setText(w.query_ID("/_ARMER/_P_ID/string()",path2DbEnv,theContainer));  // 文本

            pItem->setText(ID_num[i]);

            ui->listWidget->addItem(pItem);
        }
    }
}

//添加相对应的选项
void platformclient::doClicked(const QString& Tag_name)
{
    // QMessageBox::information(this, "Clicked", Tag_name + " is clicked!");//显示被按下的名称。
    QTreeWidgetItem* item=ui->treeWidget_custom->currentItem();
    QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<Tag_name);
    item->addChild(A);
}

void platformclient::addplatform()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget_custom);
    newgroup->setText(0, QStringLiteral("_PLATFORM"));
}
void platformclient::addDev()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget_custom);
    newgroup->setText(0, QStringLiteral("_SENSOR"));
}
void platformclient::gripadd()
{
    QTreeWidgetItem* item=ui->treeWidget_custom->currentItem();
    QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<"costom_name");

    item->addChild(A);
}
void platformclient::del()
{
    QTreeWidgetItem* curritem=ui->treeWidget_custom->currentItem();
    if (curritem != NULL)
    {
        //全部项都可删除
        QTreeWidgetItem *curritemparent = curritem->parent();
        if (curritemparent == NULL)
        {
            int index = ui->treeWidget_custom->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget_custom->takeTopLevelItem(index);
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
            int index = ui->treeWidget_custom->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget_custom->takeTopLevelItem(index);
            }
        }
    }
}

void platformclient::onGrpMenuTriggered(QAction *action)
{
    if (action == m_grpAddOneMember)
    {//增加子目录
        //修改
        QTreeWidgetItem* item=ui->treeWidget_custom->currentItem();
        QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<"costom_name");

        item->addChild(A);
    }
    else if (action == m_grpDeleteGrp)
    {//删除目录
        //修改
        QTreeWidgetItem* curritem=ui->treeWidget_custom->currentItem();
        if (curritem != NULL)
        {
            //全部项都可删除
            QTreeWidgetItem *curritemparent = curritem->parent();
            if (curritemparent == NULL)
            {
                int index = ui->treeWidget_custom->indexOfTopLevelItem(curritem);
                if (index != -1)
                {
                    ui->treeWidget_custom->takeTopLevelItem(index);
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
                int index = ui->treeWidget_custom->indexOfTopLevelItem(curritem);
                if (index != -1)
                {
                    ui->treeWidget_custom->takeTopLevelItem(index);
                }
            }

        }
    }
}

void platformclient::onMemberMenuTriggered(QAction *action)
{
    if (action == m_grpAddOneMember)
    {//增加子目录
        QTreeWidgetItem* item=ui->treeWidget_custom->currentItem();
        QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<"custom_name");
        item->addChild(A);
    }
}

void platformclient::onAddAreaMenuTriggered(QAction *action)
{
    if (action == m_addAreaAction)
    {//增加平台
        QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget_custom);
        newgroup->setText(0, QStringLiteral("PLATFORM"));
    }
    if(action == m_addDev)
    {//增加传感器
        QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget_custom);
        newgroup->setText(0, QStringLiteral("SENSOR"));
    }
}
/*******************
 * 设置可编辑tree
 * ******************/
QTreeWidgetItem *temItem;
int temColumn;

void platformclient::openEditor(QTreeWidgetItem *item, int column)
{
    check_save=0;
    ui->treeWidget_custom->openPersistentEditor(item,column);// 设置某一item可以编辑
    temItem = item;
    temColumn = column;
   // qDebug()<<"column"<<column;
}

// 当修改完以后或者选中其他列的时候，关闭可编辑状态
void platformclient::closeEditor()
{
    //qDebug()<<"关闭该功能1";
    if(temItem != NULL)
    {
        //qDebug()<<"关闭该功能2";
        if(temItem->text(0)=="_TOPIC" || temItem->text(0)=="TOPIC" ||temItem->text(0)=="topic")
        {
            if(top != "_SENSOR")
            {
                QMessageBox WAR_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("不能在此添加topic"), QMessageBox::Ok);
                WAR_MsgBox.exec();
                temItem=NULL;
            }
            else if( -1!= topic_list.indexOf(temItem->text(1)))
            {
                QMessageBox WARNING_MsgBox(QMessageBox::Warning, QStringLiteral("WARNING"), QStringLiteral("请重新命名topic名"), QMessageBox::Ok);
                WARNING_MsgBox.exec();
                temItem=NULL;
            }
        }
        ui->treeWidget_custom->closePersistentEditor(temItem,temColumn);// 设置某一item不可编辑
    }
    //qDebug()<<"关闭该功能";
    // ui->treeWidget_custom->closePersistentEditor(temItem,temColumn);
    //归位
    temItem=NULL;
    temColumn=0;
}
void platformclient::on_merge_clicked()
{
    if(stateflog==2)
    {
        //保存当前
        slotSaveAs_dbxml();
        //合并模板
        if(merge()!=false)
        {
            QMessageBox tmpMsgBox(QMessageBox::Information, QStringLiteral("提示"), QStringLiteral("合并模板文件成功！"), QMessageBox::Ok);
            tmpMsgBox.exec();
        }
    }
    else
    {
        qDebug()<<"复选框为模板不需合并";
    }
}

//判断复选框状态
int platformclient::onStateChanged(int state)
{

    if (state == Qt::Checked) // "选中"
    {
        if(stage_checkbox==1)
        {

            ui->checkBox->setCheckState(Qt::Unchecked);
            QMessageBox tmpMsgBox(QMessageBox::Warning, QStringLiteral("错误提示"), QStringLiteral("平台文件包含模板以外的新标签！"), QMessageBox::Ok);
            tmpMsgBox.exec();
            return 0;
        }
        else
        {
            ui->checkBox->setText("使用模板");
            stateflog=1;
        }
    }
    // 未选中 - Qt::Unchecked
    else
    {
        //ui->checkBox->setText("自定义填加");
        stateflog=2;
    }
}

//find Now_Tagname
void platformclient::foundTagname(QString Tagname)
{
    //读文件
    open_temp_XML(temp_memry,Tagname);
    //qDebug()<<"templet_path:"<<templet_path;
    //自主选择
    //open_temp_XML(templet_path,Tagname);
}


//读模板
void platformclient::open_temp_XML(QDomDocument dom,QString Tagname)
{
            QDomElement docElem = dom.documentElement();
            listDom_temp(docElem);
}

void platformclient::listDom_temp(QDomElement docElem)
{

    QDomNode node = docElem.firstChild();
    if(node.toElement().isNull())
    {
        //pItem->setText (1, docElem.text());
        //qDebug()<<"测试里层"<<docElem.tagName();
    }
    if(node.parentNode().toElement().tagName()==Now_Tagname)
    {
        if(P_value.indexOf(node.toElement().tagName())!=-1)
        {
            qDebug()<<"========";
            return;
        }
        else{
            // P_value.push_back(node.toElement().tagName());
            //qDebug()<<"测试"<<node.toElement().tagName();
            //qDebug()<<"测试"<<node.toElement().nextSiblingElement().tagName();
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

bool platformclient::new_merge()
{
    QDomDocument document_platform;
    QDomDocument document_template;
    slotSaveAs_dbxml();
    document_platform=docm;
    document_template=temp_memry;

    XmlMergeFunc(document_template,document_platform);

    //自定义修改 2017-11-14
    /*
    QString fileName ="./new_add_templet.xml";

    QFile file1(fileName);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file1);
    out.setCodec("UTF-8");
    document_template.save(out, 4, QDomNode::EncodingFromTextStream);
    file1.close();
    */

    merge_templet=document_template.toString().toStdString();
       cout<<merge_templet<<endl;

    QFile file_remove("./tree_xml.xml");
    if (file_remove.exists())
    {
        file_remove.remove();
    }
}
//合并模板
bool platformclient::merge()
{
    QDomDocument document_platform;
    QDomDocument document_template;
    slotSaveAs_dbxml();
    document_platform=docm;
    document_template=temp_memry;

    XmlMergeFunc(document_template,document_platform);

    //自定义修改 2017-11-14
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As XML File"), "./custom_templet_xml.templetxml", tr("XML Files (*.templetxml)"));

    QFile file1(fileName);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file1);
    out.setCodec("UTF-8");
    document_template.save(out, 4, QDomNode::EncodingFromTextStream);
    file1.close();
}

//mach xml check has new tagname?
//查找新标签
bool platformclient::has_new_tagname(QDomDocument templet,QDomDocument temp)
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
void platformclient::open_check_xml_only(QDomDocument dom)
{
        QDomElement docElem = dom.documentElement();
        check_new_tagname(docElem);
}

//打开 两个对比的xml文件
void platformclient::open_check_xml(QString fileName)
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
void platformclient::check_new_tagname(QDomElement& docElem)
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


//临时保存当前编辑xml文件,获得document   string类型
void platformclient::slotSaveAs_dbxml()
{
    //询问是否保存
    check_save=1;
    QTreeWidgetItem *rootItem = ui->treeWidget_custom->invisibleRootItem(); //树的根节点
    int countGroup = rootItem->childCount();
    if (countGroup <= 0)  //列表树没有项
    {
        return;
    }
    //只写方式打开，并清空以前的信息
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("_ARMER");
    doc.appendChild(root); //添加根元素

    for (int i = 0; i < countGroup; i++)
    {
        QTreeWidgetItem *groupitem = rootItem->child(i);
        QString groupname = groupitem->text(0);
        QDomElement elementGrp = doc.createElement(groupname);
        root.appendChild(elementGrp);

        //ID
        if(groupitem->text(1)!="")
        {
            QDomText strMacNodeText = doc.createTextNode(groupitem->text(1));
            //保存ID_value
            ID_value=groupitem->text(1).toStdString();
            elementGrp.appendChild(strMacNodeText);
        }
        int countDev = groupitem->childCount();
        for (int j = 0; j < countDev; j++)
        {
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
            for (int k = 0; k < countChn; k++)
            {
                QTreeWidgetItem *channelitem = deviceitem->child(k);
                QString channelname = channelitem->text(0);
                QDomElement elementChn = doc.createElement(channelname);
                elementDev.appendChild(elementChn);

                if(channelitem->text(1)!="")
                {
                    QDomText strMacNodeText = doc.createTextNode(channelitem->text(1));
                    elementChn.appendChild(strMacNodeText);
                }
                int countChm = channelitem->childCount();
                for(int m=0;m < countChm;m++)
                {
                    QTreeWidgetItem *channelitem_m = channelitem->child(m);
                    QString channelname_m = channelitem_m->text(0);
                    if(channelitem_m->text(1)!="")
                    {
                        if(!channelname_m.startsWith("_P_"))
                        {
                            channelname_m=QString("_P_")+channelname_m;
                        }
                    }
                    QDomElement elementChm = doc.createElement(channelname_m);
                    elementChn.appendChild(elementChm);
                    //添加文本
                    if(channelitem_m->text(1)!="")
                    {
                        QDomText strMacNodeText = doc.createTextNode(channelitem_m->text(1));
                        elementChm.appendChild(strMacNodeText);
                    }
                }
            }
        }
    }
    docm=doc;
    //保存docstring
    docstring=doc.toString().toStdString();


    if(check_save==1)
    {
        ui->label_3->setText("编辑"+file_Name);
    }

}

void platformclient::on_action_New_Folder_triggered()
{
    openFile();
}

void platformclient::on_action_New_File_triggered()
{
    openFile();
}

void platformclient::on_save_as_triggered()
{
    slotSaveAs();
}

void platformclient::on_action_D_triggered()
{
    ui->valuemessage->clear();
}

void platformclient::on_refresh_triggered()
{
    ui->treeWidget_custom->clear();
    openXML(Path);
}

void platformclient::on_treeWidget_custom_clicked(const QModelIndex &index)
{
    QString str;

    //qDebug()<<"当前点击"<<index.data().toString();
    str += QStringLiteral("当前选中：%1\n").arg(index.data().toString());
    str += QStringLiteral("父级：%1\n").arg(index.parent().data().toString());

    //兄弟节点测试
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
    if(QStringLiteral("%1").arg(name)=="_P_SPEED")
    {
        str += QStringLiteral("属性名称：%1\n速度信息：%2 m/s(米/秒)\n").arg(name).arg(info);
    }
    if(QStringLiteral("%1").arg(name)=="_P_ENDURANCE")
    {
        str += QStringLiteral("属性名称：%1\n续航时间：%2 s(秒)\n").arg(name).arg(info);
    }
    if(QStringLiteral("%1").arg(name)=="_P_FRAME_RATE")
    {
        str += QStringLiteral("属性名称：%1\n帧率：%2 HZ(赫兹)\n").arg(name).arg(info);
    }
    if(QStringLiteral("%1").arg(name)=="_P_WEIGHT")
    {
        str += QStringLiteral("属性名称：%1\n质量：%2 kg(千克)\n").arg(name).arg(info);
    }
    else
    {
        str += QStringLiteral("属性名称：%1\n信息：%2\n").arg(name).arg(info);
    }
    //右边具体属性值显示
    QString str1,str2,str3;
    str1 = QStringLiteral("%1").arg(name);
    str2 = QStringLiteral("%1").arg(info);
    str3 = QStringLiteral("%1").arg(index.parent().data().toString());
    //寻找顶层   top=QStringLiteral("%1").arg(index.parent().data().toString());

    str += QStringLiteral("根目录名：%1\n\n").arg(top);
    str+= QStringLiteral("基本单位：\n  长度   米(m) \n  质量   千克(kg) \n  时间   秒(s) \n  电流   安培(A)");
    ui->valuemessage->setText(str);
}
//载入取消
void platformclient::on_pushButton_clicked()
{
    //载入模板
    //清空操作
    //打开模板文件
    //2017/11/17修改默认导入
    // QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML templet File"), "c:/", tr("XML Files (*.templetxml)"));
    //保存路径
    //后加
    QString fileName="./base.templetxml";

    templet_path=fileName;

    if(isnull==0)
    {

        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QDomDocument dom;
            //question
            if (dom.setContent(&file))
            {
                ui->treeWidget_custom->clear();
                QDomElement docElem = dom.documentElement();
                listDom_custom(docElem,NULL);
            }
        }
        file.close();
    }
}
//加载到界面--(取消)
void platformclient::load_templet()
{
    QString fileName="./base.templetxml";

    templet_path=fileName;

    if(isnull==0)
    {

        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QDomDocument dom;
            //question
            if (dom.setContent(&file))
            {
                ui->treeWidget_custom->clear();
                QDomElement docElem = dom.documentElement();
                listDom_custom(docElem,NULL);
            }
        }
        file.close();
    }
}

void platformclient::on_openfile_triggered()
{
    openFile();
}

QStringList member;
//导入数据库文件
void platformclient::on_pushButtonOpen_2_clicked()
{
    ui->listWidget->clear();

    //清空编辑栏目
    ui->treeWidget_custom->clear();
    ui->label_3->setText("XML文件编辑栏");

    QString fileName,filePath,filefullpath;
    QFileInfo file;
    QString file_message = QFileDialog::getOpenFileName(this, tr("打开 dbxml File"), "", tr("XML Files (*.dbxml)"));

    file=QFileInfo(file_message);
    fileName=file.fileName();
    filePath=file.absolutePath();//路径
    filefullpath=file.filePath();
    //当前名子,保存
    std::string path2DbEnv;
    std::string theContainer = fileName.toStdString();
    //当前路径
    path2DbEnv=filePath.toStdString();
    cout<<"path2DbEnv==="<<path2DbEnv<<endl;

    current_name=theContainer;
    current_path=path2DbEnv;

    cout<<"path2DbEnv:"<<path2DbEnv;

    //查  打印
    QStringList ID_num=w.query_ID("/_ARMER/_P_ID/string()",path2DbEnv,theContainer);

    int size=ID_num.size();

    for(int i=0;i<size;i++)
    {
        if(ID_num[i]!="templetxml")
        {
            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            pItem->setText(ID_num[i]);
            ui->listWidget->addItem(pItem);
        }
    }
}
//新建数据库+存储新的xml文件
void platformclient::on_edit_triggered()
{
    QString fileName,filePath,filefullpath;
    QFileInfo file;
    QString file_message =QFileDialog::getSaveFileName(this, tr("存储数据库文件"), "./yourName.dbxml", tr("DBXML Files (*.dbxml)"));

    file=QFileInfo(file_message);//文件信息
    fileName=file.fileName();//文件名字
    filePath=file.absolutePath();//路径
    filefullpath=file.filePath();

    QString get_file_path;
    QFileInfo get_file_info;

    QString get_file = QFileDialog::getOpenFileName(this, tr("选择要存储的xml File"), "c:/", tr("XML Files (*.xml)"));
    get_file_info=QFileInfo(get_file);

    //路径
    get_file_path=get_file_info.filePath();

    QFile file_new(get_file);
    file_new.open(QIODevice::ReadOnly);

    QDomDocument dom_new_xml;
    dom_new_xml.setContent(&file_new);

    string new_xml_sql=dom_new_xml.toString().toStdString();

    QDomElement new_docElem = dom_new_xml.documentElement();

    listDom(new_docElem);

    w.file_input(fileName,filePath,get_file_path,new_xml_sql,new_ID);
}

void platformclient::on_Help_triggered()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://172.20.208.84:9090/lvjianghao1/platform")));
}

void platformclient::on_listWidget_doubleClicked(const QModelIndex &index)
{

    ui->treeWidget_custom->clear();

    QListWidgetItem* item =ui->listWidget->currentItem();

    isnull=1;

    ui->label_3->setText("编辑数据库中"+item->text());

    string text=item->text().toStdString();
    QString X=w.getDocument(current_path,current_name,text);

    QDomDocument memry;

    memry.setContent(X);
    //双击显示树形结构
    openXML_mem(memry);
}

void platformclient::openXML_mem(QDomDocument dom)
{

            ui->treeWidget_custom->clear();
            QDomElement docElem = dom.documentElement();
            listDom_custom(docElem,NULL);
}


void platformclient::on_pushButtonOpen_clicked()
{
    isnull=1;
}

void platformclient::on_setting_triggered()
{
    setting *set_windows=new setting();
    connect(set_windows, SIGNAL(sendString(QString,int)), this, SLOT(lineEditGetTem(QString,int)));

    set_windows->show();
}

bool true_bool=true;

void platformclient::lineEditGetTem(QString str,int font_value)
{
    qDebug()<<"ppopopp"<<str<<font_value;
    set_btn.style_color=str;
    this->setStyleSheet(str);

    QFont font;
    font.setPointSize(font_value);

    ui->treeWidget_custom->setFont(font);

    if(str=="background-color: rgb(245,245,245);color: rgb(0,0,0);")
    {
        ui->toolBar->setStyleSheet("background-color: rgb(84,85,86);color: rgb(255,255,255);");
    }
    if(str=="background-color: rgb(46, 52, 54);color: rgb(255,255,255);")
    {
        ui->toolBar->setStyleSheet("background-color: rgb(46, 52, 54);");
    }
}



