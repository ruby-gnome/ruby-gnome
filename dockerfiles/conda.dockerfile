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

COPY environment.yml /
RUN \
  conda env create && \
  echo "conda activate ruby-gnome" >> ~/.bashrc
