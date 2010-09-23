=begin
extconf.rb for Ruby/Libglade2 extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.expand_path
top_dir = base_dir.parent.expand_path
mkmf_gnome2_dir = top_dir + "glib" + 'lib'
top_build_dir = Pathname(".").parent.expand_path

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "libglade2"
package_id = "libglade-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit(false)

have_header("libintl.h")

have_func("bindtextdomain")
have_func("bind_textdomain_codeset")

[["glib", "glib2"],
 ["gtk", "gtk2"]].each do |directory, library_name|
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{library_name}/#{RUBY_VERSION}"
  add_depend_package(library_name, "#{directory}/ext/#{library_name}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

if /mswin32/ =~ RUBY_PLATFORM
# gladewin32 has been changed?
#  $libs.gsub!(/ xml2.lib /, " libxml2.lib ")
#  $libs.gsub!(/ z.lib /, " libz.lib ")
  #FIXME
  $CFLAGS += " -DHAVE_BINDTEXTDOMAIN -DHAVE_BIND_TEXTDOMAIN_CODESET "
end

make_version_header("LIBGLADE", package_id, ".")
create_pkg_config_file("Ruby/Libglade2", package_id)
create_makefile(module_name)
