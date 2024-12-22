FROM ubuntu:22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Enable i386 architecture
RUN dpkg --add-architecture i386

# Install required packages
RUN apt-get update && apt-get install -y \
    libc6:i386 lib32gcc-s1 gdb gcc make cmake g++ python3 python3-dev zlib1g-dev \
    git vim build-essential tzdata libssl-dev qtbase5-dev qttools5-dev qttools5-dev-tools \
    libcapstone-dev flex bison bsdiff wget unzip xz-utils && \
    apt-get clean

# Set timezone to UTC
RUN ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

# Install PEDA for GDB
RUN git clone https://github.com/longld/peda.git ~/peda && \
    echo "source ~/peda/peda.py" >> ~/.gdbinit

# Install Radare2
WORKDIR /etc
RUN git clone https://github.com/radareorg/radare2 && \
    cd radare2 && \
    sys/install.sh && \
    cd .. && \
    rm -rf radare2

# Download and extract prebuilt RetDec binary
RUN wget -q https://github.com/avast/retdec/releases/download/v4.0/retdec-v4.0-ubuntu-64b.tar.xz && \
    tar -xf retdec-v4.0-ubuntu-64b.tar.xz -C /opt && \
    rm retdec-v4.0-ubuntu-64b.tar.xz

# Add RetDec to PATH
ENV PATH="/opt/retdec/bin:$PATH"

# Wrapper script for easier decompilation
RUN echo '#!/bin/bash\n'\
'retdec-decompiler.py --output /workspace/decompiled.c "$1" && \n'\
'cat /workspace/decompiled.c\n' > /usr/local/bin/decompile && \
    chmod +x /usr/local/bin/decompile

RUN mkdir /workspace

# Default command
CMD ["/bin/bash"]
