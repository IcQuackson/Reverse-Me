FROM ubuntu:22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386
RUN apt-get update && \
    apt-get install -y libc6:i386 lib32gcc-s1 gdb gcc make python3 git vim build-essential tzdata && \
    rm -rf /var/lib/apt/lists/*

# Set timezone to UTC
RUN ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

RUN git clone https://github.com/longld/peda.git ~/peda && \
    echo "source ~/peda/peda.py" >> ~/.gdbinit