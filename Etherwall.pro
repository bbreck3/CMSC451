TEMPLATE = app

QT += qml quick widgets network

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/main.cpp \
    src/accountmodel.cpp \
    src/types.cpp \
    src/etheripc.cpp \
    src/settings.cpp \
    src/bigint.cpp \
    src/transactionmodel.cpp \
    src/clipboard.cpp \
    src/etherlog.cpp \
<<<<<<< HEAD
    src/contractmodel.cpp \
    src/receiptmodel.cpp
=======
    src/contractmodel.cpp
>>>>>>> 099452e8305506f3f2695a486526fba629aa7a32

RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Default rules for deployment.
include(deployment.pri)

TRANSLATIONS += \
    i18n/etherwall.ts

lupdate_only {
    SOURCES += \
        qml/*.qml \
        qml/components/*.qml
}

HEADERS += \
    src/accountmodel.h \
    src/types.h \
    src/etheripc.h \
    src/settings.h \
    src/bigint.h \
    src/transactionmodel.h \
    src/clipboard.h \
    src/etherlog.h \
<<<<<<< HEAD
    src/contractmodel.h \
    src/receiptmodel.h
=======
    src/contractmodel.h
>>>>>>> 099452e8305506f3f2695a486526fba629aa7a32

macx {
    ICON=qml/images/icon.icns
}

win32 {
    RC_ICONS = icon.ico
}

DISTFILES += \
    EthereumForm.ui.qml \
    Ethereum.qml \
    qml/components/EthForm.ui.qml \
    qml/components/Eth.qml
