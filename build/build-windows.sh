#!/bin/sh

export LANG=C
export DEBIAN_FRONTEND=noninteractive

ruby_versions="2.0.0-p598 2.1.5 2.2.0-preview1"
export RUBY_CC_VERSION="2.0.0:2.1.5:2.2.0"

N_CPUS=$(grep '^processor' /proc/cpuinfo | wc -l)
export MAKE_N_JOBS=${N_CPUS}

export RUBYLIB="$(pwd)/pkg-config/lib"

run()
{
  "$@"
  if [ $? -ne 0 ]; then
    echo "Failed $@"
    exit 1
  fi
}

run sudo sed -i'' -e 's,http://us,http://jp,g' /etc/apt/sources.list
run sudo apt-get update

echo ttf-mscorefonts-installer \
     msttcorefonts/accepted-mscorefonts-eula \
     select true | \
  run sudo debconf-set-selections

run \
  sudo apt-get install -y -V \
  git \
  ruby \
  ruby-dev \
  build-essential \
  pkg-config \
  autoconf \
  libtool \
  gettext \
  intltool \
  gtk-doc-tools \
  libffi-dev \
  libglib2.0-dev \
  libgdk-pixbuf2.0-dev \
  gobject-introspection \
  libgtk2.0-bin \
  flex \
  bison \
  python-dev \
  wine1.6 \
  mingw-w64

run git clone file:///pkg-config/.git
run git clone file:///rcairo/.git rcairo.${DIRECTORY_SUFFIX}
run git clone file:///ruby-gnome2/.git ruby-gnome2.${DIRECTORY_SUFFIX}

run sudo gem install --no-document \
    rake \
    bundler \
    pkg-config \
    rake-compiler \
    mechanize \
    packnga

if [ ! -d ~/.wine/ ]; then
  run wineboot
  until [ -f ~/.wine/system.reg ]; do
    sleep 1
  done
  wine_home="z:/home/vagrant"
  wine_rcairo="${wine_home}/rcairo.${DIRECTORY_SUFFIX}"
  wine_ruby_gnome2="${wine_home}/ruby-gnome2.${DIRECTORY_SUFFIX}"
  bin_dir="vendor/local/bin"
  path=$(
    (
      echo -n "${wine_rcairo}/${bin_dir};";
      echo -n "${wine_ruby_gnome2}/glib2/${bin_dir};";
      echo -n "${wine_ruby_gnome2}/gdk_pixbuf2/${bin_dir};"
      echo -n "${wine_ruby_gnome2}/pango/${bin_dir};";
      echo -n "${wine_ruby_gnome2}/atk/${bin_dir};";
      echo -n "${wine_ruby_gnome2}/gtk2/${bin_dir};";
      echo -n "${wine_ruby_gnome2}/gdk3/${bin_dir};";
    ) | \
      sed -e 's,/,\\\\\\\\,g')
  run sed -i'' -r \
      -e "s,^(\"PATH\"=str\\(2\\):\"),\\1${path},g" \
      ~/.wine/system.reg
fi

run wget https://www.opengl.org/registry/api/GL/glext.h
run sudo install glext.h /usr/share/mingw-w64/include/GL/
run rm -f glext.h

if ! grep -q GL/glext /usr/share/mingw-w64/include/GL/gl.h; then
  cat <<EOF > /tmp/insert.txt

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;
#include <GL/glext.h>

EOF
  run sudo sed -i'' -e '/typedef void GLvoid;/ r /tmp/insert.txt' \
      /usr/share/mingw-w64/include/GL/gl.h
  run rm -f /tmp/insert.txt
fi

sudo chown -R "${USER}:${USER}" /var/lib/gems

for ruby_version in ${ruby_versions}; do
  run rake-compiler cross-ruby HOST="${BUILD_HOST}" VERSION="${ruby_version}"
done

run cd ~/rcairo.${DIRECTORY_SUFFIX}
run rake windows:gcc:dll:bundle windows:zlib:build

run cd ~/ruby-gnome2.${DIRECTORY_SUFFIX}/glib2
run rake native:build windows:build

run cd ~/ruby-gnome2.${DIRECTORY_SUFFIX}/gobject-introspection
run rake native:build

run cd ~/rcairo.${DIRECTORY_SUFFIX}
run rake windows:build
run rake cross compile native

run cd ~/ruby-gnome2.${DIRECTORY_SUFFIX}
run rake gem:windows:build

run mkdir -p /vagrant/pkg
run cp */pkg/*.gem /vagrant/pkg/
