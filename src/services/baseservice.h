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

#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <Armorial/Base/Service/Service.h>
#include <Armorial/Threaded/Entity/Entity.h>

#include <spdlog/spdlog.h>

template <typename T>
class BaseService : public Threaded::Entity, public Base::GRPC::Service<T>
{
public:
    /*!
     * \brief Override for the Base::GRPC::Service constructor.
     * \param serviceAddress The address which the implementation of the service will serve to.
     * \param servicePort The port which the implementation of the service will serve to.
     */
    BaseService(QString serviceAddress, quint16 servicePort) : Base::GRPC::Service<T>(serviceAddress, servicePort) { }

private:
    /*!
     * \brief Override from Threaded::Entity that allows the children to initialize the service and its dependencies.
     */
    virtual void initialization() = 0;

    /*!
     * \brief Override from Threaded::Entity that allows the children to enable threaded work scope.
     */
    virtual void loop() = 0;

    /*!
     * \brief Override from Threaded::Entity that allows the children to finalize the service and its dependencies.
     */
    virtual void finalization() = 0;
};

#endif // BASESERVICE_H
