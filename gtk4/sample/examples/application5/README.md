# Step 5: Add a GSettings object

The original files are located in the following directory.

- https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5

The original schema file is [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application5/org.gtk.exampleapp.gschema.xml) and the license is LGPL 2.1 or later.

This program uses GSettings object of the GIO library.
You need to compile the schema file (`org.gtk.exampleapp.gschema.xml`) in advance.
Therefore, there are two tasks.

- Compile the schema file
- Run the Ruby script `exampleapp.rb`

These tasks are integrated into a Rakefile.
What you need is just to type `rake`.

```console
$ cd gtk4/sample/examples/application5
$ rake
```

If you want to give filenames to `exampleapp.rb`, use square bracket.

```console
$ rake default[README.md,Rakefile]
```

The two arguments `README.md` and `Rakefile` are given to the Ruby script so that you can see the contents of them in the window.
If you want to put spaces after the comma, surround the bracket with double quotes.
It is because spaces are shell script delimiters.

The `rake` creates a file `gschema.compiled`.
If you want to remove it, do like this:

```console
$ rake clean
```
