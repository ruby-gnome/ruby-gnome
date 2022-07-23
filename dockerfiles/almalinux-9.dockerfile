FROM almalinux:9

RUN \
  dnf install -y \
    epel-release && \
  dnf install -y \
    adwaita-gtk2-theme \
    dbus-daemon \
    dejavu-sans-fonts \
    gcc \
    gcc-c++ \
    git \
    gstreamer1-plugins-good \
    make \
    redhat-rpm-config \
    ruby-devel \
    sudo \
    which \
    xorg-x11-server-Xvfb && \
  dnf clean all

RUN \
  gem install bundler

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome

COPY Gemfile .
RUN sudo gem install cairo
RUN bundle install
