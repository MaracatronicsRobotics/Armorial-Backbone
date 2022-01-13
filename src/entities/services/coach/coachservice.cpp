#include "coachservice.h"

CoachService::CoachService(QString address) : BaseService(address) {
    registerService(this);
}

QString CoachService::name() {
    return "Coach";
}

grpc::Status CoachService::GetRobot(grpc::ServerContext* context, const RobotIdentifier* identifier,
                          Robot* robot) {

//    mutex.lock();
//    for(int i = 0; i < robots.size(); i++) {
//        if(robots.at(i).robotidentifier().robotid() == identifier->robotid()
//                && (robots.at(i).robotidentifier().robotcolor().isblue() == identifier->robotcolor().isblue())) {
//            Robot robozinho = robots.at(i);
//            robot = &robozinho;
//            mutex.unlock();

//            return grpc::Status::OK;
//        }
//    }
//    mutex.unlock();

    return grpc::Status::CANCELLED;
}

grpc::Status CoachService::GetRobots(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                        grpc::ServerWriter<Robot>* writer) {

//    mutex.lock();
//    for (Robot r : robots) {
//        writer->Write(r);
//    }
//    mutex.unlock();

    return grpc::Status::OK;
}


