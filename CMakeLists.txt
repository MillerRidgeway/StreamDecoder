cmake_minimum_required(VERSION 3.10)
project(StreamDecoder)

set(CMAKE_CXX_STANDARD 11)

add_executable(StreamDecoder main.cpp pkt_decoder.h pkt_decoder.cpp)
