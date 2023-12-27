cmake_minimum_required(VERSION 3.27)
project(DataStructuresProject C)

set(CMAKE_C_STANDARD 11)

#add_definitions(-DDEBUG)

add_executable(DataStructuresProject main.c
        Headers/Queue.h
        HeadersImplementation/Queue.c
        Headers/ActionUndo.h
        HeadersImplementation/ActionUndo.c
        Headers/strux.h
        Headers/MainWrapper.h
        HeadersImplementation/MainWrapper.c
        Headers/ActionUndoLink.h
        HeadersImplementation/ActionUndoLink.c
)
