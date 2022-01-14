#include "sensorservice.h"

SensorService::SensorService(QString address) : BaseService(address) {
    registerService(this);
    robots.clear();
}

QString SensorService::name() {
    return "Sensor";
}

grpc::Status SensorService::SetRobotStatus(grpc::ServerContext* context, RobotStatus* robotStatus, const ::google::protobuf::Empty* request) {
    RobotIdentifier rsid = robotStatus->robotidentifier();

    mutex.lock();
    for (Robot r : robots) {
        RobotIdentifier rid = r.robotidentifier();

        if ((rid.robotid() == rsid.robotid()) && rid.robotcolor().isblue() == rsid.robotcolor().isblue()){
            // set Information
            r.set_allocated_robotstatus(robotStatus);
            mutex.unlock();
            return grpc::Status::OK;
        }
    }
    mutex.unlock();
    return grpc::Status::CANCELLED;
}

grpc::Status SensorService::SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* reader, const ::google::protobuf::Empty* request) {
    mutex.lock();
    
    RobotStatus rs;
    while (reader->Read(&rs)){
        RobotIdentifier rsid = rs.robotidentifier();
        for (Robot r : robots) {
            RobotIdentifier rid = r.robotidentifier();

            if (rid.robotid() == rsid.robotid() && rid.robotcolor().isblue() == rsid.robotcolor().isblue()){
                // Match
                r.set_allocated_robotstatus(&rs);
            }
        }
    }
    mutex.unlock();
    
    return grpc::Status::OK;
}
