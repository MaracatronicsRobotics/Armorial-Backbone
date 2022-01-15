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

#include "baseservice.h"

BaseService::BaseService(QString address, World* world){
    _address = address;
    _world = world;
    _service = nullptr;
}

void BaseService::registerService(grpc::Service *service) {
    if(service == nullptr) {
        std::cout << "BaseService::registerService(grpc::Service*) could not register an null service.\n";
        return ;
    }

    _service = service;
}

World* BaseService::getWorld() {
    if(_world == nullptr) {
        std::cout << "BaseService::getWorld() returned nullptr!\n";
    }

    return _world;
}

void BaseService::initialization() {
    if(_service == nullptr) {
        std::cout << "Could not find an suitable gRPC service for " + name().toStdString() + ". Have you used the registerService(grpc::Service) function?\n";
        return ;
    }

    if(_world == nullptr) {
        std::cout << "Could not initialize service for " + name().toStdString() + " because World is nullptr.\n";
        return ;
    }

    _serverBuilder.AddListeningPort(_address.toStdString(), grpc::InsecureServerCredentials());
    _serverBuilder.RegisterService(_service);
    _server = std::unique_ptr<grpc::Server>(_serverBuilder.BuildAndStart());

    if(_server == nullptr) {
        std::cout << "Error when opening " + name().toStdString() + " service.\n";
        return ;
    }

    std::cout << name().toStdString() + " Service listening on " << _address.toStdString() << std::endl;
}

void BaseService::loop() {

}

void BaseService::finalization() {
    _server->Shutdown();
    _server->Wait();
    std::cout << name().toStdString() + " Server shutdown." << std::endl;
}
