FROM gcc

# docker build -t tors-base .
# docker run --network="host" --rm -it tors-base /bin/bash

RUN apt-get clean && apt-get update && apt-get install -y locales
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && \
    locale-gen
ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8
ENV SHELL /bin/bash
ENV DEBIAN_FRONTEND noninteractive

# switch to bash within the container so ROS sourcing is easy in build commands
SHELL ["/bin/bash", "-c"]

#Install Git, curl and make
RUN apt-get update && \
    apt-get install -y curl make autoconf automake libtool g++ unzip && \
    apt-get install -y cmake && \
    apt-get install -y python3-dev python3-pip && \
    apt-get clean

#install protobuf
RUN mkdir /protobuf
WORKDIR /protobuf
RUN curl -L -o protobuf.zip https://github.com/protocolbuffers/protobuf/releases/download/v3.15.6/protobuf-cpp-3.15.6.zip 
RUN unzip protobuf.zip
WORKDIR /protobuf/protobuf-3.15.6
RUN ./configure
RUN make
RUN make check; exit 0
RUN make install
RUN ldconfig

#Copy ctors folder
RUN mkdir /ctors
COPY . /ctors
WORKDIR /ctors


#install requirements
RUN python3 -m pip install -r TORS/requirements
#RUN python3 -m pip install -r TORS/requirements-gym --no-cache-dir #--no-cache-dir to prevent out of memory errors
RUN python3 -m pip install -r TORS/requirements-visualizer


#Build cTORS
RUN mkdir agents && \
    mkdir TORS/log_tensorboard && \
    mkdir build
RUN python3 setup.py install

#Configure visualizer
WORKDIR /ctors/TORS/visualizer
ENV FLASK_APP main.py
ENV FLASK_ENV development
ENV FLASK_RUN_PORT=5005

WORKDIR /ctors

#Run run.py
#WORKDIR /ctors/TORS
#RUN python3 run.py

#Run run_gym.py
#WORKDIR /ctors/TORS
#RUN python3 run_gym.py

#Run visualizer
#WORKDIR /ctors/TORS/visualizer
#RUN python3 -m flask run
    

