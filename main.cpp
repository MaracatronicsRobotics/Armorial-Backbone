#include <QCoreApplication>
#include <grpc/grpc.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <include/messages.pb.h>
#include <include/coachservice.grpc.pb.h>
#include <include/visionservice.grpc.pb.h>
#include <QList>
#include <QMutex>

QList<Robot> robots;
QMutex mutex;

class CoachServiceImpl final : public Coach::CoachService::Service {
public:
    explicit CoachServiceImpl() {}

    grpc::Status GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier,
                          Robot* robot) {

        mutex.lock();
        for(int i = 0; i < robots.size(); i++) {
            if(robots.at(i).robotidentifier().robotid() == identifier->robotid()
                    && (robots.at(i).robotidentifier().robotcolor().isblue() == identifier->robotcolor().isblue())) {
                Robot robozinho = robots.at(i);
                robot = &robozinho;
                mutex.unlock();

                return grpc::Status::OK;
            }
        }
        mutex.unlock();

        return grpc::Status::CANCELLED;
    }

    grpc::Status GetRobots(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                           grpc::ServerWriter<Robot>* writer) {

        mutex.lock();
        for (Robot r : robots) {
            writer->Write(r);
        }
        mutex.unlock();

        return grpc::Status::OK;
    }
};

class VisionServiceImpl final : public Vision::VisionService::Service {
public:
    explicit VisionServiceImpl() {}

    grpc::Status SetRobot(grpc::ServerContext* context, const Robot* identifier,
                          ::google::protobuf::Empty* response) {

        mutex.lock();
        for(int i = 0; i < robots.size(); i++) {
            if(robots.at(i).robotidentifier().robotid() == identifier->robotidentifier().robotid()
                    && (robots.at(i).robotidentifier().robotcolor().isblue() && identifier->robotidentifier().robotcolor().isblue())) {

                robots[i] = (*identifier);
                mutex.unlock();

                return grpc::Status::OK;
            }
        }

        Robot tst = (*identifier);
        robots.push_back(tst);
        mutex.unlock();

        return grpc::Status::OK;
    }

    grpc::Status SetRobots(grpc::ServerContext* context, grpc::ServerReader<Robot>* reader,
                           ::google::protobuf::Empty* response) {

        Robot robot;
        while(reader->Read(&robot)) {
            bool found = false;
            mutex.lock();

            for(int i = 0; i < robots.size(); i++) {
                if(robots.at(i).robotidentifier().robotid() == robot.robotidentifier().robotid()
                        && (robots.at(i).robotidentifier().robotcolor().isblue() && robot.robotidentifier().robotcolor().isblue())) {

                    robots[i] = robot;
                    found = true;
                }
            }

            if(!found) {
                robots.push_back(robot);
            }

            mutex.unlock();
        }

        return grpc::Status::OK;
    }
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
            std::cout << "Nao foi possivel achar o danadinho do robo.\n";
            return false;
        }

        return true;
    }

    bool GetRobots(QList<Robot>* robot) {
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<Robot>> robots = stub_->GetRobots(&context, google::protobuf::Empty());

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
            std::cout << "nao foi possivel achar o danadinho do robo.\n";
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

void createRobot(Robot *robot, int robotId, bool isBlue, float x, float y, float z, float vx, float vy, float vz, float ori, bool isOriRad, float vw, bool isVwRad) {
    Color color;
    color.set_isblue(isBlue);

    RobotIdentifier robotIdentifier;
    robotIdentifier.set_robotid(robotId);
    robotIdentifier.set_allocated_robotcolor(&color);

    Position position;
    position.set_x(x);
    position.set_y(y);
    position.set_z(z);

    Velocity velocity;
    velocity.set_vx(vx);
    velocity.set_vy(vy);
    velocity.set_vz(vz);

    Angle angle;
    angle.set_value(ori);
    angle.set_isinradians(isOriRad);

    AngularSpeed aSpeed;
    aSpeed.set_vw(vw);
    aSpeed.set_isinradians(isVwRad);

    ::google::protobuf::Timestamp tempo;

    RobotStatus robotStatus;
    robotStatus.set_infrared(true);
    robotStatus.set_isdribbling(true);
    robotStatus.set_kickontouch(true);
    robotStatus.set_batterycharge(0.0);
    robotStatus.set_capacitorcharge(0.0);

    robot->set_allocated_robotidentifier(&robotIdentifier);
    robot->set_allocated_robotvelocity(&velocity);
    robot->set_allocated_robotposition(&position);
    robot->set_allocated_robotangularspeed(&aSpeed);
    robot->set_allocated_robotorientation(&angle);
    robot->set_allocated_packettimestamp(&tempo);
    robot->set_allocated_robotstatus(&robotStatus);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Coach
    std::string coachAddress("0.0.0.0:50051");
    CoachServiceImpl coachService;

    grpc::ServerBuilder coachBuilder;
    coachBuilder.AddListeningPort(coachAddress, grpc::InsecureServerCredentials());
    coachBuilder.RegisterService(&coachService);

    std::unique_ptr<grpc::Server> coachServer(coachBuilder.BuildAndStart());
    std::cout << "Coach Server listening on " << coachAddress << std::endl;

    // Vision
    std::string visionAddress("0.0.0.0:50052");
    VisionServiceImpl visionService;

    grpc::ServerBuilder visionBuilder;
    visionBuilder.AddListeningPort(visionAddress, grpc::InsecureServerCredentials());
    visionBuilder.RegisterService(&visionService);

    std::unique_ptr<grpc::Server> visionServer(visionBuilder.BuildAndStart());
    std::cout << "Vision Server listening on " << visionAddress << std::endl;

    CoachClient coachClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    VisionClient visionClient(grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials()));

    // Teste do coach client consultando o robo 0 amarelo
    RobotIdentifier identifier;
    Color robotColor;
    robotColor.set_isblue(false);
    identifier.set_robotid(0);
    identifier.set_allocated_robotcolor(&robotColor);
    Robot robot;
    coachClient.GetRobot(identifier, &robot);

    // Teste do cadastro do robo 0 amarelo
    Robot robo_danadinho = Robot();
    visionClient.SetRobot(robo_danadinho);

    // Teste do coach client consultando o robo 0 amarelo
    coachClient.GetRobot(identifier, &robot);

    coachServer->Wait();
    visionServer->Wait();

    /*
    if(argc >= 2) {
        std::string server_address("0.0.0.0:50051");
        RouteGuideImpl service;

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
        server->Wait();
    }
    else {
        RouteGuideClient guide(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
        guide.GetFeature();
    }
    */

    return a.exec();
}
