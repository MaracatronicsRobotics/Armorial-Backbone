#include <QCoreApplication>
#include <QList>
#include <QMutex>

#include <src/entities/services/coach/coachservice.h>
#include <src/entities/services/vision/visionservice.h>
#include <src/entities/services/sensor/sensorservice.h>
#include <src/exithandler.h>

class SensorClient {
public:
    SensorClient(std::shared_ptr<grpc::Channel> channel) : stub_(Sensor::SensorService::NewStub(channel)) {

    }

    bool SetRobotStatus(RobotStatus robotStatus) {
        grpc::ClientContext context;
        google::protobuf::Empty empty;
        grpc::Status status = stub_->SetRobotStatus(&context, robotStatus, &empty);

        if(status.ok()) {
            std::cout << "robot succesfully registered.\n";
        }
        else {
            std::cout << "nao foi possivel achar o danadinho do robo em SensorService.\n";
            return false;
        }

        return true;
    }

    bool SetAllRobotStatus(QList<RobotStatus> robotStatusList) {
        grpc::ClientContext context;
        google::protobuf::Empty empty;
        std::unique_ptr<grpc::ClientWriter<RobotStatus>> writer = stub_->SetAllRobotStatus(&context, &empty);

        for (RobotStatus r : robotStatusList) {
            writer->Write(r);
        }

        return true;
    }

private:
    std::unique_ptr<Sensor::SensorService::Stub> stub_;
};

class CoachClient {
public:
    CoachClient(std::shared_ptr<grpc::Channel> channel) : stub_(Coach::CoachService::NewStub(channel)) {

    }

    bool GetRobot(const RobotIdentifier& identifier, Robot *robot) {
        grpc::ClientContext context;
        grpc::Status status = stub_->GetRobot(&context, identifier, robot);

        if(status.ok()) {
            std::cout << "x: " << robot->robotposition().x() << " | y: " << robot->robotposition().y() << std::endl;
        }
        else {
            std::cout << "Nao foi possivel achar o danadinho do robo em CoachService.\n";
            return false;
        }

        return true;
    }

    bool GetRobots(QList<Robot>* robot, Color color) {
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<Robot>> robots = stub_->GetRobots(&context, color);

        Robot readRobot;
        while(robots->Read(&readRobot)) {
            robot->push_back(readRobot);
        }

        return true;
    }

private:
    std::unique_ptr<Coach::CoachService::Stub> stub_;
};

class VisionClient {
public:
    VisionClient(std::shared_ptr<grpc::Channel> channel) : stub_(Vision::VisionService::NewStub(channel)) {

    }

    bool SetRobot(Robot robot) {
        grpc::ClientContext context;
        google::protobuf::Empty empty;
        grpc::Status status = stub_->SetRobot(&context, robot, &empty);

        if(status.ok()) {
            std::cout << "robot succesfully registered.\n";
        }
        else {
            std::cout << "nao foi possivel achar o danadinho do robo em VisionService.\n";
            return false;
        }

        return true;
    }

    bool SetRobots(QList<Robot> robots) {
        grpc::ClientContext context;
        google::protobuf::Empty empty;
        std::unique_ptr<grpc::ClientWriter<Robot>> writer = stub_->SetRobots(&context, &empty);

        for (Robot r : robots) {
            writer->Write(r);
        }

        return true;
    }

private:
    std::unique_ptr<Vision::VisionService::Stub> stub_;
};

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

    VisionClient *visionClient = new VisionClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    CoachClient *coachClient = new CoachClient(grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials()));
    SensorClient *sensorClient = new SensorClient(grpc::CreateChannel("localhost:50053", grpc::InsecureChannelCredentials()));

    QList<RobotIdentifier*> robotIdentifierList;
    QList<Color*> colorList;
    QList<RobotStatus> robotStatusList;
    QList<Robot> robotList;
    std::cout << "Inserting robots..." << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "Robot n: " << i << std::endl;
        Robot robot;
        RobotIdentifier *robotID = new RobotIdentifier();
        RobotIdentifier *robotID2 = new RobotIdentifier();
        RobotStatus robotStatus;

        robotID->set_robotid(i);
        robotID2->set_robotid(i);
        Color *color = new Color();
        Color *color2 = new Color();
        color->set_isblue(i%2);
        color2->set_isblue(i%2);
        robotID->set_allocated_robotcolor(color);
        robotID2->set_allocated_robotcolor(color2);
        robot.set_allocated_robotidentifier(robotID);
        std::cout << "Robot " << i << " is set" << std::endl;

        robotStatus.set_allocated_robotidentifier(robotID2);
        robotStatus.set_infrared(true);
        robotStatus.set_isdribbling(true);
        robotStatus.set_batterycharge(80.0);
        robotStatus.set_capacitorcharge(69.0);
        robotStatus.set_kickontouch(false);
        std::cout << "Robot " << i << " status are set" << std::endl;

        visionClient->SetRobot(robot);
        std::cout << "Robot " << i << " sent to vision" << std::endl;
        robotIdentifierList.push_back(robotID2);
        colorList.push_back(color2);
        robotStatusList.push_back(robotStatus);
        //sensorClient->SetRobotStatus(robotStatus);
        //std::cout << "Robot " << i << " status sent to sensor" << std::endl;
    }
    std::cout << "Setting all robots in the robotStatusList!" << std::endl;
    sensorClient->SetAllRobotStatus(robotStatusList);
    std::cout << "Setted all robots in the robotStatusList!" << std::endl;
    Color color;
    color.set_isblue(false);

//    coachClient->GetRobots(&robotList, color);

//    std::cout << "Tamanho da lista: " << robotList.size() << std::endl;
//    for (Robot r : robotList) {
//        std::cout << r.robotidentifier().robotid() << std::endl;
//    }

    bool exec = a.exec();

    visionService->stopEntity();
    visionService->wait();
    coachService->stopEntity();
    coachService->wait();
    sensorService->stopEntity();
    sensorService->wait();

    return exec;
}
