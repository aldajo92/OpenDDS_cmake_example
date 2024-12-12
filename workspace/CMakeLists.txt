cmake_minimum_required(VERSION 3.3...3.27)
project(opendds_cmake_project CXX)

# Find OpenDDS
find_package(OpenDDS REQUIRED)

# IDL TypeSupport Library
add_library(example_idl)
opendds_target_sources(example_idl PUBLIC "Example.idl")
target_link_libraries(example_idl PUBLIC OpenDDS::Dcps)

# Define the libraries to link
set(opendds_libs
  OpenDDS::Dcps # Core OpenDDS Library
  example_idl
)

# Publisher executable
add_executable(publisher
  src/Publisher.cpp
  src/Publisher.h
  src/publisher_main.cpp
)
target_link_libraries(publisher ${opendds_libs})

# Subscriber executable
add_executable(subscriber
  src/Subscriber.cpp
  src/Subscriber.h
  src/subscriber_main.cpp
)
target_link_libraries(subscriber ${opendds_libs})
