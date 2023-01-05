FROM archlinux

RUN \
  pacman --sync --noconfirm --refresh --sysupgrade && \
  pacman --sync --noconfirm \
    adwaita-icon-theme \
    awk \
    dbus \
    gcc \
    git \
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
