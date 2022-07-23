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
QT += core network

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lQt5Core -lprotobuf -lgrpc -lgrpc++ -lGLU -lfmt -lArmorial

system(echo "Generating GRPC headers" && cd include/proto/services && protoc --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto && cd ../../..)
system(echo "Generating proto headers" && cd include/proto/services && protoc --cpp_out=../ *.proto && cd ../../..)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += PROJECT_PATH=\\\"$${PWD}\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        include/proto/actuatorservice.grpc.pb.cc \
        include/proto/actuatorservice.pb.cc \
        include/proto/messages.grpc.pb.cc \
        include/proto/messages.pb.cc \
        include/proto/sensorservice.grpc.pb.cc \
        include/proto/sensorservice.pb.cc \
        include/proto/visionservice.grpc.pb.cc \
        include/proto/visionservice.pb.cc \
        main.cpp \
        src/services/actuator/actuatorservice.cpp \
        src/services/sensor/sensorservice.cpp \
        src/services/vision/visionservice.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/proto/actuatorservice.grpc.pb.h \
    include/proto/actuatorservice.pb.h \
    include/proto/messages.grpc.pb.h \
    include/proto/messages.pb.h \
    include/proto/sensorservice.grpc.pb.h \
    include/proto/sensorservice.pb.h \
    include/proto/visionservice.grpc.pb.h \
    include/proto/visionservice.pb.h \
    src/services/actuator/actuatorservice.h \
    src/services/baseservice.h \
    src/services/sensor/sensorservice.h \
    src/services/vision/visionservice.h
