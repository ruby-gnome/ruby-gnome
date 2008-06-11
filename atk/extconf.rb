=begin
extconf.rb for Ruby/Atk extention library
=end

PACKAGE_NAME = "atk"
PACKAGE_ID   = "atk"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..') 
SRCDIR = TOPDIR + '/atk/src'
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)

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

add_depend_package("glib2", "glib/src", TOPDIR)
add_distcleanfile("rbatkinits.c")

make_version_header("ATK", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_ATK_COMPILATION") {
  SRCDIR_QUOTED = SRCDIR.gsub(' ', '\ ')
  system("#{$ruby} #{SRCDIR_QUOTED}/makeinits.rb #{SRCDIR_QUOTED}/*.c > rbatkinits.c") or raise "failed to make ATK inits"
}

create_top_makefile

