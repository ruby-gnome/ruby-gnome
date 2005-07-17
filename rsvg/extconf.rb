=begin
extconf.rb for Ruby/RSVG extention library
=end

def mkenums(output, config, files)
  makefile_created = $makefile_created
  $makefile_created = true
  args = []
  %w(fhead fprod ftail eprod vhead vprod vtail comments template).each do |param|
    param = param.intern
    args += ["--#{param}", config[param]] if config.has_key?(param)
  end
  args += files
  for_read, for_write = IO.pipe
  pid = fork do
    STDOUT.reopen(for_write)
    for_write.close
    system('glib-mkenums', *args)
  end
  for_write.close
  File.open(output, "w") do |out|
    out.print(for_read.read)
  end
  Process.waitpid(pid)
ensure
  $makefile_created = makefile_created
end

def mkenums_h(prefix, files)
  header = prefix + ".h"
  const = "__#{header.upcase.gsub(/-|\./, '_')}__"
  config = {
    :fhead => <<-FHEAD,
#ifndef #{const}
#define #{const}

#include <glib-object.h>

G_BEGIN_DECLS
FHEAD
    :fprod => <<-FPROD,
/* enumerations from "@filename@" */
FPROD
    :vhead => <<-VHEAD,
GType @enum_name@_get_type (void);
#define RSVG_TYPE_@ENUMSHORT@ (@enum_name@_get_type())
VHEAD
    :ftail => <<-FTAIL,
G_END_DECLS

#endif /* #{const}*/
FTAIL
  }
  mkenums(header, config, files)
end

def mkenums_c(prefix, files)
  header = prefix + ".h"
  c = prefix + ".c"
  config = {
    :fhead => <<-FHEAD,
#include "#{header}"
#include <librsvg/rsvg.h>
FHEAD
    :fprod => <<-FPROD,
/* enumerations from "@filename@" */
FPROD
    :vhead => <<-VHEAD,
GType
@enum_name@_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const G@Type@Value values[] = {
VHEAD
    :vprod => <<-VPROD,
      { @VALUENAME@, "@VALUENAME@", "@valuenick@" },
VPROD
    :vtail => <<-VTAIL,
      { 0, NULL, NULL }
    };
    etype = g_@type@_register_static ("@EnumName@", values);
  }
  return etype;
}
VTAIL
  }
  mkenums(c, config, files)
end

PACKAGE_NAME = "rsvg2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/rsvg/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('librsvg-2.0') or exit 1
setup_win32(PACKAGE_NAME)

have_func("rsvg_set_default_dpi_x_y")
have_func("rsvg_handle_set_dpi_x_y")
have_header("librsvg/rsvg-gz.h")

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_RSVG2_COMPILATION") {
  enum_type_prefix = "librsvg-enum-types"
  unless have_header("librsvg/#{enum_type_prefix}.h")
    include_paths = PKGConfig.cflags_only_I("librsvg-2.0")
    include_path = include_paths.split.find do |x|
      /librsvg/.match(x)
    end.sub(/^-I/, "")
    headers = Dir.glob(File.join(include_path, "librsvg", "*.h"))
    mkenums_h(enum_type_prefix, headers)
    mkenums_c(enum_type_prefix, headers)
  end

  add_distcleanfile(enum_type_prefix + ".c")
  add_distcleanfile(enum_type_prefix + ".h")
}

create_top_makefile
