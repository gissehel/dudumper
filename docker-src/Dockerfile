FROM alpine:3.7

RUN apk update && \
    apk add g++ make musl-dev git curl nodejs perl && \
    npm install -g coffeescript pug-cli stylus uglify-es && \
    cd /root && \
    git clone https://github.com/gissehel/dudumper && \
    cd /root/dudumper/web && \
    make clean all && \
    cd /root/dudumper/src && \
    make clean all && \
    echo "All is OK..."

ENTRYPOINT [ "/root/dudumper/src/dudumper" ]



