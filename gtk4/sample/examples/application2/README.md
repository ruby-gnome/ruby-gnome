# Step 2: Populating the window

The original files are located in the following directory.

- https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application2

They are written in C language and use GResource because it is the best way in C language.
But when it comes to Ruby language, GResource is not the best way especially when the resource only includes ui files.
So, the program here uses a string instead of a resource.
If you want to know how to program with GResource in Ruby language, see gtk3/sample/tutorial/exampleapp2 directory.