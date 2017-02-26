HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `env PKG_CONFIG_PATH=/usr/local/lib/pkgconfig pkg-config --libs grpc++ grpc libmacaroons`       \
		   -lgrpc++_reflection \
		   -lprotobuf -lpthread -ldl
else
LDFLAGS += -L/usr/local/lib `env PKG_CONFIG_PATH=/usr/local/lib/pkgconfig pkg-config --libs grpc++ grpc libmacaroons`       \
		   -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
		   -lprotobuf -lpthread -ldl 
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = .

vpath %.proto $(PROTOS_PATH)

all: proto bakery_server

proto:
	protoc ./bakery.proto --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/$(GRPC_CPP_PLUGIN)

bakery_server: bakery.grpc.pb.o bakery.grpc.pb.h  bakery.pb.o bakery_server.o
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.gch *.o *.pb.cc *.pb.h bakery_server
