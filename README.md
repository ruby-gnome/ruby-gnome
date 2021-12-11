# Ruby-GNOME

[![Gem Version](https://badge.fury.io/rb/gtk3.svg)](https://badge.fury.io/rb/gtk3)

![Ruby-GNOME Logo](https://avatars1.githubusercontent.com/u/416159?v=3&s=200)
Ruby bindings for GNOME

This is a set of bindings for the GNOME 3.x and 4.x libraries to use
from Ruby 2.5, 2.6 and 2.7.

## Installation

### Install the current releases.

The gems of the Ruby-GNOME projects can be easily installed with the `gem` command.
For example in order to install the gtk3 gem and its dependencies:

    gem install gtk3

### Install from GitHub master branch

You can also install these gems from GitHub master branch.

Create `Gemfile` like the following:

```ruby
source "https://rubygems.org/"

git "https://github.com/ruby-gnome/ruby-gnome.git" do
  gem "gtk3"
end
```

Install these gems by Bundler:

```console
% bundle install
```

## Advanced Installation

    % ruby extconf.rb
    % make
    % sudo make install

To compile and install a particular sub-binding, you can add arguments:

    % ruby extconf.rb [subdir]...
    e.g.) ruby extconf.rb glib2 pango atk gdk_pixbuf2 gtk2

Or you can compile each sub-binding:

    % cd <each sub-directory>
    % ruby extconf.rb
    % make
    % sudo make install

### extconf.rb options

* `--ruby`
  * ruby directory

* `--topsrcdir`
  * top source directory

* `--topdir`
  * top directory

* `--strict`
  * if some libraries fail to compile/install, "make"
    command returns 1(exit 1)

## Bugs

Please report bugs either in our bug tracker

* [https://github.com/ruby-gnome/ruby-gnome/issues](https://github.com/ruby-gnome/ruby-gnome/issues) (current)

or on the ruby-gnome2-devel-en / ruby-gnome2-devel-ja mailing list

* [http://sourceforge.net/mail/?group_id=53614](http://sourceforge.net/mail/?group_id=53614)

## Copying

Copyright (c) 2002-2021 Ruby-GNOME Project Team

This program is free software.
You can distribute/modify this program under the terms of
the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

### Exceptions

Some GTK+ 3 examples are licensed under GNU Free Documentation License
1.3 or any later version later; with no Invariant Sections, no
Front-Cover Texts, and no Back-Cover Texts.

Because they are based on scripts in
[PyGObject-Tutorial](https://github.com/sebp/PyGObject-Tutorial).
[PyGObject-Tutorial is licensed under GFDL 1.3](https://github.com/sebp/PyGObject-Tutorial/blob/master/COPYING).

See [gtk3/sample/](/gtk3/sample) directory for details.

## Project Website

[https://ruby-gnome2.osdn.jp/](https://ruby-gnome2.osdn.jp/)
