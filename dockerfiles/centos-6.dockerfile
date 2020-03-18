FROM centos:6

RUN \
  yum install -y \
    sudo \
    which \
    git \
    bzip2 \
    gcc \
    gcc-c++ \
    make \
    openssl-devel \
    readline-devel \
    zlib-devel && \
  yum clean all

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome

RUN \
  git clone --depth 1 https://github.com/rbenv/rbenv.git ~/.rbenv
RUN \
  git clone --depth 1 https://github.com/rbenv/ruby-build.git \
    ~/.rbenv/plugins/ruby-build

ENV PATH=/home/ruby-gnome/.rbenv/shims:/home/ruby-gnome/.rbenv/bin:$PATH
ENV RUBY_VERSION=2.4.1
RUN \
  rbenv install ${RUBY_VERSION}
RUN \
  rbenv global ${RUBY_VERSION}

WORKDIR /home/ruby-gnome

RUN gem install bundler

COPY Gemfile .
RUN bundle install
