=begin
extconf.rb for Ruby/Libglade2 extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "libglade2"
package_id = "libglade-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit(false)

have_header("libintl.h")

have_func("bindtextdomain")
have_func("bind_textdomain_codeset")

["glib2", "gtk2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
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
