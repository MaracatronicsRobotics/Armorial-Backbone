#include "coachservice.h"

CoachService::CoachService(QString address) : BaseService(address) {
    registerService(this);
}

QString CoachService::name() {
    return "Coach";
}

grpc::Status CoachService::GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier,
                          Robot* robot) {

    return grpc::Status::OK;
}

grpc::Status CoachService::GetRobots(grpc::ServerContext* context, const Color* teamColor,
                        grpc::ServerWriter<Robot>* writer) {

    return grpc::Status::OK;
}

grpc::Status CoachService::GetBall(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                          Ball* ball) {

    return grpc::Status::OK;
}

grpc::Status CoachService::GetField(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                          Field* field) {

    return grpc::Status::OK;
}
