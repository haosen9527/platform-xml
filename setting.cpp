#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");
    this->setStyleSheet(style_color);
    ui->spinBox->setValue(font_value);
}

setting::~setting()
{
    delete ui;
}
void setting::on_buttonBox_accepted()
{
    font_value=ui->spinBox->value();
    emit sendString(style_color,font_value);
    this->close();
}

void setting::on_radioButton_clicked()
{
    style_color="background-color: rgb(245,245,245);color: rgb(0,0,0);";
}

void setting::on_radioButton_2_clicked()
{
    style_color="background-color: rgb(46, 52, 54);color: rgb(255,255,255);";
}
