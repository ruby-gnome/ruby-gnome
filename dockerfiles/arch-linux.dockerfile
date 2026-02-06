FROM archlinux

RUN \
  pacman --sync --noconfirm --refresh --sysupgrade && \
  pacman --sync --noconfirm \
    adwaita-icon-theme \
    awk \
    dbus \
    gcc \
    git \
    make \
    ruby \
    ruby-rake \
    sudo \
    which \
    xorg-server-xvfb

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome

# Clutter: It's not available.
# * clutter
# * clutter-gdk
# * clutter-gtk
# * clutter-gstreamer
ENV \
  RUBY_GNOME_CLUTTER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GDK_ENABLE=no \
  RUBY_GNOME_CLUTTER_GSTREAMER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GTK_ENABLE=no
