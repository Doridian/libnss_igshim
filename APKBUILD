# Maintainer: Mark Dietzer <git@doridian.net>
pkgname=libnss_igshim
pkgver=1.1.0
pkgrel=0
pkgdesc="Shim to provide initgroups if only getgrent is present"
url="https://github.com/Doridian/libnss_igshim"
arch="all"
options="!check" # no test suite
license="MIT"
source=""

build() {
    make clean
    make
}

package() {
    make DESTDIR="$pkgdir" install
}
