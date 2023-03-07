# Step 5: Add a GSettings object

The original files are located in the following directory.

- https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5

The original schema file is [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5/org.gtk.exampleapp.gschema.xml) and the license is LGPL 2.1 or later.

This program uses GSettings object of the GIO library.
You need to compile the schema file (`org.gtk.exampleapp.gschema.xml`) in advance.
The instruction is written in the `Rakefile`, so you just run `rake` to compile it.

```console
$ cd gtk4/sample/examples/application5
$ rake
```

The Ruby script `exampleapp.rb` works with `gschemas.compiled`.

```console
$ ruby exampleapp.rb README.md Rakefile
```

You will see the contents of the two files `README.md` and `Rakefile` in the window.

If you want to remove `gschemas.compiled`, do like this:

```console
$ rake clean
```
