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
#include "coachservice.h"

CoachService::CoachService(QString address, World* world) : BaseService(address, world) {
    registerService(this);
}

QString CoachService::name() {
    return "Coach";
}

grpc::Status CoachService::GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier,
                          Robot* robot) {

    Robot robotData = getWorld()->getRobotData(identifier);
    *robot = robotData;

    if(robotData.robotidentifier().robotid() == ROBOT_INVALID_ID) {
        return grpc::Status::CANCELLED;
    }

    return grpc::Status::OK;
}

grpc::Status CoachService::GetRobots(grpc::ServerContext* context, const Color* teamColor,
                        grpc::ServerWriter<Robot>* writer) {

    QList<Robot> robots = getWorld()->getRobotsFromColor(teamColor);
    for(Robot r : robots) {
        writer->Write(r);
    }

    return grpc::Status::OK;
}

grpc::Status CoachService::GetBall(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                          Ball* ball) {

    *ball = getWorld()->getBallData();

    return grpc::Status::OK;
}

grpc::Status CoachService::GetField(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                          Field* field) {

    *field = getWorld()->getFieldData();

    return grpc::Status::OK;
}
