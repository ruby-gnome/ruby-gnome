FROM centos:7

ENV SCL=rh-ruby30

RUN \
  yum install -y \
    centos-release-scl-rh \
    epel-release && \
  yum install -y \
    adwaita-gtk2-theme \
    clutter-gtk \
    dbus-daemon \
    dejavu-sans-fonts \
    gcc \
    gcc-c++ \
    gnome-icon-theme \
    gtksourceview3 \
    libwebkit2-gtk \
    make \
    ${SCL}-ruby-devel \
    ${SCL}-ruby-rubygem-bundler \
    sudo \
    vte3 \
    webkitgtk \
    webkitgtk3 \
    which \
    xorg-x11-server-Xvfb && \
  yum clean all

RUN \
  systemd-machine-id-setup

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome
