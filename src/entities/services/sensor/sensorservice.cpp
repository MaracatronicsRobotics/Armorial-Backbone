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


#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "sensorservice.h"

SensorService::SensorService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString SensorService::name() {
    return "Sensor";
}

grpc::Status SensorService::SetRobotStatus(grpc::ServerContext* context, const RobotStatus* robotStatus, ::google::protobuf::Empty* request) {
    getWorld()->setRobotStatus(robotStatus);

    return grpc::Status::OK;
}

grpc::Status SensorService::SetAllRobotStatus(grpc::ServerContext* context, grpc::ServerReader<RobotStatus>* reader, ::google::protobuf::Empty* request) {
    RobotStatus rs;

    QList<RobotStatus> rsList;
    while (reader->Read(&rs)){
        rsList.push_back(rs);
    }

    getWorld()->setRobotsStatus(rsList);

    return grpc::Status::OK;
}
