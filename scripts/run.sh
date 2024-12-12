#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")"; cd ..; pwd)"
source ${PROJECT_ROOT}/config.sh

docker run -it --rm \
    --name=${DOCKER_CONTAINER_NAME} \
    --network ${DOCKER_NETWORK} \
    --volume ${PROJECT_ROOT}/workspace:/workspace \
    -- ${DOCKER_IMAGE_NAME}
