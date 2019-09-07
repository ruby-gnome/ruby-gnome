FROM archlinux/base

RUN \
  pacman --sync --noconfirm --refresh && \
  pacman --sync --noconfirm \
    awk \
    dbus \
    gcc \
    gnome-icon-theme \
    gnumeric \
    gst-plugins-good \
    make \
    ruby \
    sudo \
    which \
    xorg-server-xvfb

RUN \
  gem install --no-user-install bundler

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

COPY . /home/ruby-gnome/ruby-gnome
RUN chown -R ruby-gnome: /home/ruby-gnome/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome/ruby-gnome

RUN bundle install

CMD \
  dbus-run-session \
    xvfb-run --server-args "-screen 0 640x480x24" \
    bundle exec rake
