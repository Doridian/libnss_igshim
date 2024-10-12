#!/bin/sh

set -ex

mkdir -p /var/run/nscd

mkdir -p /var/run/kanidm-unixd /var/cache/kanidm-unixd /var/lib/kanidm-unixd && \
chown -R kanidm-unixd:kanidm-unixd /var/run/kanidm-unixd /var/cache/kanidm-unixd /var/lib/kanidm-unixd

/usr/sbin/nscd &
sudo -u kanidm-unixd kanidm_unixd &
kanidm_unixd_tasks &

set +x

cd /src
exec /bin/ash
