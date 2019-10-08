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
    zlib-devel

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

COPY . /home/ruby-gnome/ruby-gnome
RUN sudo chown -R ruby-gnome:ruby-gnome ~/ruby-gnome
WORKDIR /home/ruby-gnome/ruby-gnome

RUN gem install bundler
RUN bundle install

CMD cd glib2 && ruby extconf.rb && test/run-test.rb -v
