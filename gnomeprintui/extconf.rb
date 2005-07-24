=begin
extconf.rb for Ruby/GnomePrintUI extention library
=end

require "fileutils"

maintainer = false

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
  Process.waitpid(pid)
  File.open(output, "w") do |out|
    out.print(for_read.read)
  end
ensure
  $makefile_created = makefile_created
end

def mkenums_h(dir, prefix, files)
  header = prefix + ".h"
  const = "__#{header.upcase.gsub(/-|\./, '_')}__"
  header = File.join(dir, header)
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
#define GNOME_TYPE_PRINT_@ENUMSHORT@ (@enum_name@_get_type())
VHEAD
    :ftail => <<-FTAIL,
G_END_DECLS

#endif /* #{const}*/
FTAIL
  }
  mkenums(header, config, files)
end

def mkenums_c(dir, prefix, files)
  header = File.join(dir, prefix + ".h")
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

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("libart2", "libart/src", TOPDIR)
add_depend_package("gnomeprint2", "gnomeprint/src", TOPDIR)

have_header("libgnomeprintui/gnome-print-config-dialog.h")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMEPRINTUI2_COMPILATION"){
  enum_type_dir = "libgnomeprintui"
  enum_type_prefix = "libgnomeprintui-enum-types"
  unless have_header("#{enum_type_dir}/#{enum_type_prefix}.h")
    FileUtils.mkdir_p(enum_type_dir)
    enum_type_full_prefix = File.join(enum_type_dir, enum_type_prefix)
    enum_type_c = "#{enum_type_prefix}.c"
    enum_type_h = "#{enum_type_full_prefix}.h"
    maintainer_enum_type_c = "#{enum_type_prefix}.c.maintainer"
    maintainer_enum_type_h = "#{enum_type_prefix}.h.maintainer"
    
    if maintainer
      include_paths = `pkg-config libgnomeprintui-2.2 --cflags-only-I`
      include_path = include_paths.split.find do |x|
        /libgnomeprintui/.match(x)
      end.sub(/^-I/, "")
      headers = Dir.glob(File.join(include_path, "libgnomeprintui", "*.h"))

      mkenums_c(enum_type_dir, enum_type_prefix, headers)
      mkenums_h(enum_type_dir, enum_type_prefix, headers)

      FileUtils.mv("#{enum_type_prefix}.c", maintainer_enum_type_c)
      FileUtils.mv("#{File.join(enum_type_dir, enum_type_prefix)}.h",
                   maintainer_enum_type_h)
    end

    FileUtils.cp(maintainer_enum_type_c, enum_type_c)
    FileUtils.cp(maintainer_enum_type_h, enum_type_h)

    add_distcleanfile(enum_type_c)
    add_distcleanfile(enum_type_h)
  end
}
create_top_makefile
