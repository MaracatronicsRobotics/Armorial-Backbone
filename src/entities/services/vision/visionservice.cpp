#include "visionservice.h"

VisionService::VisionService(QString address) : BaseService(address) {
    registerService(this);
}

QString VisionService::name() {
    return "Vision";
}

grpc::Status VisionService::SetRobot(::grpc::ServerContext *context, const ::Robot *robot, ::google::protobuf::Empty *response) {
    mutex.lock();

    RobotIdentifier robotID = robot->robotidentifier();
    for (Robot r : _robots) {
        RobotIdentifier rid = r.robotidentifier();
        if (robotID.robotid() == rid.robotid() && robotID.robotcolor().isblue() == rid.robotcolor().isblue()){
            //Set infos
            Position robotPosition = robot->robotposition();
            Angle robotOrientation = robot->robotorientation();
            Velocity robotVelocity = robot->robotvelocity();
            AngularSpeed robotAngularSpeed = robot->robotangularspeed();
            RobotStatus robotStatus = robot->robotstatus();
            ::google::protobuf::Timestamp packetTS = robot->packettimestamp();

            r.set_allocated_robotposition(&robotPosition);
            r.set_allocated_robotorientation(&robotOrientation);
            r.set_allocated_robotvelocity(&robotVelocity);
            r.set_allocated_robotangularspeed(&robotAngularSpeed);
            r.set_allocated_robotstatus(&robotStatus);
            r.set_allocated_packettimestamp(&packetTS);

            mutex.unlock();

            return grpc::Status::OK;
        }
    }

    mutex.unlock();

    return grpc::Status::CANCELLED;
}

grpc::Status VisionService::SetRobots(::grpc::ServerContext *context, ::grpc::ServerReader< ::Robot> *reader, ::google::protobuf::Empty *response){
    mutex.lock();

    Robot robot;

    while (reader->Read(&robot)) {
        RobotIdentifier robotID = robot.robotidentifier();
        for (Robot r : _robots) {
            RobotIdentifier rid = r.robotidentifier();
            if (robotID.robotid() == rid.robotid() && robotID.robotcolor().isblue() == rid.robotcolor().isblue()){
                //Set infos
                Position robotPosition = robot.robotposition();
                Angle robotOrientation = robot.robotorientation();
                Velocity robotVelocity = robot.robotvelocity();
                AngularSpeed robotAngularSpeed = robot.robotangularspeed();
                RobotStatus robotStatus = robot.robotstatus();
                ::google::protobuf::Timestamp packetTS = robot.packettimestamp();

                r.set_allocated_robotposition(&robotPosition);
                r.set_allocated_robotorientation(&robotOrientation);
                r.set_allocated_robotvelocity(&robotVelocity);
                r.set_allocated_robotangularspeed(&robotAngularSpeed);
                r.set_allocated_robotstatus(&robotStatus);
                r.set_allocated_packettimestamp(&packetTS);
            }
        }
    }

    mutex.unlock();

    return grpc::Status::OK;
}

grpc::Status VisionService::SetBall(::grpc::ServerContext *context, const ::Ball *ball, ::google::protobuf::Empty *response){
    mutex.lock();

    Position ballPos = ball->ballposition();
    Velocity ballVel = ball->ballvelocity();

    _ball.set_allocated_ballposition(&ballPos);
    _ball.set_allocated_ballvelocity(&ballVel);

    mutex.unlock();

    return grpc::Status::OK;
}

grpc::Status VisionService::SetField(::grpc::ServerContext *context, const ::Field *field, ::google::protobuf::Empty *response){
    mutex.lock();

    /*
     * Setting Field Lines and Arcs: Begin
    */

    FieldLineSegment* fieldLinesPointer = _field.add_fieldlines();
    FieldCircularArc* fieldArcsPointer = _field.add_fieldarcs();

    ::google::protobuf::RepeatedPtrField<FieldLineSegment> fieldLines = field->fieldlines();
    ::google::protobuf::RepeatedPtrField<FieldCircularArc> fieldArcs = field->fieldarcs();

    FieldLineSegment* lines = fieldLines.Add();
    FieldCircularArc* arcs = fieldArcs.Add();

    Position p1 = lines->p1();
    Position p2 = lines->p2();

    Position center = arcs->center();

    fieldLinesPointer->set_allocated_p1(&p1);
    fieldLinesPointer->set_allocated_p2(&p2);
    fieldLinesPointer->set_thickness(lines->thickness());

    fieldArcsPointer->set_allocated_center(&center);
    fieldArcsPointer->set_a1(arcs->a1());
    fieldArcsPointer->set_a2(arcs->a2());
    fieldArcsPointer->set_thickness(arcs->thickness());

    /*
     * Setting Field Lines and Arcs: End
     *
     * Setting Field Sizes: Begin
    */

    _field.set_fieldlength(field->fieldlength());
    _field.set_fieldwidth(field->fieldwidth());
    _field.set_goalwidth(field->goalwidth());
    _field.set_goaldepth(field->goaldepth());
    _field.set_boundarywidth(field->boundarywidth());

    /*
     * Setting Field Sizes: End
    */

    mutex.unlock();

    return grpc::Status::OK;
}
