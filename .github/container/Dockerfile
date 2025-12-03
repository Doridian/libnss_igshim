FROM alpine:3.23

RUN apk add alpine-sdk musl-nscd-dev ca-certificates curl sudo

ARG PUID=1000

RUN echo 'user ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

RUN adduser -D -u ${PUID} user && \
    adduser user abuild && \
    rm -rf /home/user && \
    mkdir -p /home/user && \
    chown -R ${PUID}:${PGID} /home/user

COPY build.sh /build.sh

USER user:abuild
CMD [ "/build.sh" ]
