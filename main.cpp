#include <QCoreApplication>

#include <src/entities/services/coach/coachservice.h>
#include <src/entities/services/vision/visionservice.h>
#include <src/entities/services/sensor/sensorservice.h>
#include <src/entities/services/actuator/actuatorservice.h>
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
    ActuatorService *actuatorService = new ActuatorService("0.0.0.0:50054", world);

    ControlPacket packet;
    Velocity *velocity = new Velocity();
    velocity->set_vx(2.0);
    velocity->set_vy(0.0);
    packet.set_allocated_robotvelocity(velocity);
    world->setControlPacket(packet);

    visionService->start();
    coachService->start();
    sensorService->start();
    actuatorService->start();

    bool exec = a.exec();

    visionService->stopEntity();
    visionService->wait();
    coachService->stopEntity();
    coachService->wait();
    sensorService->stopEntity();
    sensorService->wait();
    actuatorService->stopEntity();
    actuatorService->wait();

    return exec;
}
