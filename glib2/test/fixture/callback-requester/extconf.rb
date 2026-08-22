require "pathname"

base_dir = Pathname(__FILE__).dirname.parent.parent.parent.expand_path
mkmf_gnome2_dir = base_dir + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

package_id = "gobject-2.0"
require "mkmf-gnome"

include_paths = PKGConfig.cflags_only_I(package_id)

ext_dir = base_dir/"ext/glib2"

$INCFLAGS << " #{include_paths} -I#{ext_dir}"

create_makefile("callback_requester")
