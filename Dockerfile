# Dockerfile

FROM gcc AS build-setup
WORKDIR /code
COPY . .
RUN make setup

FROM debian:stable
ARG python=NULL
ARG gcc=NULL
ARG java=NULL
WORKDIR /code
COPY --from=build-setup /code/setup .
COPY src/sources.list /etc/apt
RUN apt-get update -y
RUN ./setup "${python}" "${gcc}" "${java}"
RUN rm -rf *
