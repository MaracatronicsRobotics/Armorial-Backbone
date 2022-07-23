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

#include <QCoreApplication>

#include <grpc++/health_check_service_interface.h>

#include <src/services/actuator/actuatorservice.h>
#include <src/services/sensor/sensorservice.h>
#include <src/services/vision/visionservice.h>

#include <Armorial/Utils/ExitHandler/ExitHandler.h>
#include <Armorial/Threaded/EntityManager/EntityManager.h>
#include <Armorial/Utils/ParameterHandler/ParameterHandler.h>

int main(int argc, char *argv[]) {
    // Setup application
    QCoreApplication a(argc, argv);

    // Setup ExitHandler
    Utils::ExitHandler::setApplication(&a);
    Utils::ExitHandler::setup();

    grpc::EnableDefaultHealthCheckService(true);

    // Start service manager
    Threaded::EntityManager *serviceManager = new Threaded::EntityManager();

    // Start ParameterHandler and load parameters file
    Utils::ParameterHandler parameterHandler;
    if(!parameterHandler.loadFile(PROJECT_PATH + QString("/src/parameters/parameters.json"))) {
        spdlog::error("Failed to load parameters file at {}.", parameterHandler.fileName().toStdString());
        exit(-1);
    }

    // Setup Actuator
    QString actuatorAddress = parameterHandler["Actuator"]["Address"].toString();
    quint16 actuatorPort = parameterHandler["Actuator"]["Port"].toUInt();

    ActuatorService *actuatorService = nullptr;
    serviceManager->addEntity(actuatorService = new ActuatorService(actuatorAddress, actuatorPort));

    // Setup Sensor
    QString sensorAddress = parameterHandler["Sensor"]["Address"].toString();
    quint16 sensorPort = parameterHandler["Sensor"]["Port"].toUInt();

    SensorService *sensorService = nullptr;
    serviceManager->addEntity(sensorService = new SensorService(sensorAddress, sensorPort));

    // Setup Vision
    QString visionAddress = parameterHandler["Vision"]["Address"].toString();
    quint16 visionPort = parameterHandler["Vision"]["Port"].toUInt();

    VisionService *visionService = nullptr;
    serviceManager->addEntity(visionService = new VisionService(visionAddress, visionPort));

    // Connect services
    QObject::connect(sensorService, &SensorService::sendRobotsStatus, visionService, &VisionService::receiveRobotsStatus);

    // Start services entities
    serviceManager->startEntities();

    // Hold app
    bool exec = a.exec();

    // Disable all entities
    serviceManager->disableEntities();

    return exec;
}
