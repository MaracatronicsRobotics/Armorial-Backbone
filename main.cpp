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

    //Setup world
    World *world = new World();

    //Setup pointer
    VisionService *visionService = new VisionService("0.0.0.0:50051", world);
    CoachService *coachService = new CoachService("0.0.0.0:50052", world);
    SensorService *sensorService = new SensorService("0.0.0.0:50053", world);

    visionService->start();
    coachService->start();
    sensorService->start();

    bool exec = a.exec();

    visionService->stopEntity();
    visionService->wait();
    coachService->stopEntity();
    coachService->wait();
    sensorService->stopEntity();
    sensorService->wait();

    return exec;
}
