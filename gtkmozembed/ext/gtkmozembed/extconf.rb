=begin
extconf.rb for Ruby/GtkMozEmbed extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

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

["glib2", "gtk2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
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

make_version_header("GTKMOZEMBED", package_id, ".")

create_pkg_config_file('Ruby/GtkMozEmbed', package_id)
$defs << "-DRUBY_GTKMOZEMBED_COMPILATION"
create_makefile_at_srcdir(module_name, (base_dir + "ext" + module_name).to_s) do
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
pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end

