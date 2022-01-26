#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H


#include <include/sensorservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

class SensorService : public BaseService, public Sensor::SensorService::Service {
public:
    SensorService(QString address, World* world);
    QString name();

    /**
     * Updates the status of a single robot.
     *
     * @param The new RobotStatus (containing the identifier)
     * @param request Empty as default (no request data needed)
     * @return status of the request
     */
    grpc::Status SetRobotStatus(grpc::ServerContext* context, const RobotStatus* robotStatus, ::google::protobuf::Empty* request);
    /**
     * Updates the status of many robots.
     *
     * @param The new RobotStatus (containing the identifiers)
     * @param request Empty as default (no request data needed)
     * @return status of the request
     */
    grpc::Status SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* allRobotStatus, ::google::protobuf::Empty* request);
};

#endif // SENSOR_SERVICE_H
