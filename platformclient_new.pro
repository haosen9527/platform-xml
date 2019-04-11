#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T13:02:10
#
#-------------------------------------------------

QT       += core gui
QT       +=xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = platformclient_new
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        platformclient.cpp \
    setting.cpp \
    dbxml_sql.cpp

HEADERS += \
        platformclient.h \
    setting.h \
    dbxml_sql.h \
    xml_merge.h

FORMS += \
    platformclient.ui \
    setting.ui

unix:!macx: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -ldb_cxx-6.2

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../dbxml-6.1.4/install/lib/libdb_cxx-6.2.a

unix:!macx: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -ldbxml

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../dbxml-6.1.4/install/lib/libdbxml.a

unix:!macx: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -lxerces-c

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../dbxml-6.1.4/install/lib/libxerces-c.a

unix:!macx: LIBS += -L$$PWD/../../dbxml-6.1.4/install/lib/ -lxqilla

INCLUDEPATH += $$PWD/../../dbxml-6.1.4/install/include
DEPENDPATH += $$PWD/../../dbxml-6.1.4/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../dbxml-6.1.4/install/lib/libxqilla.a

RESOURCES += \
    platformclient.qrc
