/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef COACH_SERVICE_H
#define COACH_SERVICE_H

#include <include/coachservice.grpc.pb.h>
#include <src/entities/services/baseservice.h>

/**
 * @brief The CoachService class is the responsible for the threatment of all Coach related rpcs
 */
class CoachService : public BaseService, public Coach::CoachService::Service {
public:
    CoachService(QString address, World* world); /*!< @note Remember that this method needs to cast registerService() method
                                                      @param address The address which the service will listen at
                                                      @param world The world pointer which the service will cast requests and store data
                                                  */
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
