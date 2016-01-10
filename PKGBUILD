# Maintainer: Sebastian Johansson <chloride@devurandom.net>
pkgbase='mediacourier'
pkgname=('mediacourier-transmission-server' 'mediacourier-hiveprotobuf')
pkgver() {
  cd "${srcdir}/MediaCourier"
  printf "git.r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}
pkgrel=1
pkgdesc="Streaming media server with small footprint and RTMP support"
url="https://github.com/ChlorideCull/MediaCourier"
arch=('any')
license=('unknown')
depends=()
optdepends=()
makedepends=('protobuf3' 'cmake')
conflicts=()
replaces=()
backup=()
install=
source=("git+ssh://git@github.com/ChlorideCull/MediaCourier.git")
#source=("git+file:///home/chloridecull/Development/MediaCourier")
md5sums=('SKIP')

build() {
  cd "${srcdir}/MediaCourier"
  mkdir -p "build"
  cd "build"
  cmake ../ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
  make
  make preinstall
}

package_mediacourier-transmission-server() {
  arch=('i686' 'x86_64')
  depends=('protobuf3' 'ffmpeg')
  
  cd "${srcdir}/MediaCourier/build"
  cmake -DCOMPONENT=TransmissionServer -DCMAKE_INSTALL_PREFIX="${pkgdir}/usr" -P cmake_install.cmake
}

package_mediacourier-hiveprotobuf() {
  arch=('any')
  depends=('protobuf3')
  
  cd "${srcdir}/MediaCourier/build"
  cmake -DCOMPONENT=HiveProtobuf -DCMAKE_INSTALL_PREFIX="${pkgdir}/usr" -P cmake_install.cmake
}


# vim:set ts=2 sw=2 et:
