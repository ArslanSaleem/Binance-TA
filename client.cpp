/*
 *
 * Copyright 2021, Bitlytics.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ticker.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using ticker::Integer;
using ticker::TickData;
using ticker::Ticker;

class TickerClient
{
public:
    TickerClient(std::shared_ptr<Channel> channel)
        : stub_(Ticker::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    google::protobuf::Timestamp EmaTick(const int &days)
    {
        // Data we are sending to the server.
        Integer request;
        request.set_value(days);

        // Container for the data we expect from the server.
        TickData reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->EmaTick(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.time();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return google::protobuf::Timestamp();
        }
    }

private:
    std::unique_ptr<Ticker::Stub> stub_;
};

int main(int argc, char **argv)
{
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    TickerClient ticker(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    int days{12};
    google::protobuf::Timestamp reply = ticker.EmaTick(days);
    std::cout << "Greeter received: " << reply.nanos() << std::endl;

    return 0;
}
