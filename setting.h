#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

    QString style_color="background-color: rgb(46, 52, 54);color: rgb(255,255,255);";

    int font_value=11;

private:
    Ui::setting *ui;
signals:

    void sendString(QString,int);

private slots:
    void on_buttonBox_accepted();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

};

#endif // SETTING_H
