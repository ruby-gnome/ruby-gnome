# Ruby/GDK4

Ruby/GDK4 is a Ruby binding of GDK 4.
https://developer.gnome.org/gdk4/3.90/

## Requirements

* Ruby/GLib2, Ruby/ATK, Ruby/Pango and Ruby/GdkPixbuf2 in
  [Ruby-GNOME2](http://ruby-gnome2.sourceforge.jp/)
* [rcairo](https://github.com/rcairo/rcairo)
* [GTK+](https://www.gtk.org/) 3.93 or later

## Test

With jhbuild environment:

```bash
#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export GI_TYPELIB_PATH="/home/$USER/jhbuild/install/lib/girepository-1.0"
export LD_LIBRARY_PATH="/home/$USER/jhbuild/install/lib"

if [ $# -eq 0 ]
  then
   $DIR/gdk4/test/run-test.rb
  else
   $DIR/gdk4/test/run-test.rb -t $1
fi
```

Run with:

```
runtest.sh
# or
runtest.sh TestGdkRectangle
```

## Install

    gem install gdk4

## License

Copyright (c) 2002-2018 Ruby-GNOME2 Project Team

This program is free software. You can distribute/modify this program
under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

## Project Website

http://ruby-gnome2.sourceforge.jp/
