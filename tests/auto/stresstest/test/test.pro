include(../../auto.pri)
TEMPLATE = app
TARGET = ../tst_stresstest
linux:CONFIG += testcase

SOURCES += \
    ../tst_stresstest.cpp \
