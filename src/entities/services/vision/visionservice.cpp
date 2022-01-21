#include "visionservice.h"

VisionService::VisionService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString VisionService::name() {
    return "Vision";
}

grpc::Status VisionService::SetRobot(::grpc::ServerContext *context, const ::Robot *robot, ::google::protobuf::Empty *response) {
    getWorld()->setRobotData(robot);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetRobots(::grpc::ServerContext *context, ::grpc::ServerReader< ::Robot> *reader, ::google::protobuf::Empty *response){
    Robot robot;
    QList<Robot> robotList;

    while (reader->Read(&robot)) {
        robotList.push_back(robot);
    }

    getWorld()->setRobotsData(robotList);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetBall(::grpc::ServerContext *context, const ::Ball *ball, ::google::protobuf::Empty *response){
    getWorld()->setBallData(ball);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetField(::grpc::ServerContext *context, const ::Field *field, ::google::protobuf::Empty *response){
    getWorld()->setFieldData(field);

    return grpc::Status::OK;
}
