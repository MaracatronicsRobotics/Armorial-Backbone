#ifndef COACH_SERVICE_H
#define COACH_SERVICE_H

#include <include/coachservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

class CoachService : public BaseService, public Coach::CoachService::Service {
public:
    CoachService(QString address);
    QString name();

    grpc::Status GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier, Robot* robot);
    grpc::Status GetRobots(grpc::ServerContext* context, const ::google::protobuf::Empty* request, grpc::ServerWriter<Robot>* writer);
};

#endif // COACH_SERVICE_H
