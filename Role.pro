#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T07:56:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Role
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        role.cpp \
    dbxml_sql.cpp \
    create_new_xml.cpp \
    treeview.cpp

HEADERS  += role.h \
    dbxml_sql.h \
    file_list.h \
    create_new_xml.h \
    treeview.h

FORMS    += role.ui \
    create_new_xml.ui \
    treeview.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/release/ -ldb_cxx-6.2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/debug/ -ldb_cxx-6.2
else:unix: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -ldb_cxx-6.2

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/release/ -ldbxml-6.1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/debug/ -ldbxml-6.1
else:unix: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -ldbxml-6.1

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

RESOURCES += \
    ico.qrc
