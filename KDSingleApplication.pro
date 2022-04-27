lessThan(QT_MAJOR_VERSION, 6) {
  lessThan(QT_MAJOR_VERSION, 5)|lessThan(QT_MINOR_VERSION, 12) {
    error("Qt >= 5.12 is required")
  }
}

TEMPLATE = subdirs
SUBDIRS += \
    src \
    examples \
    tests \

CONFIG += ordered
