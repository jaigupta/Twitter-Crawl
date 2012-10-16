#-------------------------------------------------
#
# Project created by QtCreator 2011-05-10T21:14:18
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = web
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    linklabel.cpp \
    store.cpp \
    mycookiejar.cpp \
    mywebpage.cpp \
    qjson/json_parser.cc \
    qjson/json_scanner.cpp \
    qjson/parser.cpp \
    qjson/parserrunnable.cpp \
    qjson/qobjecthelper.cpp \
    qjson/serializer.cpp \
    qjson/serializerrunnable.cpp

HEADERS  += mainwindow.h \
    linklabel.h \
    config.h \
    store.h \
    mycookiejar.h \
    mywebpage.h \
    qjson/json_parser.hh \
    qjson/json_scanner.h \
    qjson/location.hh \
    qjson/parser.h \
    qjson/parser_p.h \
    qjson/parserrunnable.h \
    qjson/position.hh \
    qjson/qjson_debug.h \
    qjson/qjson_export.h \
    qjson/qobjecthelper.h \
    qjson/serializer.h \
    qjson/serializerrunnable.h \
    qjson/stack.hh

FORMS    += mainwindow.ui

OTHER_FILES += \
    qjson/CMakeLists.txt \
    qjson/json_parser.yy
