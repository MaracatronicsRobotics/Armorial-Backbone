#include "sensorservice.h"

SensorService::SensorService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString SensorService::name() {
    return "Sensor";
}

grpc::Status SensorService::SetRobotStatus(grpc::ServerContext* context, RobotStatus* robotStatus, const ::google::protobuf::Empty* request) {

    std::cout << "UEPA 1" << std::endl;
    getWorld()->setRobotStatus(robotStatus);
    std::cout << "UEPA 2" << std::endl;

    return grpc::Status::OK;
}

grpc::Status SensorService::SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* reader, const ::google::protobuf::Empty* request) {
    RobotStatus rs;

    QList<RobotStatus> rsList;
    while (reader->Read(&rs)){
        rsList.push_back(rs);
    }

    for (RobotStatus robotStatus : rsList) {
        std::cout << robotStatus.robotidentifier().robotid() << std::endl;
    }

    getWorld()->setRobotsStatus(rsList);
    
    return grpc::Status::OK;
}
