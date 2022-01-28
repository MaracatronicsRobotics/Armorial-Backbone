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
#include "visionservice.h"

VisionService::VisionService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString VisionService::name() {
    return "Vision";
}

grpc::Status VisionService::SetRobot(::grpc::ServerContext *context, const ::Robot *robot, ::google::protobuf::Empty *response) {
    getWorld()->setRobotData(robot);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetRobots(::grpc::ServerContext *context, ::grpc::ServerReader< ::Robot> *reader, ::google::protobuf::Empty *response){
    Robot robot;
    QList<Robot> robotList;

    while (reader->Read(&robot)) {
        robotList.push_back(robot);
    }

    getWorld()->setRobotsData(robotList);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetBall(::grpc::ServerContext *context, const ::Ball *ball, ::google::protobuf::Empty *response){
    getWorld()->setBallData(ball);

    return grpc::Status::OK;
}

grpc::Status VisionService::SetField(::grpc::ServerContext *context, const ::Field *field, ::google::protobuf::Empty *response){
    getWorld()->setFieldData(field);

    return grpc::Status::OK;
}
