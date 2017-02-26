#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

#include <macaroons.h>

#include "bakery.grpc.pb.h"
#include "bakery.pb.h"

using bakery::NewMacaroonRequest;
using bakery::MacaroonSerialized;
using bakery::Bakery;

class BakeryServiceImpl final : public Bakery::Service {
  Status CreateMacaroon(ServerContext* context, const NewMacaroonRequest* request, MacaroonSerialized* reply) override {

    macaroon_returncode ret;
    std::string location;
    location = "https://pierrezemb.fr";
    std::string secure_key;
    secure_key = "unicorn are always secret";
    std::string issuer_id;
    issuer_id = request->identifier();

    macaroon* m = macaroon_create(
      (const unsigned char*)location.c_str(), strlen(location.c_str()),
      (const unsigned char*)secure_key.c_str(), strlen(secure_key.c_str()),
      (const unsigned char*)issuer_id.c_str(), strlen(issuer_id.c_str()),
      &ret);

    // Adding first class caveats
    for(const std::string &caveat : request->first_party_caveats()){
      m = macaroon_add_first_party_caveat(m,
        (const unsigned char*)caveat.c_str(), strlen(caveat.c_str()),
        &ret));
    }
    // TODO: checking ret code
    size_t ms_size = macaroon_serialize_size_hint(m, MACAROON_V2);
    unsigned char* ms = (unsigned char *)malloc(ms_size);

    macaroon_serialize(m,MACAROON_V2, ms, ms_size, &ret);
    // TODO: checking ret code

    std::string str;
    str.append(reinterpret_cast<const char*>(macaroon_serialize));

    reply->set_serialized(str);
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  BakeryServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.$
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

int main(int argc, char** argv) {
  RunServer();

  return 0;
}