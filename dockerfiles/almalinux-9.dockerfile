FROM almalinux:9

RUN \
  dnf install -y \
    epel-release && \
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

# GOffice: It's not available.
# * gnumeric
# * goffice
#
# GtkSourceView5: It's not available.
# * gtksourceview5
#
# VTE4: It's not available.
# * vte4
#
# WebKit: It's not available.
# * webkit-gtk
# * webkit2-gtk
ENV \
  RUBY_GNOME_GNUMERIC_ENABLE=no \
  RUBY_GNOME_GOFFICE_ENABLE=no \
  RUBY_GNOME_GTKSOURCEVIEW5_ENABLE=no \
  RUBY_GNOME_VTE4_ENABLE=no \
  RUBY_GNOME_WEBKIT2_GTK_ENABLE=no \
  RUBY_GNOME_WEBKIT_GTK_ENABLE=no
