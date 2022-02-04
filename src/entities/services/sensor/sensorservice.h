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


#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H

#include <src/entities/services/baseservice.h>
#include <proto/sensorservice.grpc.pb.h>

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
