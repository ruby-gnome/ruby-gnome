FROM continuumio/miniconda3

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  apt update && \
  apt install -y \
    shared-mime-info \
    xvfb

# Ruby in conda uses /usr/bin/mkdir not /bin/mkdir
RUN ln -s /bin/mkdir /usr/bin/

RUN \
  conda update -c conda-forge -y --quiet --all && \
  conda install -c conda-forge -y --quiet \
    adwaita-icon-theme \
    compilers \
    dbus \
    glib \
    libgcc-ng \
    make \
    ruby && \
  conda update -c conda-forge -y --quiet --all
