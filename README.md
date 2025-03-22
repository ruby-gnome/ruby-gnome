# Ruby-GNOME
[![Gem Version](https://badge.fury.io/rb/gtk4.svg)](https://badge.fury.io/rb/gtk4)

![Ruby-GNOME Logo](https://avatars1.githubusercontent.com/u/416159?v=3&s=200)
Ruby bindings for GNOME

This is a set of bindings for the GNOME 3.x and 4.x libraries to use
from Ruby that allows you to write graphical user interfaces in Ruby.

## Installation Instructions for gtk3 and gtk4

### Debian/Ubuntu

Ruby can be run from the system directories, or from your home
directory, or both.  Most people won't have the correct permissions
for a system install, so the vast majority of people to run Ruby from
their local, `/home/...` directory.

To test if your copy of Ruby is running locally enter this on the
command line:

```bash
which ruby
```

If it outputs something like this:

```bash
/home/ralph/.rbenv/shims/ruby
```

that starts with `/home/`, that's local (good!)

But if outputs something like this:

```bash
/usr/bin/ruby
```

That is a system directory, and the gtk4 gem will fail to install.
Actually, a system administrator with super-user permissions could
install everything in the system folders (see advanced install), but
its much better for everyone else to run Ruby locally.

If you're already running a copy of Ruby under your home directory,
you can install gtk4 (or gtk3):

```bash
gem install gtk4
```

But if you're running ruby from the system folders, you can install a
Ruby version manager like [RVM](https://rvm.io/) or
[rbenv](https://github.com/rbenv/rbenv).  These programs allow you to
run multiple versions of Ruby from your home directory.

rbenv has a
[rbenv-installer](https://github.com/rbenv/rbenv-installer) that
installs everything and adds a line to your `.bashrc` file so you're
running local copies of Ruby.

To get rbenv:
1. Run the installer
2. Restart your command prompt
3. Run `rbenv install --list` (find recent Ruby version)
4. Run `rbenv install 3.3.5` (example version.  choose one from list)
5. Run `which ruby` (check that its running from home directory)

If that works, then you're ready to install gtk4 (or gtk3):

```bash
gem install gtk4
```

### macOS

Input needed...

### Windows

In Windows, the gem should be installed without issue. If you install
Ruby using the [Ruby Installer](https://rubyinstaller.org/), make sure
that MSYS2 gets installed too.  GTK needs MSYS2 to make the binary
files. Then, at the Ruby command prompt:

```bash
gem install gtk4
```

## Advanced Installation instructions

### Debian/Ubuntu

```bash
sudo apt install -y gcc make ruby-dev
sudo gem install rubygems-requirements-system
sudo gem install gtk4
```

## Install from GitHub master branch

You can also install these gems from GitHub master branch.

Create `Gemfile` like the following:

```ruby
source "https://rubygems.org/"

plugin "rubygems-requirements-system"

git "https://github.com/ruby-gnome/ruby-gnome.git" do
  gem "gtk4"
end
```

Install these gems by Bundler:

```bash
bundle install
```

## Build From Source Code

```bash
ruby extconf.rb
make
```

To compile and install a particular sub-binding, you can add arguments:

```bash
ruby extconf.rb [subdir]...
# e.g.) ruby extconf.rb glib2 pango atk gdk_pixbuf2 gtk4
```

Or you can compile each sub-binding:

```bash
cd <each sub-directory>
ruby extconf.rb
make
```

### extconf.rb options

* `--ruby`
  * ruby directory

* `--topsrcdir`
  * top source directory

* `--topdir`
  * top directory

* `--strict`
  * if some libraries fail to compile/install, `make`
    command returns 1(exit 1)

## Bugs

Please report bugs in our bug tracker:

* [https://github.com/ruby-gnome/ruby-gnome/issues](https://github.com/ruby-gnome/ruby-gnome/issues)

## Copying

Copyright (c) 2002-2025 Ruby-GNOME Project Team

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

[https://ruby-gnome.github.io/](https://ruby-gnome.github.io/)
