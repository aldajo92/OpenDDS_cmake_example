# OPENDDS and Cmake

## Build and Run Docker Container
~~~bash
./scrips/build.sh
./scripts/run.sh
~~~

## Compile project
~~~bash
cd /build
cmake ..
make
~~~

## Execute

~~~bash
DCPSInfoRepo -o repo.ior
~~~

## Run Publisher
Open a new terminal:
~~~bash
./scrips/bash.sh
~~~

Then execute Publisher:
~~~bash
./build/publisher
~~~

## Run Subscriber
Open a new terminal:
~~~bash
./scrips/bash.sh
~~~

Then execute Subscriber:
~~~bash
./build/subscriber
~~~