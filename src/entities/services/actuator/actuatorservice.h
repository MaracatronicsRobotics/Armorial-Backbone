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


#ifndef ACTUATORSERVICE_H
#define ACTUATORSERVICE_H

#include <src/entities/services/baseservice.h>
#include <proto/actuatorservice.grpc.pb.h>

/**
 * @brief The ActuatorService class is the responsible for the threatment of all Actuator related rpcs
 */

class ActuatorService : public BaseService, public Actuator::ActuatorService::Service
{
public:
    ActuatorService(QString address, World* world); /*!< @note Remember that this method needs to cast registerService() method
                                                      @param address The address which the service will listen at
                                                      @param world The world pointer which the service will cast requests and store data
                                                    */
    QString name();

    grpc::Status SetControl(grpc::ServerContext *context, const ControlPacket *request, google::protobuf::Empty *response);
    grpc::Status SetControls(grpc::ServerContext *context, grpc::ServerReader< ::ControlPacket> *reader, google::protobuf::Empty *response);
    grpc::Status GetControls(grpc::ServerContext *context, const google::protobuf::Empty *request, grpc::ServerWriter< ::ControlPacket> *writer);
};

#endif // ACTUATORSERVICE_H
