/*
 * Copyright 2021, Bitlytics
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "src/core/services/TickerService.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

void RunServer()
{
    // ServiceLoop service_loop;
    // service_loop.start_service_loop();

    std::string server_address("0.0.0.0:50051");
    TickerServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();

    return 0;
}
