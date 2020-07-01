QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Connection.cpp \
    Image.cpp \
    Message.cpp \
    Server.cpp \
    admin.cpp \
    adminprocess.cpp \
    adminwindow.cpp \
    ansquiz.cpp \
    audio.cpp \
    audioframe.cpp \
    client.cpp \
    loginprocess.cpp \
    main.cpp \
    loginwindow.cpp \
    makequiz.cpp \
    performance.cpp \
    quiz.cpp \
    resultwindow.cpp \
    student.cpp \
    studentprocess.cpp \
    studentquiz.cpp \
    studentwindow.cpp \
    teacher.cpp \
    teacherprocess.cpp \
    teacherquiz.cpp \
    teacherwindow.cpp \
    user.cpp

HEADERS += \
    Connection.h \
    Image.h \
    Message.h \
    MessageType.h \
    Server.h \
    admin.h \
    adminprocess.h \
    adminwindow.h \
    ansquiz.h \
    audio.h \
    audioframe.h \
    client.h \
    loginprocess.h \
    loginwindow.h \
    makequiz.h \
    performance.h \
    quiz.h \
    resultwindow.h \
    student.h \
    studentprocess.h \
    studentquiz.h \
    studentwindow.h \
    teacher.h \
    teacherprocess.h \
    teacherquiz.h \
    teacherwindow.h \
    user.h

FORMS += \
    adminwindow.ui \
    ansquiz.ui \
    loginwindow.ui \
    makequiz.ui \
    resultwindow.ui \
    studentwindow.ui \
    teacherwindow.ui

LIBS += -lws2_32 \
    -lgdi32 \
    -lgdiplus \
    -lwinmm \
    -lole32

DEFINES += QT_MESSAGELOGCONTEXT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
