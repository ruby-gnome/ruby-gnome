FROM almalinux:8

RUN \
  dnf install -y \
    epel-release && \
  dnf module disable -y ruby && \
  dnf module enable -y ruby:3.3 && \
  dnf install -y \
    adwaita-gtk2-theme \
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

# GTK4: It's not available.
# * adwaita
# * gdk4
# * graphene
# * gsk4
# * gtk4
# * gtksourceview5
# * vte4
#
# GOffice: It's not available.
# * gnumeric
# * goffice
#
# VLC: It's not available.
# * gvlc
#
# libhandy: It's not available.
# * libhandy
#
# WebKit: It's not available.
# * webkit-gtk
# * webkit2-gtk
ENV \
  RUBY_GNOME_ADWAITA_ENABLE=no \
  RUBY_GNOME_GDK4_ENABLE=no \
  RUBY_GNOME_GSK4_ENABLE=no \
  RUBY_GNOME_GNUMERIC_ENABLE=no \
  RUBY_GNOME_GOFFICE_ENABLE=no \
  RUBY_GNOME_GRAPHENE_ENABLE=no \
  RUBY_GNOME_GTK4_ENABLE=no \
  RUBY_GNOME_GTKSOURCEVIEW5_ENABLE=no \
  RUBY_GNOME_GVLC_ENABLE=no \
  RUBY_GNOME_LIBHANDY_ENABLE=no \
  RUBY_GNOME_VTE4_ENABLE=no \
  RUBY_GNOME_WEBKIT2_GTK_ENABLE=no \
  RUBY_GNOME_WEBKIT_GTK_ENABLE=no
