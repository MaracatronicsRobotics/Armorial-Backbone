#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H


#include <include/sensorservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

class SensorService : public BaseService, public Sensor::SensorService::Service {
public:
    SensorService(QString address, World* world);
    QString name();

    QMutex mutex;
    /**
     * Updates the status of a single robot.
     *
     * @param The new RobotStatus (containing the identifier)
     * @param request Empty as default (no request data needed)
     * @return status of the request
     */
    grpc::Status SetRobotStatus(grpc::ServerContext* context, RobotStatus* robotStatus, const ::google::protobuf::Empty* request);
    /**
     * Updates the status of many robots.
     *
     * @param The new RobotStatus (containing the identifiers)
     * @param request Empty as default (no request data needed)
     * @return status of the request
     */
    grpc::Status SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* allRobotStatus, const ::google::protobuf::Empty* request);
private:
    QList<Robot> robots;
};

#endif // SENSOR_SERVICE_H
