require 'rbbr_rb'

classes = initialize_args({"gnome"=>["Gtk","Gdk","GdkImlib","Gnome"]})
ClassDumper::new().visit(classes)

