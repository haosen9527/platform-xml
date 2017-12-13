#include "create_new_xml.h"
#include "ui_create_new_xml.h"
#include "ui_role.h"

create_new_xml::create_new_xml(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_new_xml)
{
    ui->setupUi(this);
}

create_new_xml::~create_new_xml()
{
    delete ui;
}

void create_new_xml::on_buttonBox_accepted()
{
    custom_name=ui->textEdit->toPlainText();

    emit sendString(custom_name);

    this->close();
//    ui->textEdit->setText("please input actor_name");
//    ui->textEdit->setTextColor(QColor(20, 80, 100,255));
}
