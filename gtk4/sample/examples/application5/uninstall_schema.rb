# uninstall_schema.rb

require "fileutils"
include FileUtils

schema_dir = "#{Dir.home}/.local/share/glib-2.0/schemas"
schema_file = "org.gtk.exampleapp.gschema.xml"

return unless File.exist?("#{schema_dir}/#{schema_file}")
rm("#{schema_dir}/#{schema_file}")
system("glib-compile-schemas", schema_dir)
