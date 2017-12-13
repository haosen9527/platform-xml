#ifndef DBXML_SQL_H
#define DBXML_SQL_H

//#include <dbxml/DbXml.hpp>
#include <QWidget>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <QDebug>
#include <typeinfo>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QListWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

#include <dbxml/DbXml.hpp>
#include <db.h>

using namespace DbXml;
using namespace std;

namespace Ui {
class dbxml_sql;
}

class dbxml_sql
{

public:
    explicit dbxml_sql(QWidget *parent = 0);
    ~dbxml_sql();

    void file_input(QString fileName, QString filePath, QString, string docstring, string ID_value);

    QStringList query_ID(string condition,string path2DbEnv,string theContainer);

    void usage();
    QStringList doQuery(XmlTransaction &txn, XmlManager &db, const XmlContainer &container, const std::string &query );

    void loading_sql();

    void delete_one_sql(string path,string filename,string key);

    void on_pushButton_clicked(QString fileName, QString filePath, string docstring, string ID_value);

    QString doGetDocument( XmlTransaction &txn, XmlContainer &container, const std::string docname);

    QString getDocument(string path,string filename,string key);

private:
    Ui::dbxml_sql *ui;
};

#endif // DBXML_SQL_H
