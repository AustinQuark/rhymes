# Use an appropriate base image with the desired OS and tools
FROM ubuntu:latest

# Set environment variables
ENV DEBIAN_FRONTEND="dialog" 
# noninteractive

# Install necessary packages and dependencies
RUN apt-get update \
    && apt-get install -y \
        gcc \
        g++ \  
        make \
        libsonic-dev \
        libtool \
        ronn \ 
        kramdown \
        cmake \
        automake \
        git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN git clone https://github.com/espeak-ng/espeak-ng.git /app/espeak-ng \ 
    && cd espeak-ng \
    && ./autogen.sh \
    && ./configure --prefix=/usr \
    && make

# Set the working directory inside the container

# Copy your C++ project files into the container
# COPY . .

# Build your C++ project (Replace with your build commands)
# RUN mkdir build \
#    && cd build \
#    && cmake .. \
#    && make

# Define the command to run your application
# CMD ["./path/to/your/executable"] # Replace with the path to your executable within the container
# CMD ["gcc test.cpp"]
