#ifndef COACH_SERVICE_H
#define COACH_SERVICE_H

#include <include/coachservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

class CoachService : public BaseService, public Coach::CoachService::Service {
public:
    CoachService(QString address); /*!< @note Remember that this method needs to cast registerService() method @param address The address which the service will listen at */
    QString name();

    /**
     * Get an robot with specified identifier
     *
     * @param identifier The robot identifier (id and color)
     * @param robot The returned robot data (if any)
     * @return status of the request
     */
    grpc::Status GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier, Robot* robot);

    /**
     * Get all the robots from specified team color
     *
     * @param teamColor The required team color
     * @param writer The gRPC stream that will output all the robots
     * @return status of the request
     */
    grpc::Status GetRobots(grpc::ServerContext* context, const Color* teamColor, grpc::ServerWriter<Robot>* writer);

    /**
     * Get the ball data
     *
     * @param request Empty as default (no request data needed)
     * @param ball The returned ball data (if any)
     * @return status of the request
     */
    grpc::Status GetBall(grpc::ServerContext* context, const ::google::protobuf::Empty* request, Ball* ball);

    /**
     * Get the field data
     *
     * @param request Empty as default (no request data needed)
     * @param field The returned field data (if any)
     * @return status of the request
     */
    grpc::Status GetField(grpc::ServerContext* context, const ::google::protobuf::Empty* request, Field* field);

};

#endif // COACH_SERVICE_H
