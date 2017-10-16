FROM centos:7

RUN \
  yum install -y epel-release
RUN \
  yum install -y \
    sudo \
    xorg-x11-server-Xvfb \
    which \
    git \
    bzip2 \
    gcc \
    gcc-c++ \
    make \
    openssl-devel \
    readline-devel \
    zlib-devel \
    dejavu-sans-fonts \
    gnome-icon-theme \
    adwaita-gtk2-theme \
    vte3 \
    webkitgtk \
    webkitgtk3 \
    libwebkit2-gtk \
    clutter-gtk \
    gtksourceview3 \
    dbus-x11

RUN \
  systemd-machine-id-setup

RUN \
  useradd --user-group --create-home ruby-gnome2

RUN \
  echo "ruby-gnome2 ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome2

USER ruby-gnome2

RUN \
  git clone --depth 1 https://github.com/rbenv/rbenv.git ~/.rbenv
RUN \
  git clone --depth 1 https://github.com/rbenv/ruby-build.git \
    ~/.rbenv/plugins/ruby-build

ENV PATH=/home/ruby-gnome2/.rbenv/shims:/home/ruby-gnome2/.rbenv/bin:$PATH
ENV RUBY_VERSION=2.4.1
RUN \
  rbenv install ${RUBY_VERSION}
RUN \
  rbenv global ${RUBY_VERSION}

COPY . /home/ruby-gnome2/ruby-gnome2
RUN sudo chown -R ruby-gnome2:ruby-gnome2 ~/ruby-gnome2
WORKDIR /home/ruby-gnome2/ruby-gnome2

RUN gem install bundler
RUN bundle install

CMD xvfb-run --server-args "-screen 0 640x480x24" bundle exec rake
