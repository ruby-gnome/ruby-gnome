=begin
extconf.rb for Ruby/Atk extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib" + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "atk"
package_id = "atk"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

atk_header = "atk/atk.h"
have_func('atk_action_get_localized_name', atk_header)
have_func('atk_hyperlink_is_inline', atk_header)
have_func('atk_object_add_relationship', atk_header)
have_func('atk_object_remove_relationship', atk_header)
have_func('atk_component_get_layer', atk_header)
have_func('atk_component_get_mdi_zorder', atk_header)
have_func('atk_hyperlink_is_selected_link', atk_header)
have_func('atk_text_get_bounded_ranges', atk_header)
have_func('atk_role_get_localized_name', atk_header)
have_func('atk_text_clip_type_get_type', atk_header)

have_func('atk_text_free_ranges', atk_header)

build_dir = "glib/tmp/#{RUBY_PLATFORM}/glib2/#{RUBY_VERSION}"
options = {}
options[:target_build_dir] = build_dir if (top_dir + build_dir).exist?
add_depend_package("glib2", "glib/ext/glib2", top_dir.to_s, options)
add_distcleanfile("rbatkinits.c")

make_version_header("ATK", package_id, ".")

create_pkg_config_file("Ruby/ATK", package_id)
$defs << " -DRUBY_ATK_COMPILATION"
create_makefile(module_name)
