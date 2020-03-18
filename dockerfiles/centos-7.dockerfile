FROM centos:7

RUN \
  yum install -y epel-release && \
  yum install -y \
    adwaita-gtk2-theme \
    bzip2 \
    clutter-gtk \
    dbus-daemon \
    dejavu-sans-fonts \
    gcc \
    gcc-c++ \
    git \
    gnome-icon-theme \
    gtksourceview3 \
    libwebkit2-gtk \
    make \
    openssl-devel \
    readline-devel \
    sudo \
    vte3 \
    webkitgtk \
    webkitgtk3 \
    which \
    xorg-x11-server-Xvfb \
    zlib-devel && \
  yum clean all

RUN \
  systemd-machine-id-setup

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome

RUN \
  git clone --depth 1 https://github.com/rbenv/rbenv.git ~/.rbenv
RUN \
  git clone --depth 1 https://github.com/rbenv/ruby-build.git \
    ~/.rbenv/plugins/ruby-build

ENV PATH=/home/ruby-gnome/.rbenv/shims:/home/ruby-gnome/.rbenv/bin:$PATH
ENV RUBY_VERSION=2.4.1
RUN \
  rbenv install ${RUBY_VERSION}
RUN \
  rbenv global ${RUBY_VERSION}

WORKDIR /home/ruby-gnome

RUN gem install bundler

COPY Gemfile .
RUN bundle install
