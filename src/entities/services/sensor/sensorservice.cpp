#include "sensorservice.h"

SensorService::SensorService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString SensorService::name() {
    return "Sensor";
}

grpc::Status SensorService::SetRobotStatus(grpc::ServerContext* context, const RobotStatus* robotStatus, ::google::protobuf::Empty* request) {
    getWorld()->setRobotStatus(robotStatus);

    return grpc::Status::OK;
}

grpc::Status SensorService::SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* reader, ::google::protobuf::Empty* request) {
    RobotStatus rs;

    QList<RobotStatus> rsList;
    while (reader->Read(&rs)){
        rsList.push_back(rs);
    }

    getWorld()->setRobotsStatus(rsList);

    return grpc::Status::OK;
}
