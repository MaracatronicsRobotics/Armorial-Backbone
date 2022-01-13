#include "sensorservice.h"

SensorService::SensorService(QString address) : BaseService(address) {
    registerService(this);
    robots.clear();
}

QString SensorService::name() {
    return "Sensor";
}

grpc::Status SensorService::SetRobotStatus(grpc::ServerContext* context, RobotStatus* robotStatus) {
    mutex.lock();
    for (Robot r : robots) {
        if (r.RobotIdentifier == robotStatus.RobotIdentifier){
            r.RobotStatus.set_batterycharge(robotStatus.batteryCharge);
            r.RobotStatus.set_capacitorCharge(robotStatus.capacitorCharge);
            r.RobotStatus.set_isDribbling(robotStatus.isDribbling);
            r.RobotStatus.set_infrared(robotStatus.infrared);
            r.RobotStatus.set_kickOnTouch(robotStatus.kickOnTouch);

            return grpc::Status::OK;
        }
    }
    mutex.unlock();
    return grpc::Status::NOT_FOUND;
}

grpc::Status SensorService::SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* reader) {
    mutex.lock();
    
    int messages = reader->NextMessageSize();

    for (int i = 0; i < messages; i++){
        reader->read()
        for (Robot r : robots) {
            if (r.RobotIdentifier == robotStatus.RobotIdentifier){
                r.RobotStatus.set_batterycharge(robotStatus.batteryCharge);
                r.RobotStatus.set_capacitorCharge(robotStatus.capacitorCharge);
                r.RobotStatus.set_isDribbling(robotStatus.isDribbling);
                r.RobotStatus.set_infrared(robotStatus.infrared);
                r.RobotStatus.set_kickOnTouch(robotStatus.kickOnTouch);
            }
        }
    }
    
    mutex.unlock();
    
    return grpc::Status::OK;
}