=begin
extconf.rb for Ruby/GnomePrintUI extention library
=end

require "fileutils"

maintainer = false

def mkenums(output, config, files)
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
#define GNOME_TYPE_PRINTUI_@ENUMSHORT@ (@enum_name@_get_type())
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
#include <libgnomeprintui/gnome-print-dialog.h>
#include <libgnomeprintui/gnome-print-paper-selector.h>
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

PACKAGE_NAME = "gnomeprintui2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomeprintui/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'
  
PKGConfig.have_package("libgnomeprintui-2.2") or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("gnomeprint2", "gnomeprint/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMEPRINTUI2_COMPILATION"){
  enum_type_prefix = "libgnomeprintui-enum-types"
  if !have_header("#{enum_type_prefix}.h")
    if maintainer
      include_paths = `pkg-config libgnomeprintui-2.2 --cflags-only-I`
      include_path = include_paths.split.find do |x|
        /libgnomeprintui/.match(x)
      end.sub(/^-I/, "")
      headers = Dir.glob(File.join(include_path, "libgnomeprintui", "*.h"))
      mkenums_h(enum_type_prefix, headers)
      mkenums_c(enum_type_prefix, headers)
    else
      %w(h c).each do |ext|
        enum_type_filename = "#{enum_type_prefix}.#{ext}"
        FileUtils.cp("#{enum_type_filename}.maintainer", enum_type_filename)
      end
    end
  end

  add_distcleanfile(enum_type_prefix + ".c")
  add_distcleanfile(enum_type_prefix + ".h")
}
create_top_makefile
