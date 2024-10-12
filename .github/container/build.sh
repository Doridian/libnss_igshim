#!/bin/sh
set -e

export HOME=/home/user

mkdir -p /home/user/.abuild
if [ ! -z "${ABUILD_PRIVATE_KEY-}" ]; then
    echo "$ABUILD_PRIVATE_KEY" > "/home/user/.abuild/abuild.key"
    chmod 600 "/home/user/.abuild/abuild.key"
fi

if [ ! -z "${ABUILD_PUBLIC_KEY-}" ]; then
    echo "$ABUILD_PUBLIC_KEY" > "/home/user/.abuild/abuild.key.pub"
    sudo cp -fv /home/user/.abuild/abuild.key.pub /etc/apk/keys/
fi

echo 'PACKAGER_PRIVKEY="/home/user/.abuild/abuild.key"' > /home/user/.abuild/abuild.conf
echo "PACKAGER=\"${GIT_USER_NAME} <${GIT_USER_EMAIL}>\"" >> /home/user/.abuild/abuild.conf
echo 'REPODEST="/src/packages"' >> /home/user/.abuild/abuild.conf

git config --global user.name "${GIT_USER_NAME}"
git config --global user.email "${GIT_USER_EMAIL}"

cd /src
abuild
