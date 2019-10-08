FROM fedora:rawhide

RUN \
  dnf install -y \
    adwaita-gtk2-theme \
    dbus-daemon \
    gcc \
    gcc-c++ \
    gnome-icon-theme \
    gnumeric \
    gstreamer1-plugins-good \
    make \
    redhat-rpm-config \
    ruby-devel \
    sudo \
    which \
    xorg-x11-server-Xvfb

RUN \
  gem install bundler

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

COPY . /home/ruby-gnome/ruby-gnome
RUN chown -R ruby-gnome: /home/ruby-gnome/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome/ruby-gnome

RUN bundle install --path vendor/bundle

CMD \
  dbus-run-session \
    xvfb-run --server-args "-screen 0 640x480x24" \
    bundle exec rake
