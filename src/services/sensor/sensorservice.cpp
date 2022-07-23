#include "sensorservice.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

SensorService::SensorService(QString serviceAddress, quint16 servicePort) : BaseService<Armorial::Sensor::SensorService>(serviceAddress, servicePort) {
    //
}

grpc::Status SensorService::SetRobotsStatus(grpc::ServerContext* context, grpc::ServerReader<Armorial::RobotStatus>* reader, google::protobuf::Empty* response) {
    _mutex.lock();
    // Clear old robot status list
    _robotsStatus.clear();

    // Receive upcoming packets from client
    Armorial::RobotStatus robotStatus;
    while(reader->Read(&robotStatus)) {
        // Send received robot status to list
        _robotsStatus.push_back(robotStatus);
    }

    // Send robot status update to vision
    emit sendRobotsStatus(_robotsStatus);

    _mutex.unlock();

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status SensorService::GetRobotsStatus(grpc::ServerContext* context, const google::protobuf::Empty* request, grpc::ServerWriter< ::Armorial::RobotStatus>* writer) {
    // If has no robot status to send, cancel the connection
    _mutex.lock();
    bool hasStatuses = _robotsStatus.size();
    _mutex.unlock();

    if(!hasStatuses) {
        return grpc::Status::CANCELLED;
    }

    _mutex.lock();
    // Iterate over the robot status list
    for (auto &rs : _robotsStatus) {
        writer->Write(rs);
    }
    _mutex.unlock();

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status SensorService::Check(grpc::ServerContext* context, const Armorial::HealthCheckRequest* request, Armorial::HealthCheckResponse* response) {
    response->set_status(response->SERVING);

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status SensorService::Watch(grpc::ServerContext *context, const Armorial::HealthCheckRequest *request, grpc::ServerWriter<Armorial::HealthCheckResponse> *writer) {
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "Not implemented.");
}

void SensorService::initialization() {
    if(initializeService()) {
        spdlog::info("[{}] Initialized service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
    }
    else {
        spdlog::error("[{}] Failed to initialize the service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        return ;
    }
}

void SensorService::loop() {
    //
}

void SensorService::finalization() {
    spdlog::info("[{}] Finished service thread.", serviceName().toStdString());
}
