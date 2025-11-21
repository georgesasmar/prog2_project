QT += core gui widgets

CONFIG += c++11

TARGET = StudentGradesSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    json.hpp

FORMS +=

# Deployment settings
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/StudentGradesSystem
INSTALLS += target