# bakery
A Macaroons factory built with gRPC

## Macaroons?

* [Macaroons: Cookies with Contextual Caveats for Decentralized Authorization in the Cloud](https://air.mozilla.org/macaroons-cookies-with-contextual-caveats-for-decentralized-authorization-in-the-cloud/)

## Why C++?

Because [the main implementation is written in C](https://github.com/rescrv/libmacaroons) and I don't want to write a lib in Rustlang.

## Why gRPC?

Because I don't want to write too much dirty/unsafe C.

## Can I use it in production?

It's only a toy to play a bit with macaroons, don't assume that my code is actually working. 

### Generate proto reminder

```bash
protoc ./macaroons.proto --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin
```