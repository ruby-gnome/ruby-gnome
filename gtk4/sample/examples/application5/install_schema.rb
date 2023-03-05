# install_schema.rb

require "fileutils"
include FileUtils

current_path = File.expand_path(File.dirname(__FILE__))
schema_dir = "#{Dir.home}/.local/share/glib-2.0/schemas"
schema_file = "org.gtk.exampleapp.gschema.xml"

mkdir_p(schema_dir)
cp("#{current_path}/#{schema_file}", "#{schema_dir}/#{schema_file}")
system("glib-compile-schemas", schema_dir)
