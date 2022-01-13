#include "baseservice.h"

BaseService::BaseService(QString address){
    _address = address;
    _service = nullptr;
}

void BaseService::registerService(grpc::Service *service) {
    if(service == nullptr) {
        std::cout << "BaseService::registerService(grpc::Service*) could not register an null service.\n";
        return ;
    }

    _service = service;
}

void BaseService::initialization() {
    if(_service == nullptr) {
        std::cout << "Could not find an suitable gRPC service for " + name().toStdString() + ". Have you used the registerService(grpc::Service) function?\n";
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
