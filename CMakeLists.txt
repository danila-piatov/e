cmake_minimum_required(VERSION 3.0...4.0)
project(e LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

add_executable(e main.cpp
        vt100.h
        vt100.cc
        util.h
        util.cc
        terminal.h
        terminal.cc
        raw_mode.cc
        raw_mode.h
        signals.h
        signals.cc
        line.cc
        line.h
        editor.cc
        editor.h)
