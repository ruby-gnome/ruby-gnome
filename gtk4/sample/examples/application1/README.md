# Step 1: A trivial application

The following is only useful in the desktop system based on [Desktop Entry Specification](https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html) such as GNOME.

To make gnome-shell use the desktop file and icon for this example while running it uninstalled, do the following:

```console
$ mkdir -p ~/.local/share/applications
$ cp org.gtk.exampleapp.desktop ~/.local/share/applications/org.gtk.exampleapp.desktop
$ mkdir -p ~/.local/share/icons/hicolor/48x48/apps
$ cp exampleapp.png ~/.local/share/icons/hicolor/48x48/apps
```

The file exapmleapp.png is from the GTK GitLab repository.
The original file is [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1/exampleapp.png) and the license is LGPL 2.1 or later.

Note:

1. You need to install your exampleapp.rb in advance.

   ```console
   $ cp exampleapp.rb ~/.local/bin/exampleapp.rb
   $ chmod +x ~/.local/bin/exampleapp.rb
   ```

2. If you use rbenv, your ruby executable may not be found.
   You can solve this by creating a symbolic link in `~/.local/bin` to your ruby executable.
   For example,

   ```console
   $ ln -s $(rbenv which ruby) ~/.local/bin/ruby
   ```
