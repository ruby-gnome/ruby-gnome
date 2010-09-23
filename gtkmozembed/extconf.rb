=begin
extconf.rb for Ruby/GtkMozEmbed extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.expand_path
top_dir = base_dir.parent.expand_path
mkmf_gnome2_dir = top_dir + "glib" + 'lib'
top_build_dir = Pathname(".").parent.expand_path

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gtkmozembed"
package_ids = ["libxul-embedding-unstable",
               "libxul-embedding",
               "xulrunner-gtkmozembed",
               "firefox-gtkmozembed",
               "seamonkey-gtkmozembed",
               "mozilla-gtkmozembed"]

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

package_id = nil
package_ids.each do |v|
  if PKGConfig.exist?(v)
    package_id = v
    $stderr.puts "#{v} is found."
    break
  else
    $stderr.puts "#{v} is not found."
  end
end

unless package_id
  $stderr.puts "No gtkmozembed is found. Abort."
  exit 1
end


#
# detect GTK+ configurations
#

PKGConfig.have_package('gtk+-2.0')
PKGConfig.have_package(package_id)

mozilla_library_path = PKGConfig.libs_only_L(package_id)
mozilla_path = mozilla_library_path.strip.sub(/^-L/, "")

if mozilla_path
  # please comment the CFLAGS line if you don't want a default comp_path
  $CFLAGS << " -DDEFAULT_MOZILLA_FIVE_HOME='\"#{mozilla_path}\"' "
  $LDFLAGS << " -Wl,-rpath,#{mozilla_path}"
else
  $stderr.puts "${package_id}.pc cannot be found."
  exit 1
end

[["glib", "glib2"],
 ["gtk", "gtk2"]].each do |directory, library_name|
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{library_name}/#{RUBY_VERSION}"
  add_depend_package(library_name, "#{directory}/ext/#{library_name}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

if have_library("xpcomglue")
  have_header("gtkmozembed_glue.cpp") or exit 1
  CONFIG['LDSHARED'].gsub!(/\$\(CC\)/, "$(CXX)")
elsif have_library("gtkembedmoz")
  have_func('gtk_moz_embed_new') or exit 1
  have_func('gtk_moz_embed_set_profile_path') or exit 1
else
  exit 1
end

make_version_header("GTKMOZEMBED", package_id)

create_pkg_config_file('Ruby/GtkMozEmbed', package_id)

create_makefile_at_srcdir(module_name, (base_dir + "src").to_s,
                          "-DRUBY_GTKMOZEMBED_COMPILATION") do
  enum_type_prefix = "gtkmozembed-enum-types"
  include_paths = PKGConfig.cflags_only_I(package_id)
  include_paths = include_paths.split.collect do |path|
    path.strip.sub(/^-I/, '')
  end
  headers = include_paths.inject([]) do |result, path|
    gtkmozembed_h = File.join(path, "gtkmozembed.h")
    result += [gtkmozembed_h] if File.exist?(gtkmozembed_h)
    result
  end
  glib_mkenums(enum_type_prefix, headers, "GTK_TYPE_", ["gtkmozembed.h"],
               :force_flags => [/Flags/])
end

create_top_makefile
