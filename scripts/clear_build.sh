#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")"; cd ..; pwd)"

rm -rf ${PROJECT_ROOT}/workspace/build/*
