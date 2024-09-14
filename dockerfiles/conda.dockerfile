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
