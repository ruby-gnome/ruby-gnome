FROM ubuntu:24.04

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  apt update && \
  apt install -y \
    adwaita-icon-theme \
    dbus \
    g++ \
    gcc \
    git \
    gnumeric \
    gstreamer1.0-plugins-good \
    make \
    ruby-dev \
    sudo \
    xvfb

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome
