# Step 4: Add a menu

The original files are located in the following directory.

- https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application4

They are written in the C language and use `g_action_map_add_action entries`.
It is a convenience function for creating multiple GSimpleAction instances and adding them to a GActionMap.
If you write a program in C, it is the best way.

However, the program in this directory doesn't use `add_action_entries` method.
Instead, it uses `each` method of an array of action names.
The way above is flexible and simple.
