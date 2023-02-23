# Step 1: A trivial application

The following is only useful in the desktop system based on [Desktop Entry Specification](https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html) such as Gnome.

To make gnome-shell use the desktop file and icon for this example while running it uninstalled, do the following:

```
$ mkdir -p ~/.local/share/applications
$ cp org.gtk.exampleapp.desktop ~/.local/share/applications/org.gtk.exampleapp.desktop
$ mkdir -p ~/.local/share/icons/hicolor/48x48/apps
$ cp exampleapp.png ~/.local/share/icons/hicolor/48x48/apps
```

The file exapmleapp.png is from the GTK GitLab repository.
The original file is [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1) and the license is LGPL.

Note:

1. You need to install your exampleapp.rb in advance.

        $ cp exampleapp.rb $HOME/.local/bin/exampleapp.rb
        $ chmod 755 $HOME/.local/bin/exampleapp.rb

2. If you use rbenv, your ruby executable may not be found.
You can solve this by creating a symbolic link in $HOME/.local/bin to your ruby executable.
For example,

        $ cd $HOME/.local/bin
        $ ln -s $HOME/.rbenv/versions/3.1.2/bin/ruby ruby
