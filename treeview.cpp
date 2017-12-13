#include "treeview.h"
#include "ui_treeview.h"

treeview::treeview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::treeview)
{
    ui->setupUi(this);
}

treeview::~treeview()
{
    delete ui;
}


void treeview::openXML_mem(QDomDocument dom)
{

            ui->treeWidget->clear();
            QDomElement docElem = dom.documentElement();
            listDom(docElem,NULL);
}



void treeview::listDom(QDomElement& docElem, QTreeWidgetItem * pItem)
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
