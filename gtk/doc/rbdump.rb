require 'rbbr_rb'

classes = initialize_args({"gtk"=>["Gtk","Gdk"]})
ClassDumper::new().visit(classes)

