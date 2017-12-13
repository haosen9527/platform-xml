#ifndef CREATE_NEW_XML_H
#define CREATE_NEW_XML_H

#include <QDialog>
#include "role.h"

namespace Ui {
class create_new_xml;
}

class create_new_xml : public QDialog
{
    Q_OBJECT

public:
    explicit create_new_xml(QWidget *parent = 0);
    ~create_new_xml();

    QString custom_name;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::create_new_xml *ui;

signals:

    void sendString(QString);
};

#endif // CREATE_NEW_XML_H
