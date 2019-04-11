#ifndef XML_MERGE_H
#define XML_MERGE_H

#endif // XML_MERGE_H
#include <iostream>
#include <QtCore>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomComment>
#include <QFile>
#include <QtXml/qdom.h>
void XmlMergeFunc(QDomDocument &template_root , QDomDocument &platform_root);
void XmlMerge_Func(QDomElement &el_template , QDomElement &el_platform);
QString GetTagnameFullpath_func(const QDomElement &leaf);
void ClearTextValue(QDomElement &leaf);
bool MatchXmlTemplate(QDomElement &el_template , QDomElement &el_platform);
void XmlMergeFunc(QDomDocument &template_root , QDomDocument &platform_root)
{
    QDomElement el_template = template_root.firstChildElement();
    QDomElement el_platform = platform_root.firstChildElement();
    XmlMerge_Func(el_template,el_platform);
    template_root.clear();
    QDomProcessingInstruction instruction = template_root.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    template_root.appendChild(instruction);
    template_root.appendChild(el_template);
}
void XmlMerge_Func(QDomElement &el_template , QDomElement &el_platform)
{
    for(QDomNode p_node = el_platform.firstChild();(!p_node.isNull())&&p_node.isElement();p_node = p_node.nextSibling())
    {
        QDomElement el = p_node.toElement();
        qDebug() << el.tagName();
        QString tagname = p_node.nodeName();
        QDomNodeList nodelist = el_template.elementsByTagName(tagname);
        //the if code may have more consideration
        if(nodelist.count()==0&&!el.parentNode().isNull())
        {
            QString fathertagname = p_node.parentNode().nodeName();
            ClearTextValue(el);
            QDomNodeList t_nodelist = el_template.elementsByTagName(fathertagname);
            if(t_nodelist.count()!=0)
            {
                QDomElement e = t_nodelist.at(0).toElement();
                QDomNode oldnode = e;
                e.appendChild(el);
                QDomNode newnode = e;
                el_template.replaceChild(newnode,oldnode);
            }else
            {
                qDebug() << "something goes wrong!";
            }
        }else
        {
            XmlMerge_Func(el_template,el);
        }
    }
}
QString GetTagnameFullpath_func(const QDomElement &leaf)
{
  QString full_path = "";
  for(QDomNode node = leaf;!node.parentNode().parentNode().isNull();node = node.parentNode())
  {
      full_path = full_path.append("/").append(node.toElement().tagName());
  }
  return full_path;
}
void ClearTextValue(QDomElement &leaf)
{
    for(QDomNode node = leaf.firstChild();(!node.isNull())&&node.isElement();node = node.nextSibling())
    {
        QDomElement el = node.toElement();
        ClearTextValue(el);
    }
    QDomNode oldnode = leaf.firstChild();
    leaf.firstChild().setNodeValue("");
    QDomNode newnode = leaf.firstChild();
    leaf.replaceChild(newnode,oldnode);
}
bool MatchXmlTemplate(QDomElement &el_template , QDomElement &el_platform)
{
    bool result = true;
        for(QDomNode p_node = el_platform.firstChild();(!p_node.isNull())&&p_node.isElement();p_node = p_node.nextSibling())
        {
            QDomElement el = p_node.toElement();
            QString tagname = el.tagName();
            bool match = false;
            for(QDomNode t_node = el_template.firstChild();(!t_node.isNull())&&t_node.isElement();t_node = t_node.nextSibling())
            {
                if(t_node.nodeName()==tagname)
                {
                    QDomElement t_el = t_node.toElement();
                    result = MatchXmlTemplate(t_el,el);
                    if(result == false)
                    {
                        return false;
                    }
                }
            }
            if(match == false)
            {
                result = false;
            }
            return result;
        }
        return result;

}
