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

#ifndef VISIONSERVICE_H
#define VISIONSERVICE_H

#include <QMap>
#include <QReadWriteLock>

#include <src/services/baseservice.h>
#include <include/proto/visionservice.grpc.pb.h>

/*!
 * \brief Provides a implementation for the Armorial::Vision::VisionService GRPC interface.
 */
class VisionService : public BaseService<Armorial::Vision::VisionService>
{
    Q_OBJECT
public:
    /*!
     * \brief ActuatorService class constructor that overrides parent constructor.
     * \param serviceAddress The address which this grpc service will serve to.
     * \param servicePort The port which this grpc service will serve to.
     */
    VisionService(QString serviceAddress, quint16 servicePort);

    /*!
     * \brief Implementation of rpc `SetRobots` which allows to setup multiple robots information.
     * \param context The server context.
     * \param reader The client request (providing the robots data).
     * \param response The server response to the request (empty because the grpc::Status will be returned to the client).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status SetRobots(grpc::ServerContext* context, grpc::ServerReader<Armorial::Robot>* reader, google::protobuf::Empty* response);

    /*!
     * \brief Implementation of rpc `SetBall` which allows to setup the ball information.
     * \param context The server context.
     * \param request The client request (providing the ball data).
     * \param response The server response to the request (empty because the grpc::Status will be returned to the client).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status SetBall(grpc::ServerContext* context, const Armorial::Ball* request, google::protobuf::Empty* response);

    /*!
     * \brief Implementation of rpc `SetField` which allows to setup the field information.
     * \param context The server context.
     * \param request The client request (providing the field data).
     * \param response The server response to the request (empty because the grpc::Status will be returned to the client).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status SetField(grpc::ServerContext* context, const Armorial::Field* request, google::protobuf::Empty* response);

    /*!
     * \brief Implementation of rpc `GetRobots` which allows to retrieve robots information.
     * \param context The server context.
     * \param request The client request (providing the requested team color).
     * \param writer The server response to the request (sending robots data through a grpc::ServerWriter interface).
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status GetRobots(grpc::ServerContext* context, const Armorial::Color* request, grpc::ServerWriter<Armorial::Robot>* writer);

    /*!
     * \brief Implementation of rpc `GetBall` which allows to retrieve ball information.
     * \param context The server context.
     * \param request The client request (empty because the grpc::Status will be returned to the client).
     * \param response The server response to the request, returning a Armorial::Ball object which contains the ball data.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status GetBall(grpc::ServerContext* context, const google::protobuf::Empty* request, Armorial::Ball* response);

    /*!
     * \brief Implementation of rpc `GetField` which allows to retrieve field information.
     * \param context The server context.
     * \param request The client request (empty because the grpc::Status will be returned to the client).
     * \param response The server response to the request, returning a Armorial::Field object which contains the field data.
     * \return A grpc::Status returning the status of the request.
     */
    grpc::Status GetField(grpc::ServerContext* context, const google::protobuf::Empty* request, Armorial::Field* response);

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
    QMap<bool, QList<Armorial::Robot>> _robots;
    Armorial::Ball _ball;
    Armorial::Field _field;

    // Mutex for _robots access due to the signal-slot connection with SensorService class
    QReadWriteLock _robotsMutex;

public slots:
    /*!
     * \brief A slot implementation which receive a list of robots status and update
     * them in the robots list.
     * \param robotsStatus The given list of robots status.
     */
    void receiveRobotsStatus(const QList<Armorial::RobotStatus>& robotsStatus);
};

#endif // VISIONSERVICE_H
