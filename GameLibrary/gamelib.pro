QT       += core gui widgets

CONFIG   += c++17

TEMPLATE = app
TARGET   = gamelib

#—— Source files ———————————————————————————————
SOURCES += \
    main.cpp \
    DataStore.cpp \
    LoginWidget.cpp \
    SignupWidget.cpp \
    LibraryWidget.cpp

#—— Header files ———————————————————————————————
HEADERS += \
    DataStore.h \
    LoginWidget.h \
    SignupWidget.h \
    LibraryWidget.h

#—— Deploy the JSON files alongside your exe ——————————
DISTFILES += \
    users.json \
    games.json
