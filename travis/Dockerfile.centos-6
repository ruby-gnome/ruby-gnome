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
  useradd --user-group --create-home ruby-gnome2

RUN \
  echo "ruby-gnome2 ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome2

USER ruby-gnome2

RUN \
  git clone --depth 1 https://github.com/rbenv/rbenv.git ~/.rbenv
RUN \
  git clone --depth 1 https://github.com/rbenv/ruby-build.git \
    ~/.rbenv/plugins/ruby-build

ENV PATH=/home/ruby-gnome2/.rbenv/shims:/home/ruby-gnome2/.rbenv/bin:$PATH
ENV RUBY_VERSION=2.4.1
RUN \
  rbenv install ${RUBY_VERSION}
RUN \
  rbenv global ${RUBY_VERSION}

COPY . /home/ruby-gnome2/ruby-gnome2
RUN sudo chown -R ruby-gnome2:ruby-gnome2 ~/ruby-gnome2
WORKDIR /home/ruby-gnome2/ruby-gnome2

RUN gem install bundler
RUN bundle install

CMD cd glib2 && ruby extconf.rb && test/run-test.rb -v
