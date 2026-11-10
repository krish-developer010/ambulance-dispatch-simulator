cmake_minimum_required(VERSION 3.16)

project(AmbulanceDispatch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ambulance_dispatch
    src/main.cpp
    src/Graph.cpp
    src/RoutingEngine.cpp
    src/EmergencyQueue.cpp
    src/AmbulanceManager.cpp
    src/HospitalManager.cpp
    src/DispatchEngine.cpp
    src/Reports.cpp
)

target_include_directories(ambulance_dispatch PRIVATE include)

include(CTest)

if(BUILD_TESTING)
    add_executable(ambulance_tests
        tests/tests.cpp
        src/Graph.cpp
        src/RoutingEngine.cpp
        src/EmergencyQueue.cpp
        src/AmbulanceManager.cpp
        src/HospitalManager.cpp
        src/DispatchEngine.cpp
        src/Reports.cpp
    )

    target_include_directories(ambulance_tests PRIVATE include)
    add_test(NAME ambulance_tests COMMAND ambulance_tests)
endif()
