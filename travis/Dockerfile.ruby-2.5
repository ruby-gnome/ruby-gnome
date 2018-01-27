FROM ruby:2.5

RUN \
  apt update && \
  apt install -y \
    sudo \
    xvfb \
    gstreamer1.0-plugins-good \
    gnumeric \
    gnome-icon-theme \
    dbus-x11

RUN \
  useradd --user-group --create-home ruby-gnome2

RUN \
  echo "ruby-gnome2 ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome2

COPY . /home/ruby-gnome2/ruby-gnome2
RUN chown -R ruby-gnome2:ruby-gnome2 /home/ruby-gnome2/ruby-gnome2

USER ruby-gnome2
WORKDIR /home/ruby-gnome2/ruby-gnome2

RUN bundle install

CMD xvfb-run --server-args "-screen 0 640x480x24" bundle exec rake
