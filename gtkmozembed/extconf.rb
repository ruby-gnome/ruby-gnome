=begin
extconf.rb for Ruby/GtkMozEmbed extention library
=end

PACKAGE_NAME = "gtkmozembed"
PACKAGE_IDS = ["libxul-embedding-unstable",
               "xulrunner-gtkmozembed",
               "firefox-gtkmozembed",
               "seamonkey-gtkmozembed",
               "mozilla-gtkmozembed"]

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtkmozembed/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

package_id = nil
PACKAGE_IDS.each do |v|
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
setup_win32(PACKAGE_NAME)

mozilla_library_path = PKGConfig.libs_only_L(package_id)
mozilla_path = mozilla_library_path.strip.sub(/^-L/, "")

if mozilla_path
  # please comment the CFLAGS line if you don't want a default comp_path
  $CFLAGS << " -DDEFAULT_MOZILLA_FIVE_HOME='\"#{mozilla_path}\"' "
  $LDFLAGS << " -Wl,-rpath #{mozilla_path}"
else
  $stderr.puts "${package_id}.pc cannot be found."
  exit 1
end

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

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

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR,
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
  glib_mkenums(enum_type_prefix, headers, "GTK_TYPE_", ["gtkmozembed.h"])
end

create_top_makefile
