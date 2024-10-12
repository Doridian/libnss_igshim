#!/bin/sh
set -e

export HOME=/home/user

mkdir -p /home/user/.abuild
if [ ! -z "${ABUILD_PRIVATE_KEY-}" ]; then
    echo "$ABUILD_PRIVATE_KEY" > "/home/user/.abuild/${ABUILD_KEY_FILENAME}"
    #echo >> "/home/user/.abuild/${ABUILD_KEY_FILENAME}"
    chmod 600 "/home/user/.abuild/${ABUILD_KEY_FILENAME}"
fi

if [ ! -z "${ABUILD_PUBLIC_KEY-}" ]; then
    echo "$ABUILD_PUBLIC_KEY" > "/home/user/.abuild/${ABUILD_KEY_FILENAME}.pub"
    #echo >> "/home/user/.abuild/${ABUILD_KEY_FILENAME}.pub"
    sudo cp -fv /home/user/.abuild/*.pub /etc/apk/keys/
fi

echo "PACKAGER_PRIVKEY=\"/home/user/.abuild/${ABUILD_KEY_FILENAME}\"" > /home/user/.abuild/abuild.conf
echo "PACKAGER=\"${GIT_USER_NAME} <${GIT_USER_EMAIL}>\"" >> /home/user/.abuild/abuild.conf
echo 'REPODEST="/src/packages"' >> /home/user/.abuild/abuild.conf

rm -rf /src/packages && mkdir -p /src/packages

git config --global user.name "${GIT_USER_NAME}"
git config --global user.email "${GIT_USER_EMAIL}"

cd /src
abuild
