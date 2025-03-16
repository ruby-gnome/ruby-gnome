FROM continuumio/miniconda3

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  apt update && \
  apt install -y \
    shared-mime-info \
    xvfb

COPY environment.yml /
RUN \
  conda env create && \
  echo "conda activate ruby-gnome" >> ~/.bashrc

# "gem install", "gem list" and so on don't recognize gems installed
# by "gem install --user-install"...
ENV RUBY_GNOME_INSTALL_USER_INSTALL=no

# Adwaita: It's not available.
# * adwaita
#
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
# GtkSourceView: It's not available.
# * gtksourceview3
# * gtksourceview4
# * gtksourceview5
#
# libhandy: It's not available.
# * libhandy
#
# libwnck: It's not available.
# * wnck3
#
# VLC: It's not available.
# * gvlc
#
# VTE: It's not available.
# * vte3
# * vte4
#
# WebKit: It's not available.
# * webkit-gtk
# * webkit2-gtk
ENV \
  RUBY_GNOME_ADWAITA_ENABLE=no \
  RUBY_GNOME_CLUTTER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GDK_ENABLE=no \
  RUBY_GNOME_CLUTTER_GSTREAMER_ENABLE=no \
  RUBY_GNOME_CLUTTER_GTK_ENABLE=no \
  RUBY_GNOME_GEGL_ENABLE=no \
  RUBY_GNOME_GNUMERIC_ENABLE=no \
  RUBY_GNOME_GOFFICE_ENABLE=no \
  RUBY_GNOME_GTKSOURCEVIEW3_ENABLE=no \
  RUBY_GNOME_GTKSOURCEVIEW4_ENABLE=no \
  RUBY_GNOME_GTKSOURCEVIEW5_ENABLE=no \
  RUBY_GNOME_GVLC_ENABLE=no \
  RUBY_GNOME_LIBHANDY_ENABLE=no \
  RUBY_GNOME_VTE3_ENABLE=no \
  RUBY_GNOME_VTE4_ENABLE=no \
  RUBY_GNOME_WEBKIT2_GTK_ENABLE=no \
  RUBY_GNOME_WEBKIT_GTK_ENABLE=no \
  RUBY_GNOME_WNCK3_ENABLE=no
