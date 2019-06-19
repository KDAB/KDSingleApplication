include(../../auto.pri)
TEMPLATE = app
TARGET = ../tst_kdsingleapplication
linux:CONFIG += testcase
#!linux:CONFIG += insignificant_test

SOURCES += \
    ../tst_kdsingleapplication.cpp \
