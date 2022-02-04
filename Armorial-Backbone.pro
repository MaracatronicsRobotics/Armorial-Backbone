QT -= gui

TEMPLATE = app
DESTDIR  = ../bin
TARGET   = Armorial-Backbone
VERSION  = 1.0.0

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

CONFIG += c++17 console
CONFIG -= app_bundle
QT += core \
        gui \
        widgets \
        network \
        opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lQt5Core -lprotobuf -lgrpc++

system(echo "compiling protobuf" && cd proto/services && protoc --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto && cd ../../..)
system(echo "compiling protobuf" && cd proto/services && protoc --cpp_out=../ *.proto && cd ../../..)

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
        proto/actuatorservice.grpc.pb.cc \
        proto/actuatorservice.pb.cc \
        proto/coachservice.grpc.pb.cc \
        proto/messages.grpc.pb.cc \
        proto/messages.pb.cc \
        proto/sensorservice.grpc.pb.cc \
        proto/sensorservice.pb.cc \
        proto/visionservice.grpc.pb.cc \
        proto/visionservice.pb.cc \
        main.cpp \
        src/entities/entity.cpp \
        src/entities/services/baseservice.cpp \
        src/entities/services/actuator/actuatorservice.cpp \
        src/entities/services/coach/coachservice.cpp \
        src/entities/services/vision/visionservice.cpp \
        src/entities/services/sensor/sensorservice.cpp \
        src/exithandler.cpp \
        src/utils/text/text.cpp \
        src/utils/timer/timer.cpp \
        src/world/world.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \ \
    proto/actuatorservice.grpc.pb.h \
    proto/actuatorservice.pb.h \
    proto/coachservice.grpc.pb.h \
    proto/coachservice.pb.h \
    proto/messages.grpc.pb.h \
    proto/messages.pb.h \
    proto/sensorservice.grpc.pb.h \
    proto/sensorservice.pb.h \
    proto/visionservice.grpc.pb.h \
    proto/visionservice.pb.h \
    src/entities/entity.h \
    src/entities/services/baseservice.h \
    src/entities/services/actuator/actuatorservice.h \
    src/entities/services/coach/coachservice.h \
    src/entities/services/vision/visionservice.h \
    src/entities/services/sensor/sensorservice.h \
    src/exithandler.h \
    src/utils/text/text.h \
    src/utils/timer/timer.h \
    src/world/world.h
