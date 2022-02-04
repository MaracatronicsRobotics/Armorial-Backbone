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


#ifndef VISIONSERVICE_H
#define VISIONSERVICE_H

#include <src/entities/services/baseservice.h>
#include <proto/visionservice.grpc.pb.h>

/**
 * @brief The VisionService class is the responsible for the threatment of all Vision related rpcs
 */
class VisionService : public BaseService, public Vision::VisionService::Service {
public:
    VisionService(QString address, World* world);/*!< @note Remember that this method needs to cast registerService() method
                                                      @param address The address which the service will listen at
                                                      @param world The world pointer which the service will cast requests and store data
                                                  */
    QString name();

    /**
     * Set a robot
     *
     * @param request Empty as default (no request data needed)
     * @param robot The robot to be set
     * @return status of the request
     */
    grpc::Status SetRobot(::grpc::ServerContext *context, const ::Robot *robot, ::google::protobuf::Empty *response);

    /**
     * Set a list of robot
     *
     * @param request Empty as default (no request data needed)
     * @param reader The server stream of robots to be set
     * @return status of the request
     */
    grpc::Status SetRobots(::grpc::ServerContext *context, ::grpc::ServerReader< ::Robot> *reader, ::google::protobuf::Empty *response);

    /**
     * Set the ball
     *
     * @param request Empty as default (no request data needed)
     * @param ball The ball to be set
     * @return status of the request
     */
    grpc::Status SetBall(::grpc::ServerContext *context, const ::Ball *ball, ::google::protobuf::Empty *response);

    /**
     * Set a field
     *
     * @param request Empty as default (no request data needed)
     * @param field The field to be set
     * @return status of the request
     */
    grpc::Status SetField(::grpc::ServerContext *context, const ::Field *field, ::google::protobuf::Empty *response);

private:
};

#endif // VISIONSERVICE_H
