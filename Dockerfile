FROM debian:stretch

RUN apt-get update && apt-get -qq install -y build-essential valgrind
RUN mkdir /src
WORKDIR /src