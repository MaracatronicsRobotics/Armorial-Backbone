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

#include <QMap>

#include <src/services/baseservice.h>
#include <include/proto/actuatorservice.grpc.pb.h>

/*!
 * \brief Provides a implementation for the Armorial::Actuator::ActuatorService GRPC interface.
 */
class ActuatorService : public BaseService<Armorial::Actuator::ActuatorService>
{
    Q_OBJECT
public:
    /*!
     * \brief ActuatorService class constructor that overrides parent constructor.
     * \param serviceAddress The address which this grpc service will serve to.
     * \param servicePort The port which this grpc service will serve to.
     */
    ActuatorService(QString serviceAddress, quint16 servicePort);

    /*!
     * \brief Implementation of rpc `SetControls` that allows a client to send several control packet
     * for the robots in the team through this rpc.
     * \param context The server context.
     * \param reader A server reader to read the client packets.
     * \param response The server response to the request (empty because the grpc::Status will be returned to the client).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status SetControls(grpc::ServerContext* context, grpc::ServerReader<Armorial::ControlPacket>* reader, google::protobuf::Empty* response);

    /*!
     * \brief Implementation of rpc `GetControls` that allows a client to read all the stored control packets.
     * \param context The server context.
     * \param request A empty request.
     * \param writer A server writer to send all the previous control packets to the client.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status GetControls(grpc::ServerContext* context, const google::protobuf::Empty* request, grpc::ServerWriter<Armorial::ControlPacket>* writer);

    /*!
     * \brief Implementation of gRPC service health checking.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status Check(grpc::ServerContext* context, const Armorial::HealthCheckRequest* request, Armorial::HealthCheckResponse* response);

    /*!
     * \brief Implementation of gRPC service health checking.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status Watch(grpc::ServerContext* context, const Armorial::HealthCheckRequest* request, grpc::ServerWriter<Armorial::HealthCheckResponse>* writer);

private:
    /*!
     * \brief Override from BaseService::initialization() to initialize the service and its dependencies.
     */
    void initialization();

    /*!
     * \brief Override from BaseService::loop() to enable threaded work scope.
     */
    void loop();

    /*!
     * \brief Override from BaseService::finalization() to finalize the service and its dependencies.
     */
    void finalization();

    /*!
     * \brief Checks if has available control packets in the map.
     * \return True if contain any control packet in the map and False otherwise.
     */
    bool hasControlPackets();

    // Internal members
    QMap<bool, QMap<quint8, Armorial::ControlPacket>> _controlPackets;
    QMutex _mutex;
};

#endif // ACTUATORSERVICE_H
