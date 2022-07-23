#include "actuatorservice.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

ActuatorService::ActuatorService(QString serviceAddress, quint16 servicePort) : BaseService<Armorial::Actuator::ActuatorService>(serviceAddress, servicePort) {
    //
}

grpc::Status ActuatorService::SetControls(grpc::ServerContext *context, grpc::ServerReader<Armorial::ControlPacket> *reader, google::protobuf::Empty *response) {
    _mutex.lock();

    // Read until the connection is stabilished
    Armorial::ControlPacket request;
    while(reader->Read(&request)) {
        // Take request robot identifier
        Armorial::RobotIdentifier identifier = request.robotidentifier();

        // Update packet in map
        _controlPackets[identifier.robotcolor().isblue()].insert(identifier.robotid(), request);
    }

    _mutex.unlock();

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status ActuatorService::GetControls(grpc::ServerContext *context, const google::protobuf::Empty *request, grpc::ServerWriter<Armorial::ControlPacket> *writer) {
    // If no control packets, finish connection.
    if(!hasControlPackets()) {
        return grpc::Status::CANCELLED;
    }

    _mutex.lock();
    // Iterate over the keys of the map
    for (auto &k : _controlPackets.keys()) {
        for(auto &kp : _controlPackets[k].keys()) {
            writer->Write(_controlPackets[k][kp]);
        }
    }

    // Clear packets
    _controlPackets.clear();
    _mutex.unlock();

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status ActuatorService::Check(grpc::ServerContext* context, const Armorial::HealthCheckRequest* request, Armorial::HealthCheckResponse* response) {
    response->set_status(response->SERVING);

    // Return ok status
    return grpc::Status::OK;
}

grpc::Status ActuatorService::Watch(grpc::ServerContext *context, const Armorial::HealthCheckRequest *request, grpc::ServerWriter<Armorial::HealthCheckResponse> *writer) {
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "Not implemented.");
}

void ActuatorService::initialization() {
    if(initializeService()) {
        spdlog::info("[{}] Initialized service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
    }
    else {
        spdlog::error("[{}] Failed to initialize the service at address '{}' and port '{}'.", serviceName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        return ;
    }

    // Start control packet maps for blue and yellow teams
    _controlPackets.insert(false, QMap<quint8, Armorial::ControlPacket>()); // yellow
    _controlPackets.insert(true, QMap<quint8, Armorial::ControlPacket>());  // blue
}

void ActuatorService::loop() {
    //
}

void ActuatorService::finalization() {
    spdlog::info("[{}] Finished service thread.", serviceName().toStdString());
}

bool ActuatorService::hasControlPackets() {
    _mutex.lock();

    // Check if has no control packets to return
    bool hasPackets = false;
    for (auto &k : _controlPackets.keys()) {
        hasPackets = hasPackets || !_controlPackets[k].isEmpty();
    }

    _mutex.unlock();

    return hasPackets;
}
