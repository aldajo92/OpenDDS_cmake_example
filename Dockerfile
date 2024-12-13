FROM ubuntu:20.04

ARG TARGETARCH

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    curl \
    gnupg \
    unzip \
    bash \
    build-essential \
    cmake \
    libssl-dev \
    libxerces-c-dev \
    libncurses5-dev \
    ca-certificates \
    gnupg \
    g++ \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Clone and build OpenDDS
RUN curl -L https://github.com/OpenDDS/OpenDDS/releases/download/DDS-3.30/OpenDDS-3.30.tar.gz -o opendds.tar.gz

RUN tar -xzf opendds.tar.gz -C /opt && \
    mv /opt/OpenDDS-3.30 /opt/OpenDDS && \
    cd /opt/OpenDDS && \
    ./configure && \
    make -j$(nproc)

RUN rm -f /opendds.tar.gz

# Verify OpenDDS installation
RUN $DDS_ROOT/tests/run_test.pl -h || true

RUN echo "export DDS_ROOT=/opt/OpenDDS" >> ~/.bashrc
RUN echo "export ACE_ROOT=\$DDS_ROOT/ACE_wrappers" >> ~/.bashrc
RUN echo "export PATH=\$DDS_ROOT/bin:\$PATH" >> ~/.bashrc
RUN echo "export LD_LIBRARY_PATH=\$DDS_ROOT/lib:\$ACE_ROOT/lib:\$LD_LIBRARY_PATH" >> ~/.bashrc

WORKDIR /workspace
ENTRYPOINT [ "/bin/bash" ]
