FROM almalinux:10

RUN \
  dnf install -y \
    almalinux-release-devel \
    epel-release && \
  dnf install -y \
    adwaita-icon-theme \
    dbus-daemon \
    dejavu-sans-fonts \
    gcc \
    gcc-c++ \
    git \
    gstreamer1-plugins-good \
    make \
    redhat-rpm-config \
    ruby-devel \
    rubygem-rake \
    sudo \
    which \
    xorg-x11-server-Xvfb && \
  dnf clean all

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
#
# GEGL: It's not available.
# * gegl
#
# GOffice: It's not available.
# * gnumeric
# * goffice
#
# WebKit: It's not available.
# * webkit-gtk
# * webkit2-gtk
ENV \
  RUBY_GNOME_CLUTTER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GDK_ENABLE=no \
  RUBY_GNOME_CLUTTER_GSTREAMER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GTK_ENABLE=no \
  RUBY_GNOME_GEGL_ENABLE=no \
  RUBY_GNOME_GNUMERIC_ENABLE=no \
  RUBY_GNOME_GOFFICE_ENABLE=no \
  RUBY_GNOME_WEBKIT2_GTK_ENABLE=no \
  RUBY_GNOME_WEBKIT_GTK_ENABLE=no
