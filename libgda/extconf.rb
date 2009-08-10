=begin
extconf.rb for Ruby/Libgda extention library
=end

package_name = package_id = nil
package_name = "libgda"
package_id   = "libgda"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/libgda/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

if PKGConfig.have_package('libgda') 
    ["1.1", "1.2", "1.3"].each do |version|
        if system("pkg-config libgda --atleast-version=#{version}")
            $CFLAGS << " -DGDA_AT_LEAST_#{version.sub(/\./, "_")} "
        end
    end
    package_name = package_id = 'libgda'
elsif PKGConfig.have_package('libgda-2.0')
    $CFLAGS << " -DGDA_AT_LEAST_1_3 "
    package_name = package_id = 'libgda-2.0'
else
    exit 1
end

PKGConfig.have_package("gobject-2.0") or exit 1

setup_win32(package_name)

add_depend_package("glib2", "glib/src", TOPDIR)

make_version_header("LIBGDA", package_id)

create_makefile_at_srcdir(package_name, SRCDIR, "-DRUBY_LIBGDA_COMPILATION")
create_pkg_config_file(package_name, package_id, ruby_gnome2_version)
create_top_makefile
