#include <QCoreApplication>
#include <QList>
#include <QMutex>

#include <src/entities/services/coach/coachservice.h>
#include <src/entities/services/vision/visionservice.h>
#include <src/entities/services/sensor/sensorservice.h>
#include <src/exithandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Setup ExitHandler
    ExitHandler::setApplication(&a);
    ExitHandler::setup();

    bool exec = a.exec();

    return exec;
}
