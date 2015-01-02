TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14

SOURCES += \
    main.cc \
    math/mexception.cc

HEADERS += \
    math/mvector.h \
    math/mexception.h \
    util/traits.h \
    math/sparse.h \
    math/matrix.h \
    algs/svd.h

include(deployment.pri)
qtcAddDeployment()

