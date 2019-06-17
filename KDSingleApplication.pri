# settings for (internal) users of KDSingleApplication: examples, tests, ...

include(common.pri)

win32 {
#    build_pass {
        CONFIG(debug, debug|release) {
            kdsingleapplication_static_lib = $$shadowed($$PWD/src)/debug/$${QMAKE_PREFIX_STATICLIB}kdsingleapplication.$${QMAKE_EXTENSION_STATICLIB}
        }
        CONFIG(release, debug|release) {
            kdsingleapplication_static_lib = $$shadowed($$PWD/src)/release/$${QMAKE_PREFIX_STATICLIB}kdsingleapplication.$${QMAKE_EXTENSION_STATICLIB}
        }
#    }
} else {
   kdsingleapplication_static_lib = $$shadowed($$PWD/src)/$${QMAKE_PREFIX_STATICLIB}kdsingleapplication.$${QMAKE_EXTENSION_STATICLIB}
}

KDINCLUDEPATH = $$PWD/src
KDSINGLEAPPLICATIONLIB = $$kdsingleapplication_static_lib

INCLUDEPATH += $$KDINCLUDEPATH
LIBS += $$KDSINGLEAPPLICATIONLIB
PRE_TARGETDEPS += $$KDSINGLEAPPLICATIONLIB

QT = core network
win32:LIBS += -lkernel32
