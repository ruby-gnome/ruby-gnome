FROM ubuntu:16.04

RUN \
  apt update && \
  apt install -y \
    dbus \
    g++ \
    gcc \
    gnome-icon-theme \
    gnumeric \
    gstreamer1.0-plugins-good \
    make \
    ruby-dev \
    sudo \
    xvfb

RUN \
  gem install bundler

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

COPY . /home/ruby-gnome/ruby-gnome
RUN chown -R ruby-gnome:ruby-gnome /home/ruby-gnome/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome/ruby-gnome

RUN bundle install --path vendor/bundle

CMD \
  dbus-run-session \
    xvfb-run --server-args "-screen 0 640x480x24" \
    bundle exec rake
