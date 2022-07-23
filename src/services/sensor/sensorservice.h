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

#ifndef SENSORSERVICE_H
#define SENSORSERVICE_H

#include <QMap>

#include <src/services/baseservice.h>
#include <include/proto/sensorservice.grpc.pb.h>

/*!
 * \brief Provides a implementation for the Armorial::Sensor::SensorService GRPC interface.
 */
class SensorService : public BaseService<Armorial::Sensor::SensorService>
{
    Q_OBJECT
public:
    /*!
     * \brief SensorService class constructor that overrides parent constructor.
     * \param serviceAddress The address which this grpc service will serve to.
     * \param servicePort The port which this grpc service will serve to.
     */
    SensorService(QString serviceAddress, quint16 servicePort);

    /*!
     * \brief Implementation of rpc `SetRobotsStatus` that allows a client to send a Armorial::RobotStatus
     * packet containing feedback from multiple robots.
     * \param context The server context.
     * \param reader A server reader to read the client packets.
     * \param response The server response to the request (empty because the grpc::Status will be returned to the client).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status SetRobotsStatus(grpc::ServerContext* context, grpc::ServerReader<Armorial::RobotStatus>* reader, google::protobuf::Empty* response);

    /*!
     * \brief Implementation of rpc `GetRobotsStatus` that allows a client to get the feedback status
     * from the robots.
     * \param context The server context.
     * \param request A empty request.
     * \param writer A server writer to send all the previous robot status packets to the client.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status GetRobotsStatus(grpc::ServerContext* context, const google::protobuf::Empty* request, grpc::ServerWriter< ::Armorial::RobotStatus>* writer);

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

    // Internal members
    QList<Armorial::RobotStatus> _robotsStatus;
    QMutex _mutex;

signals:
    void sendRobotsStatus(const QList<Armorial::RobotStatus>& robotsStatus);
};

#endif // SENSORSERVICE_H
