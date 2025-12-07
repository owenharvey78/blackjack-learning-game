QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card_sprites.cpp \
    game_widget.cpp \
    blackjack_game.cpp \
    learn_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    card.cpp \
    ruleset_widget.cpp \
    shoe.cpp

HEADERS += \
    card_sprites.h \
    game_widget.h \
    blackjack_game.h \
    learn_widget.h \
    mainwindow.h \
    ruleset.h \
    card.h \
    ruleset_widget.h \
    shoe.h

FORMS += \
    learn_widget.ui \
    mainwindow.ui
    game_widget.ui \
    mainwindow.ui \
    ruleset_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
