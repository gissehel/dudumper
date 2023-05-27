FROM ubuntu as builder

ENV DEBIAN_FRONTEND noninteractive

RUN apt update && \
    apt install -y g++ make git apt-transport-https curl perl gnupg2 && \
    curl -sL https://deb.nodesource.com/setup_16.x | bash - && \
    apt-get install -y nodejs && \
    npm install -g npm && \
    mkdir -p /work
COPY . /work
RUN cd /work/web && \
    make clean all && \
    cd /work/src && \
    make clean all && \
    echo "All is OK..."

FROM scratch
COPY --from=builder "/work/src/dudumper" /dudumper
ENTRYPOINT [ "/dudumper" ]
