#include "visionservice.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

VisionService::VisionService(QString serviceAddress, quint16 servicePort) : BaseService<Armorial::Vision::VisionService>(serviceAddress, servicePort) {
    //
}

grpc::Status VisionService::SetRobots(grpc::ServerContext* context, grpc::ServerReader<Armorial::Robot>* reader, google::protobuf::Empty* response) {
    // Lock mutex for write
    _robotsMutex.lockForWrite();

    // Clear robots lists
    QList<bool> keys = _robots.keys();
    for(auto &k : keys) {
        _robots[k].clear();
    }

    // Take robots from reader
    Armorial::Robot robot;
    while(reader->Read(&robot)) {
        // Take RobotIdentifier
        Armorial::RobotIdentifier robotIdentifier = robot.robotidentifier();

        // Setup robot in its specific list
        _robots[robotIdentifier.robotcolor().isblue()].push_back(robot);
    }

    // Unlock mutex
    _robotsMutex.unlock();

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::SetBall(grpc::ServerContext* context, const Armorial::Ball* request, google::protobuf::Empty* response) {
    // Setup ball data
    _ball = *request;

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::SetField(grpc::ServerContext* context, const Armorial::Field* request, google::protobuf::Empty* response) {
    // Setup field data
    _field = *request;

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::GetRobots(grpc::ServerContext* context, const Armorial::Color* request, grpc::ServerWriter<Armorial::Robot>* writer) {
    // Get the robots from the requested color
    _robotsMutex.lockForRead();
    QList<Armorial::Robot> robots = _robots[request->isblue()];
    _robotsMutex.unlock();

    // If robots list size is empty, return cancelled status
    if(!robots.size()) {
        return grpc::Status::CANCELLED;
    }

    // Send all the robots through server writer
    for(auto &r : robots) {
        writer->Write(r);
    }

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::GetBall(grpc::ServerContext* context, const google::protobuf::Empty* request, Armorial::Ball* response) {
    // Set the ball data into the response pointer
    *response = _ball;

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::GetField(grpc::ServerContext* context, const google::protobuf::Empty* request, Armorial::Field* response) {
    // Set the field data into the response pointer
    *response = _field;

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::Check(grpc::ServerContext* context, const Armorial::HealthCheckRequest* request, Armorial::HealthCheckResponse* response) {
    response->set_status(response->SERVING);

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status VisionService::Watch(grpc::ServerContext *context, const Armorial::HealthCheckRequest *request, grpc::ServerWriter<Armorial::HealthCheckResponse> *writer) {
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "Not implemented.");
}

void VisionService::initialization() {
    if(initializeService()) {
        spdlog::info("[{}] Initialized service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
    }
    else {
        spdlog::error("[{}] Failed to initialize the service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        return ;
    }

    // Startup _robots list
    for(int i = 0; i <= 1; i++) {
        _robots[i] = QList<Armorial::Robot>();
    }
}

void VisionService::loop() {
    //
}

void VisionService::finalization() {
    spdlog::info("[{}] Finished service thread.", serviceName().toStdString());
}

void VisionService::receiveRobotsStatus(const QList<Armorial::RobotStatus>& robotsStatus) {
    // Lock mutex for write
    _robotsMutex.lockForWrite();

    // Iterate over received robotsStatus list
    for(auto &rs : robotsStatus) {
        // Take robot identifier
        const Armorial::RobotIdentifier statusIdentifier = rs.robotidentifier();

        // Try to search in the specific _robots list for the given robot
        const QList<Armorial::Robot> robots = _robots[statusIdentifier.robotcolor().isblue()];
        const int size = robots.size();
        for (int i = 0; i < size; i++) {
            // Take robot at index 'i'
            Armorial::Robot robot = robots.at(i);

            // If matched
            if(robot.robotidentifier().robotid() == statusIdentifier.robotid()) {
                // Create a Armorial::RobotStatus object as copy of the given one
                Armorial::RobotStatus *robotStatus = new Armorial::RobotStatus();
                robotStatus->CopyFrom(rs);

                // Allocate it into the robot instance
                robot.set_allocated_robotstatus(robotStatus);

                // Replace index 'i' with the given robot
                _robots[statusIdentifier.robotcolor().isblue()].replace(i, robot);

                // Exit from search (already found the desired robot)
                break;
            }
        }
    }

    // Unlock mutex
    _robotsMutex.unlock();
}
