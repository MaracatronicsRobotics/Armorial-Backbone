#ifndef VISIONSERVICE_H
#define VISIONSERVICE_H

#include <src/entities/services/baseservice.h>
#include <include/visionservice.grpc.pb.h>

class VisionService : public BaseService, public Vision::VisionService::Service {
public:
    VisionService(QString address);
    QString name();
    QMutex mutex;

    grpc::Status SetRobot(::grpc::ServerContext *context, const ::Robot *robot, ::google::protobuf::Empty *response);
    grpc::Status SetRobots(::grpc::ServerContext *context, ::grpc::ServerReader< ::Robot> *reader, ::google::protobuf::Empty *response);
    grpc::Status SetBall(::grpc::ServerContext *context, const ::Ball *ball, ::google::protobuf::Empty *response);
    grpc::Status SetField(::grpc::ServerContext *context, const ::Field *field, ::google::protobuf::Empty *response);

private:
    QList<Robot> _robots;
    Ball _ball;
    Field _field;
};

#endif // VISIONSERVICE_H
