# -------------------------------------------------
# Project created by QtCreator 2009-07-28T23:12:55
# -------------------------------------------------
QT += opengl \
    script \
    webkit
TARGET = ../QSGL
TEMPLATE = app
SOURCES += main.cpp \
    qsglwindow.cpp \
    qsglvisualizationwindow.cpp
HEADERS += qsglwindow.h \
    qsglvisualizationwindow.h
FORMS += qsglwindow.ui \
    qsglaboutdialog.ui
TRANSLATIONS = translations/qsgl_es.ts \
    translations/qsgl_pt.ts
