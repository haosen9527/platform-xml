#include "dbxml_sql.h"
#include <QFile>
#include <QFileDialog>

using namespace DbXml;
using namespace std;

void dbxml_sql::usage()
{
    std::cerr <<   "参数错误\n"

        << "\t-h <dbenv directory>" << std::endl;
    //不可以退出
    //exit( -1 );
}
QStringList ID;
int flog=0;
QStringList dbxml_sql::doQuery( XmlTransaction &txn, XmlManager &db, const XmlContainer &container, const std::string &query )
{
    std::string fullQuery = "collection('" + container.getName() + "')" + query;

    cout<<"container.getName():"<<container.getName()<<endl;

    try {
        std::cout << "Exercising query '" << fullQuery << "' " << std::endl;

        cout<<"测试"<<endl;

        std::cout << "Return to continue: ";
        //getc(stdin);

        XmlQueryContext context = db.createQueryContext();

        XmlResults results( db.query( txn, fullQuery, context) );
        XmlValue value;

        //清除数组
        ID.clear();

        while(results.next(value))
        {
            // Obtain the value as a string and print it to stdout

            //转化为QString
            ID.push_back(QString::fromStdString(value.asString()));

            std::cout <<"信息:"<< value.asString() << std::endl;

            flog++;
            cout<<"flog:"<<flog<<endl;

        }

        return ID;
        std::cout <<"test:"<< value << std::endl;

        std::cout << (unsigned int) results.size()
            << " objects returned for expression '"
            << fullQuery << "'\n" << std::endl;

    }
    catch(XmlException &e) {
        std::cerr << "Query " << fullQuery << " failed\n";
        std::cerr << e.what() << "\n";
        txn.abort();
    }
    catch(std::exception &e) {
        std::cerr << "Query " << fullQuery << " failed\n";
        std::cerr << e.what() << "\n";
        txn.abort();
    }
}

//Get a document from the container using the document name
QString dbxml_sql::doGetDocument( XmlTransaction &txn, XmlContainer &container, const std::string docname)
{

     qDebug()<<"333333333333333333333test============";
    try {
        std::cout << "Getting document '" << docname << "' from the container." << std::endl;
        std::cout << "Return to continue: ";
        //getc(stdin);
        std::cout << "\n";

        //Get the document from the container using the document name
        XmlDocument theDocument = container.getDocument(txn, docname);
        std::string content;
        std::cout << "Document name: " << theDocument.getName() << std::endl;
        std::cout << theDocument.getContent(content) << std::endl;
        QString XX=QString::fromStdString(theDocument.getContent(content));

        return XX;

    }
    //Catches XmlException
    catch(std::exception &e) {
        std::cerr << "Get document from container failed.\n";
        std::cerr << e.what() << "\n";
        txn.abort();
        //exit( -1 );
    }
}



dbxml_sql::dbxml_sql(QWidget *parent)
{

}
dbxml_sql::~dbxml_sql()
{

}

string str_xml;
// input xmlfile at dbxml
//存数据库文件
void dbxml_sql::file_input(QString fileName,QString filePath,QString get_file_path,string docstring,string ID_value)
{

    std::string path2DbEnv;
    //当前路径
    path2DbEnv=filePath.toStdString();

    if (!path2DbEnv.length())
        {
        usage();
        return;
        }

    //string content=str_xml;
    string containerName = fileName.toStdString();

    //选择存储的文件
    //choose_xmlfile(get_file_path);

    std::string content = docstring;//内容


    std::string docName = ID_value;//ID

    string environmentDir =path2DbEnv;//文件路径

    u_int32_t envFlags = DB_RECOVER|DB_CREATE|DB_INIT_MPOOL|
            DB_INIT_LOCK|DB_INIT_TXN|DB_INIT_LOG;
    // Berkeley DB cache size (1 MB).  The default is quite small
    u_int32_t envCacheSize = 1*1024*1024;

    int dberr;
        DB_ENV *dbEnv = 0;
        dberr = db_env_create(&dbEnv, 0);
        if (dberr == 0) {
            dbEnv->set_cachesize(dbEnv, 0, envCacheSize, 1);
            //dbEnv->set_errcall(dbEnv, errcall); // set error callback
            dbEnv->set_lk_detect(dbEnv, DB_LOCK_DEFAULT); // handle deadlocks
            dberr = dbEnv->open(dbEnv, environmentDir.c_str(), envFlags, 0);
        }
        if (dberr) {
            std::cout << "Unable to create environment handle due to the following error: " <<
                db_strerror(dberr) << std::endl;
            if (dbEnv) dbEnv->close(dbEnv, 0);
            return;
        }
            XmlManager mgr(dbEnv, DBXML_ADOPT_DBENV);

            // Because the container will exist on disk, remove it
            // first if it exists
            if (mgr.existsContainer(containerName))
                mgr.removeContainer(containerName);

            XmlContainerConfig config;
            config.setTransactional(true);
            XmlContainer cont = mgr.createContainer(
                containerName,
                config);

            XmlUpdateContext uc = mgr.createUpdateContext();


            cont.putDocument(docName, content, uc);


            XmlDocument doc = cont.getDocument(docName);

            std::string docContent;
            std::string name = doc.getName();

            docContent = doc.getContent(docContent);

            // print it
            std::cout << "Document name: " << name << "\nContent: " << docContent << std::endl;
            cout<<"success========================================================="<<endl;
}

//删除单个数据库文件
void dbxml_sql::delete_one_sql(string path,string filename,string key)
{
    std::string path2DbEnv=path;
    std::string theContainer =filename;

    if (! path2DbEnv.length() )
    {
    usage();
    return;
    }

    // Berkeley DB environment flags
    u_int32_t envFlags = DB_RECOVER|DB_CREATE|DB_INIT_MPOOL|
        DB_INIT_LOCK|DB_INIT_TXN|DB_INIT_LOG;
    // Berkeley DB cache size (64 MB).  The default is quite small
    u_int32_t envCacheSize = 1*1024*1024;

    // Create and open a Berkeley DB Transactional Environment.
    int dberr;
    DB_ENV *dbEnv = 0;
    dberr = db_env_create(&dbEnv, 0);
    if (dberr == 0) {
        dbEnv->set_cachesize(dbEnv, 0, envCacheSize, 1);
        dberr = dbEnv->open(dbEnv, path2DbEnv.c_str(), envFlags, 0);
    }
    if (dberr) {
        std::cout << "Unable to create environment handle due to the following error: " <<
            db_strerror(dberr) << std::endl;
        if (dbEnv) dbEnv->close(dbEnv, 0);
    }

    XmlManager db(dbEnv, DBXML_ADOPT_DBENV);

    //Configure the container to use transactions
    XmlContainerConfig config;
    config.setTransactional(true);

    //Open a container in the db environment
    XmlContainer container = db.openContainer(theContainer, config);

    XmlUpdateContext uc = db.createUpdateContext();

    container.deleteDocument(key,uc);

}

//选择xml文件追加到数据库文件中
void dbxml_sql::on_pushButton_clicked(QString fileName, QString filePath,string docstring,string ID_value)
{
    std::string path2DbEnv;
    std::string theContainer = fileName.toStdString();//文件名
    //当前路径
    path2DbEnv=filePath.toStdString();
    cout<<"path2DbEnv==="<<path2DbEnv<<endl;

    if (!path2DbEnv.length() )
    {
    usage();
    return;
    }
    // Berkeley DB environment flags
    u_int32_t envFlags = DB_RECOVER|DB_CREATE|DB_INIT_MPOOL|
            DB_INIT_LOCK|DB_INIT_TXN|DB_INIT_LOG;
    u_int32_t envCacheSize = 1*1024*1024;

    // Create and open a Berkeley DB Transactional Environment.
    int dberr;
    DB_ENV *dbEnv = 0;
    dberr = db_env_create(&dbEnv, 0);
    if (dberr == 0) {
        dbEnv->set_cachesize(dbEnv, 0, envCacheSize, 1);
        dberr = dbEnv->open(dbEnv, path2DbEnv.c_str(), envFlags, 0);
    }
    if (dberr) {
        std::cout << "无法创建,错误: " <<db_strerror(dberr) << std::endl;
        if (dbEnv) dbEnv->close(dbEnv, 0);

        cout<<"no such";
        return;
    }

    XmlManager db(dbEnv, DBXML_ADOPT_DBENV);

    //Configure the container to use transactions
    XmlContainerConfig config;
    config.setTransactional(true);

    //Open a container in the db environment
    XmlContainer container = db.openContainer(theContainer, config);


    std::string document1 = docstring;


    //Add the documents
    XmlDocument myXMLDoc = db.createDocument();

    myXMLDoc.setContent( document1 );

    XmlUpdateContext uc = db.createUpdateContext();

     try
     {
    container.putDocument(ID_value, docstring, uc);

    cout<<"success========================================================="<<endl;
    }
    catch(XmlException &e)
    {
        if(e.what()=="Error: Document exists: "+ID_value)
        {
            cout<<"已存在......正在删除....重新生成"<<endl;
                container.deleteDocument(ID_value,uc);

                container.putDocument(ID_value, docstring, uc);
        }
        else
        {
             container.putDocument(ID_value, docstring, uc);
        }
    }

}

QStringList is_null;


QStringList dbxml_sql::query_ID(string condition,string path2DbEnv,string theContainer)
{
    if (!path2DbEnv.length() )
    {
        usage();
        is_null.clear();
        return is_null;
    }

    // Berkeley DB environment flags
    u_int32_t envFlags = DB_RECOVER|DB_CREATE|DB_INIT_MPOOL|
            DB_INIT_LOCK|DB_INIT_TXN|DB_INIT_LOG;
    u_int32_t envCacheSize = 1*1024*1024;

    // Create and open a Berkeley DB Transactional Environment.
    int dberr;
    DB_ENV *dbEnv = 0;
    dberr = db_env_create(&dbEnv, 0);
    if (dberr == 0) {
        dbEnv->set_cachesize(dbEnv, 0, envCacheSize, 1);
        dberr = dbEnv->open(dbEnv, path2DbEnv.c_str(), envFlags, 0);
    }
    if (dberr) {
        std::cout << "无法创建,错误: " <<db_strerror(dberr) << std::endl;
        if (dbEnv) dbEnv->close(dbEnv, 0);

        cout<<"no such";
        //return ;
    }

    XmlManager db(dbEnv, DBXML_ADOPT_DBENV);

    XmlContainerConfig config;
    config.setTransactional(true);
    QStringList temp;
    temp.clear();

    try
    {
        //open dbxmlfile
        XmlContainer container = db.openContainer(theContainer, config);

        //Create a transaction
        XmlTransaction txn = db.createTransaction();

        //perform the queries
        //查找
        temp=doQuery( txn, db, container, condition);

        //提交
        cout<<"==========================temp_size:==========================="<<temp.size()<<endl;

        txn.commit();

        cout<<"=================temp_size:==========="<<temp.size();
        return temp;
    }
    catch(XmlException &e)
    {
        return temp;
    }
}

QString dbxml_sql::getDocument(string path,string filename,string key)
{

        std::string path2DbEnv=path;
        std::string theContainer =filename;

        if (! path2DbEnv.length() )
        {
        usage();
        return NULL;
        }

        // Berkeley DB environment flags
        u_int32_t envFlags = DB_RECOVER|DB_CREATE|DB_INIT_MPOOL|
            DB_INIT_LOCK|DB_INIT_TXN|DB_INIT_LOG;
        // Berkeley DB cache size (64 MB).  The default is quite small
        u_int32_t envCacheSize = 1*1024*1024;

        // Create and open a Berkeley DB Transactional Environment.
        int dberr;
        DB_ENV *dbEnv = 0;
        dberr = db_env_create(&dbEnv, 0);
        if (dberr == 0) {
            dbEnv->set_cachesize(dbEnv, 0, envCacheSize, 1);
            dberr = dbEnv->open(dbEnv, path2DbEnv.c_str(), envFlags, 0);
        }
        if (dberr) {
            std::cout << "Unable to create environment handle due to the following error: " <<
                db_strerror(dberr) << std::endl;
            if (dbEnv) dbEnv->close(dbEnv, 0);
        }

        //Have the XmlManager adopt the db environment
        XmlManager mgr(dbEnv, DBXML_ADOPT_DBENV);

        //Configure the container to use transactions
        XmlContainerConfig config;
        config.setTransactional(true);

        //Open a container in the db environment
        XmlContainer container = mgr.openContainer(theContainer, config);

        //create a transaction
        XmlTransaction txn = mgr.createTransaction();

         qDebug()<<"+111111111111111  test============";

        //Get the document from the container using the document name
        QString temp =doGetDocument(txn, container, key);
        //commit the transaction

        qDebug()<<"+====================test============";
        txn.commit();

        return temp;

}


