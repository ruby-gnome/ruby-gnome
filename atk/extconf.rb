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

have_func('atk_action_get_localized_name')
have_func('atk_hyperlink_is_inline')
have_func('atk_object_add_relationship')
have_func('atk_object_remove_relationship')
have_func('atk_component_get_layer')
have_func('atk_component_mdi_zorder')
have_func('atk_hyperlink_is_selected_link')
have_func('atk_text_get_bounded_ranges')
have_func('atk_role_get_localized_name')
have_func('atk_text_clip_type_get_type')

have_func('atk_text_free_ranges')

add_depend_package("glib2", "glib/src", TOPDIR)
add_distcleanfile("rbatkinits.c")

make_version_header("ATK", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_ATK_COMPILATION") {
  system("ruby #{SRCDIR}/makeinits.rb #{SRCDIR}/*.c > rbatkinits.c") or raise "failed to make ATK inits"
}

create_top_makefile

