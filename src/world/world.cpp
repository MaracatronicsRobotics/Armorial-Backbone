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

#include "world.h"

World::World() {
    // Initialize robots map
    _robots.insert(true, new QList<Robot>());
    _robots.insert(false, new QList<Robot>());

}

void World::setRobotsData(QList<Robot> robotList) {
    // Check if the robot list contains any data
    if(!robotList.size()) {
        std::cout << "World::setRobotsData(QList<Robot>&) list not contains any element.\n";
        return ;
    }

    _robotMutex.lockForWrite();

    for(int i = 0; i < robotList.size(); i++) {
        // Get the robot list for the specified color
        QList<Robot>* robotColorList = _robots.value(robotList.at(i).robotidentifier().robotcolor().isblue());

        bool found = false;
        for(int j = 0; j < robotColorList->size(); j++) {
            if(robotColorList->at(j).robotidentifier().robotid() == robotList.at(i).robotidentifier().robotid()) {
                // Replace
                robotColorList->replace(j, robotList.at(i));
                found = true;
            }
        }

        if(!found) {
            robotColorList->push_back(robotList.at(i));
        }

    }

    _robotMutex.unlock();
}

void World::setRobotData(const Robot* robot) {
    _robotMutex.lockForWrite();

    // Get the robots list for the specified color
    QList<Robot>* robotList = _robots.value(robot->robotidentifier().robotcolor().isblue());
    bool found = false;
    for(int i = 0; i < robotList->size(); i++) {
        // If found the id in the list
        if(robotList->at(i).robotidentifier().robotid() == robot->robotidentifier().robotid()) {
            // Replace
            robotList->replace(i, *robot);
            found = true;
        }
    }

    // If data is not found, insert it
    if(!found) {
        robotList->push_back(*robot);
    }

    _robotMutex.unlock();
}

void World::setRobotStatus(const RobotStatus *robotStatus) {
    _robotMutex.lockForWrite();

    QList<Robot>* robotList = _robots.value(robotStatus->robotidentifier().robotcolor().isblue());
    RobotIdentifier rsid = robotStatus->robotidentifier();
    for (Robot r : *robotList) {
        RobotIdentifier rid = r.robotidentifier();

        if ((rid.robotid() == rsid.robotid()) && rid.robotcolor().isblue() == rsid.robotcolor().isblue()){
            // set Information
            std::string stream = robotStatus->SerializeAsString();
            RobotStatus* rsptr = new RobotStatus();
            rsptr->ParseFromString(stream);
            r.set_allocated_robotstatus(rsptr);
        }
    }

    _robotMutex.unlock();
}

void World::setRobotsStatus(QList<RobotStatus> robotStatusList) {    
    // Check if the robot list contains any data
    if(!robotStatusList.size()) {
        std::cout << "World::setRobotsStatus(QList<RobotStatus> robotStatusList) list does not contain any element.\n";
        return ;
    }

    _robotMutex.lockForWrite();

    for (RobotStatus rs : robotStatusList) {
        RobotIdentifier rsid = rs.robotidentifier();
        QList<Robot>* robotList = _robots.value(rs.robotidentifier().robotcolor().isblue());
        for (Robot r : *robotList) {
            RobotIdentifier rid = r.robotidentifier();
            if (rid.robotid() == rsid.robotid() && rid.robotcolor().isblue() == rsid.robotcolor().isblue()){
                // Match
                std::string stream = rs.SerializeAsString();
                RobotStatus* rsptr = new RobotStatus();
                rsptr->ParseFromString(stream);
                r.set_allocated_robotstatus(rsptr);
            }
        }
    }

    _robotMutex.unlock();

}

void World::setFieldData(const Field* field) {
    _fieldMutex.lockForWrite();

    // Replace
    _field = *field;

    _fieldMutex.unlock();
}

void World::setBallData(const Ball* ball) {
    _ballMutex.lockForWrite();

    // Replace
    _ball = *ball;

    _ballMutex.unlock();
}

void World::setControlPacket(ControlPacket controlPacket) {
    _robotMutex.lockForWrite();

    _controlPackets.push_back(controlPacket);

    _robotMutex.unlock();
}

void World::setControlPacketsData(QList<ControlPacket> controlPacketList) {
    _robotMutex.lockForWrite();

    _controlPackets.append(controlPacketList);

    _robotMutex.unlock();
}

QList<Robot> World::getRobotsFromColor(const Color* color) {
    _robotMutex.lockForRead();

    QList<Robot> robots = (*_robots.value(color->isblue()));

    _robotMutex.unlock();

    return robots;
}

QList<ControlPacket> World::getRobotsControlPacket() {
    _robotMutex.lockForRead();

    QList<ControlPacket> packets = _controlPackets;
    _controlPackets.clear();

    _robotMutex.unlock();

    return packets;
}

Robot* World::getRobotData(const RobotIdentifier* identifier) {
    _robotMutex.lockForRead();

    QList<Robot>* robots = _robots.value(identifier->robotcolor().isblue());
    Robot* robotData = nullptr;
    for(int i = 0; i < robots->size(); i++) {
        if(robots->at(i).robotidentifier().robotid() == identifier->robotid()) {
            Robot data = robots->at(i);
            robotData = &data;
        }
    }

    _robotMutex.unlock();

    return robotData;
}

Field* World::getFieldData() {
    _fieldMutex.lockForRead();

    Field* fieldData = &_field;

    _fieldMutex.unlock();

    return fieldData;
}

Ball* World::getBallData() {
    _ballMutex.lockForRead();

    Ball* ballData = &_ball;

    _ballMutex.unlock();

    return ballData;
}
