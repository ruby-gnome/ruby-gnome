ARG RUBY_VERSION
FROM ruby:${RUBY_VERSION}

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  apt update && \
  apt install -y \
    dbus \
    gnome-icon-theme \
    gnumeric \
    gstreamer1.0-plugins-good \
    sudo \
    xvfb

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome
