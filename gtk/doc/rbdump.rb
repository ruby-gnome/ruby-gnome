require 'rbbr_rb'

classes = initialize_args({"gtk2"=>["Gtk","Gdk"]})
ClassDumper::new().visit(classes)

