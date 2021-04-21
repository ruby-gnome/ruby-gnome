FROM archlinux/base

RUN \
  pacman --sync --noconfirm --refresh --sysupgrade && \
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

USER ruby-gnome
WORKDIR /home/ruby-gnome

COPY Gemfile .
RUN bundle install
