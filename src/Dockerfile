FROM ubuntu:22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Enable i386 architecture
RUN dpkg --add-architecture i386

# Install required packages, including Qt5 development libraries
RUN apt-get update
RUN apt-get install -y libc6:i386 lib32gcc-s1 gdb gcc make cmake g++ python3 python3-dev zlib1g-dev
RUN apt-get install -y git vim build-essential tzdata
RUN apt-get install -y libssl-dev qtbase5-dev qttools5-dev qttools5-dev-tools libcapstone-dev
RUN apt-get install -y flex bison
RUN rm -rf /var/lib/apt/lists/*

# Clone and build Boomerang
RUN git clone https://github.com/BoomerangDecompiler/boomerang.git
WORKDIR /boomerang
RUN mkdir build
WORKDIR /boomerang/build
RUN cmake -DCapstone_INCLUDE_DIR=/usr/include/capstone -DCapstone_LIBRARY=/usr/lib/x86_64-linux-gnu/libcapstone.so ..
RUN make
RUN make install

# Set timezone to UTC
RUN ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

# Install PEDA for GDB
RUN git clone https://github.com/longld/peda.git ~/peda && \
    echo "source ~/peda/peda.py" >> ~/.gdbinit
