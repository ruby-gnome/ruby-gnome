=begin
extconf.rb for Ruby/GnomeVFS extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.expand_path
top_dir = base_dir.parent.expand_path
mkmf_gnome2_dir = top_dir + "glib" + 'lib'
top_build_dir = Pathname(".").parent.expand_path

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gnomevfs"
package_id = "gnome-vfs-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1
PKGConfig.have_package('gnome-vfs-module-2.0') or exit 1
PKGConfig.have_package("gobject-2.0") or exit 1

have_func("rb_exec_recursive")

[["glib", "glib2"]].each do |directory, library_name|
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{library_name}/#{RUBY_VERSION}"
  add_depend_package(library_name, "#{directory}/ext/#{library_name}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

make_version_header("GNOMEVFS", package_id)

create_pkg_config_file("Ruby/GnomeVFS", package_id)
create_makefile_at_srcdir(module_name, (base_dir + "src").to_s,
                          "-DRUBY_GNOMEVFS_COMPILATION")
create_top_makefile
