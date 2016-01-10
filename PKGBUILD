# Maintainer: Sebastian Johansson <chloride@devurandom.net>
pkgbase='mediacourier'
pkgname=('mediacourier-transmission-server' 'mediacourier-hiveprotobuf')
pkgver() {
  cd "$pkgname"
  printf "git.r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}
pkgrel=1
pkgdesc="Streaming media server with small footprint and RTMP support"
url="https://github.com/ChlorideCull/MediaCourier"
arch=()
license=('unknown')
depends=()
optdepends=()
makedepends=('protobuf3')
conflicts=()
replaces=()
backup=()
install=
source=("git@github.com:ChlorideCull/MediaCourier.git")
md5sums=('SKIP')

build() {
  cd "${srcdir}"
  mkdir -p "build"
  cd "build"
  cmake ../ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
  make
}

package_mediacourier-transmission-server() {
  arch=('i686' 'x86_64')
  depends=('protobuf3' 'ffmpeg')
  
  cd "${srcdir}/build"
  cmake ../ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DINSTALL_TRNSMSSNSRV=ON
  make DESTDIR="${pkgdir}" install
}

package_mediacourier-hiveprotobuf() {
  arch=('any')
  depends=('protobuf3')
  
  cd "${srcdir}/build"
  cmake ../ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DINSTALL_HIVEPROTO=ON
  make DESTDIR="${pkgdir}" install
}


# vim:set ts=2 sw=2 et:
