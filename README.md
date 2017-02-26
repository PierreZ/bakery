# bakery
A Macaroons factory built with gRPC

## Macaroons?

* [Macaroons: Cookies with Contextual Caveats for Decentralized Authorization in the Cloud](https://air.mozilla.org/macaroons-cookies-with-contextual-caveats-for-decentralized-authorization-in-the-cloud/)

## Why C++?

Because [the main implementation is written in C](https://github.com/rescrv/libmacaroons) and I don't want to write a lib in Rustlang. the actual rust implementation is not supporting macaroonV2 yet.

## Why gRPC?

Because I don't want to write too much dirty/unsafe C.

## Can I use it in production?

It's only a toy to play a bit with macaroons, don't assume that my code is actually working. 

## Dependencies

* [protobuf](https://github.com/google/protobuf)
* [gRPC](https://github.com/grpc/grpc) 
* [libmacaroons](https://github.com/rescrv/libmacaroons)

## Building bakery

```bash
make proto 
make bakery_server
```