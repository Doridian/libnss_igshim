FROM alpine:3.22

RUN echo '@testing https://dl-cdn.alpinelinux.org/alpine/edge/testing' >> "$ROOTFS_PATH/etc/apk/repositories"

RUN apk update && \
    apk add strace alpine-sdk sudo zsh openrc musl-nscd musl-nscd-dev musl-nscd-doc kanidm-openrc@testing kanidm-clients@testing kanidm-unixd-clients@testing kanidm-zsh-completion@testing

RUN rc-update add nscd default
RUN rc-update add kanidm-unixd default
RUN rc-update add kanidm-unixd-tasks default

RUN ln -s /bin/zsh /usr/bin/zsh

COPY rootfs/ /

ENTRYPOINT [ "/entrypoint.sh" ]
