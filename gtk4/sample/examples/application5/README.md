# Step 5: Add a GSettings object

The original files are located in the following directory.

- https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5

This program uses GSettings object of the GIO library.
You need to compile and install the schema file (`org.gtk.exampleapp.gschema.xml`) in this directory.
To install it, just run `install_schema.rb`.

```console
$ ruby gtk4/sample/examples/application5/install_schema.rb
```

Once you install it, you don't need to do it again.

The original schema file is [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5/org.gtk.exampleapp.gschema.xml) and the license is LGPL 2.1 or later.

Now you can run the Ruby program `exampleapp.rb` with files.

```console
$ ruby gtk4/sample/examples/application5/exampleapp.rb README.md NEWS.md
```

If you want to uninstall the schema file, run `uninstall_schema.rb`.

```console
$ ruby gtk4/sample/examples/application5/uninstall_schema.rb
```

Note:

The original C program doesn't install the schema into a schema directory, which is `/usr/local/share/glib-2.0/schemas` or `~/.local/share/glib-2.0/schemas`.
It just compiles it in the build directory to test the program.

If you just want to test the program and don't want to install the schema file, do like this:

- Compile the schema file in the current directory.

```console
$ cd gtk4/sample/examples/application5
$ glib-compile-schemas . # Don't forget the argument . (dot -- current directory)
```

- Set `GSETTINGS_SCHEMA_DIR` environment variable to the current directory and execute `exampleapp.rb`.

```console
$ GSETTINGS_SCHEMA_DIR=. ruby exampleapp.rb exampleapp.rb
```
