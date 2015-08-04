#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T19:02:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crypto_analayzer_polyalphabetic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    indexofconsistency.cpp \
    polyalphabetic.cpp \
    monoalphabetic.cpp \
    mdiwindow.cpp

HEADERS  += mainwindow.h \
    indexofconsistency.h \
    polyalphabetic.h \
    monoalphabetic.h \
    mdiwindow.h

FORMS    += mainwindow.ui \
    indexofconsistency.ui \
    polyalphabetic.ui \
    monoalphabetic.ui \
    mdiwindow.ui

RESOURCES += \
    rec.qrc

OTHER_FILES += \
    alphabet.txt \
    char_bigram.txt \
    char_probability.txt \
    cipher_text_alphabet.txt \
    corr_table.txt \
    dictionary.txt \
    dictionary_big.txt \
    shift.txt

RC_FILE = myapp.rc

QMAKE_LFLAGS += -static -static-libgcc
