/********************************************************************************
** Form generated from reading UI file 'role.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROLE_H
#define UI_ROLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Role
{
public:
    QAction *action_templte;
    QAction *action_new_actor;
    QAction *action_tree;
    QAction *action_HELP;
    QAction *action_edit;
    QAction *action_copy;
    QAction *action_openpackage;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidget_sql;
    QLabel *label_2;
    QListWidget *listWidget;
    QPushButton *openpackage;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_add;
    QPushButton *pushButton;
    QPushButton *pushButton_clear;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_edit;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_open;
    QPushButton *create_new_file;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *button_ok;
    QPushButton *pushButton_cancel;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *Role)
    {
        if (Role->objectName().isEmpty())
            Role->setObjectName(QStringLiteral("Role"));
        Role->resize(928, 756);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Role->sizePolicy().hasHeightForWidth());
        Role->setSizePolicy(sizePolicy);
        Role->setStyleSheet(QLatin1String("border-color: rgb(164, 0, 0);\n"
"background-color: rgb(46, 52, 54);\n"
"color: rgb(255,255,255);\n"
"border-top-color: rgb(0, 0, 0);"));
        action_templte = new QAction(Role);
        action_templte->setObjectName(QStringLiteral("action_templte"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ICO/\346\250\241\346\235\277.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_templte->setIcon(icon);
        action_new_actor = new QAction(Role);
        action_new_actor->setObjectName(QStringLiteral("action_new_actor"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ICO/\347\251\272\347\231\275\351\241\265.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_new_actor->setIcon(icon1);
        action_tree = new QAction(Role);
        action_tree->setObjectName(QStringLiteral("action_tree"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ICO/\346\240\221\345\236\213.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_tree->setIcon(icon2);
        action_HELP = new QAction(Role);
        action_HELP->setObjectName(QStringLiteral("action_HELP"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ICO/\345\270\256\345\212\251\351\241\265.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_HELP->setIcon(icon3);
        action_edit = new QAction(Role);
        action_edit->setObjectName(QStringLiteral("action_edit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ICO/\345\242\236\345\212\240\350\241\214.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_edit->setIcon(icon4);
        action_copy = new QAction(Role);
        action_copy->setObjectName(QStringLiteral("action_copy"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ICO/\347\262\230\350\264\264.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_copy->setIcon(icon5);
        action_openpackage = new QAction(Role);
        action_openpackage->setObjectName(QStringLiteral("action_openpackage"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/ICO/\344\273\243\347\240\201.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_openpackage->setIcon(icon6);
        centralWidget = new QWidget(Role);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_4 = new QHBoxLayout(centralWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("background-color: rgb(85, 87, 83);"));

        verticalLayout->addWidget(label);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        listWidget_sql = new QListWidget(centralWidget);
        listWidget_sql->setObjectName(QStringLiteral("listWidget_sql"));
        listWidget_sql->setMaximumSize(QSize(250, 16777215));
        listWidget_sql->setContextMenuPolicy(Qt::CustomContextMenu);

        verticalLayout_2->addWidget(listWidget_sql);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(85, 87, 83);"));

        verticalLayout_2->addWidget(label_2);

        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMaximumSize(QSize(250, 16777215));
        QFont font;
        font.setPointSize(15);
        listWidget->setFont(font);

        verticalLayout_2->addWidget(listWidget);

        openpackage = new QPushButton(centralWidget);
        openpackage->setObjectName(QStringLiteral("openpackage"));

        verticalLayout_2->addWidget(openpackage);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_add = new QPushButton(centralWidget);
        pushButton_add->setObjectName(QStringLiteral("pushButton_add"));

        horizontalLayout_2->addWidget(pushButton_add);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_clear = new QPushButton(centralWidget);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));

        horizontalLayout_2->addWidget(pushButton_clear);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_edit = new QLabel(centralWidget);
        label_edit->setObjectName(QStringLiteral("label_edit"));
        label_edit->setStyleSheet(QStringLiteral("background-color: rgb(85, 87, 83);"));

        verticalLayout_3->addWidget(label_edit);

        treeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setBackground(1, QColor(85, 87, 83));
        __qtreewidgetitem->setBackground(0, QColor(85, 87, 83));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->setMinimumSize(QSize(600, 600));
        QFont font1;
        font1.setPointSize(11);
        font1.setStyleStrategy(QFont::PreferAntialias);
        treeWidget->setFont(font1);
        treeWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        treeWidget->setMouseTracking(true);
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        treeWidget->setStyleSheet(QStringLiteral(""));
        treeWidget->setLineWidth(0);
        treeWidget->setMidLineWidth(0);
        treeWidget->header()->setHighlightSections(false);

        verticalLayout_3->addWidget(treeWidget);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_open = new QPushButton(centralWidget);
        pushButton_open->setObjectName(QStringLiteral("pushButton_open"));

        horizontalLayout->addWidget(pushButton_open);

        create_new_file = new QPushButton(centralWidget);
        create_new_file->setObjectName(QStringLiteral("create_new_file"));

        horizontalLayout->addWidget(create_new_file);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout->addWidget(checkBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        button_ok = new QPushButton(centralWidget);
        button_ok->setObjectName(QStringLiteral("button_ok"));

        horizontalLayout->addWidget(button_ok);

        pushButton_cancel = new QPushButton(centralWidget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));

        horizontalLayout->addWidget(pushButton_cancel);


        verticalLayout_4->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout_4);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        Role->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Role);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 928, 25));
        Role->setMenuBar(menuBar);
        statusBar = new QStatusBar(Role);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Role->setStatusBar(statusBar);
        toolBar = new QToolBar(Role);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setIconSize(QSize(40, 40));
        Role->addToolBar(Qt::LeftToolBarArea, toolBar);

        toolBar->addAction(action_templte);
        toolBar->addAction(action_new_actor);
        toolBar->addAction(action_openpackage);
        toolBar->addAction(action_edit);
        toolBar->addAction(action_copy);
        toolBar->addAction(action_HELP);

        retranslateUi(Role);

        QMetaObject::connectSlotsByName(Role);
    } // setupUi

    void retranslateUi(QMainWindow *Role)
    {
        Role->setWindowTitle(QApplication::translate("Role", "Role", Q_NULLPTR));
        action_templte->setText(QApplication::translate("Role", "\346\250\241\346\235\277", Q_NULLPTR));
        action_new_actor->setText(QApplication::translate("Role", "\346\211\223\345\274\200\346\226\260\347\232\204\350\247\222\350\211\262\346\226\207\344\273\266", Q_NULLPTR));
        action_tree->setText(QApplication::translate("Role", "tree", Q_NULLPTR));
        action_HELP->setText(QApplication::translate("Role", "HELP", Q_NULLPTR));
        action_edit->setText(QApplication::translate("Role", "\347\274\226\350\276\221", Q_NULLPTR));
        action_copy->setText(QApplication::translate("Role", "Create new template", Q_NULLPTR));
        action_openpackage->setText(QApplication::translate("Role", "\346\211\223\345\274\200\350\247\222\350\211\262\345\214\205", Q_NULLPTR));
        label->setText(QApplication::translate("Role", "\346\225\260\346\215\256\345\272\223\346\226\207\344\273\266\346\265\217\350\247\210", Q_NULLPTR));
        label_2->setText(QApplication::translate("Role", "ACTOR LIST", Q_NULLPTR));
        openpackage->setText(QApplication::translate("Role", "\345\257\274\345\205\245\350\247\222\350\211\262\345\214\205", Q_NULLPTR));
        pushButton_add->setText(QApplication::translate("Role", "\346\267\273\345\212\240", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Role", "\345\210\240\351\231\244", Q_NULLPTR));
        pushButton_clear->setText(QApplication::translate("Role", "\345\205\250\351\203\250\346\270\205\347\251\272", Q_NULLPTR));
        label_edit->setText(QApplication::translate("Role", "\347\274\226\350\276\221\346\240\217", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("Role", "value", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("Role", "Name", Q_NULLPTR));
        pushButton_open->setText(QApplication::translate("Role", "\345\257\274\345\205\245\346\226\207\344\273\266", Q_NULLPTR));
        create_new_file->setText(QApplication::translate("Role", "\345\210\233\345\273\272\346\226\260\346\226\207\344\273\266", Q_NULLPTR));
        checkBox->setText(QApplication::translate("Role", "\344\275\277\347\224\250\346\250\241\346\235\277", Q_NULLPTR));
        button_ok->setText(QApplication::translate("Role", "\345\217\246\345\255\230\344\270\272", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("Role", "\345\205\250\351\203\250\346\270\205\351\231\244", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("Role", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Role: public Ui_Role {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROLE_H
