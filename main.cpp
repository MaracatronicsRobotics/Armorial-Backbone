#include <QCoreApplication>
#include <include/CoachService.grpc.pb.h>
#include <grpc/grpc.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

class RouteGuideImpl final : public routeguide::RouteGuide::Service {
public:
    explicit RouteGuideImpl() {}
    grpc::Status GetFeature(grpc::ServerContext* context, const routeguide::Point* point,
                    routeguide::Feature* feature) override {
        feature->set_name("fon");
        feature->mutable_location()->CopyFrom(*point);
        return grpc::Status::CANCELLED;
    }
};

routeguide::Point MakePoint(long latitude, long longitude) {
    routeguide::Point p;
    p.set_latitude(latitude);
    p.set_longitude(longitude);
    return p;
}

class RouteGuideClient {
public:
    RouteGuideClient(std::shared_ptr<grpc::Channel> channel) : stub_(routeguide::RouteGuide::NewStub(channel)) {

    }

    void GetFeature() {
        routeguide::Point point;
        routeguide::Feature feature;
        point = MakePoint(409146138, -746188906);
        GetOneFeature(point, &feature);
        point = MakePoint(0, 0);
        GetOneFeature(point, &feature);
    }

private:
    bool GetOneFeature(const routeguide::Point& point, routeguide::Feature* feature) {
        grpc::ClientContext context;
        grpc::Status status = stub_->GetFeature(&context, point, feature);
        if (!status.ok()) {
            std::cout << "GetFeature rpc failed." << std::endl;
            return false;
        }
        if (!feature->has_location()) {
            std::cout << "Server returns incomplete feature." << std::endl;
            return false;
        }
        if (feature->name().empty()) {
            std::cout << "f\n";
        } else {
            std::cout << feature->name() << std::endl;
        }
        return true;
    }

    std::unique_ptr<routeguide::RouteGuide::Stub> stub_;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc >= 2) {
        std::string server_address("0.0.0.0:50051");
        RouteGuideImpl service;

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
        server->Wait();
    }
    else {
        RouteGuideClient guide(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
        guide.GetFeature();
    }

    return a.exec();
}
