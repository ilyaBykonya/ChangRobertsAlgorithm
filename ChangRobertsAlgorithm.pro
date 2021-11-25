TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        AlgorithmParticipant/AlgorithmParticipant.cpp \
        TestModule/TestModule.cpp \
        main.cpp

HEADERS += \
    AlgorithmParticipant/AlgorithmParticipant.h \
    TestModule/TestModule.h \
    ThreadsafeCout/ThreadsafeCout.h \
    ThreadsafeQueue/ThreadsafeQueue.h
