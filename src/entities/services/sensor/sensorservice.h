#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H


#include <include/sensorservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

class SensorService : public BaseService, public Sensor::SensorService::Service {
public:
    SensorService(QString address);
    QString name();

    grpc::Status SetRobotStatus(grpc::ServerContext* context, RobotStatus* robotStatus)
    grpc::Status SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* allRobotStatus)
private:
    QList<Robot> robots;
};

#endif // SENSOR_SERVICE_H