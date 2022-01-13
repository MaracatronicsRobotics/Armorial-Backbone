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

#ifndef BASE_SERVICE_H
#define BASE_SERVICE_H

#include <src/entities/entity.h>
#include <grpc/grpc.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <include/messages.pb.h>
#include <include/coachservice.grpc.pb.h>
#include <include/visionservice.grpc.pb.h>

class BaseService : public Entity {
public:
    BaseService(QString address);
    virtual QString name() = 0;

protected:
    QString _address;
    grpc::Service *_service;
    grpc::ServerBuilder _serverBuilder;
    std::unique_ptr<grpc::Server> _server;

    void registerService(grpc::Service *service);

private:
    // Virtual methods
    void initialization();
    void loop();
    void finalization();
};

#endif // BASE_SERVICE_H
