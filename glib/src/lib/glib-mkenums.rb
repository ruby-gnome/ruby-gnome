#
# glib-mkenums.rb
#
# Wrapper of glib-mkenums tool.
#
# Copyright(C) 2006 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.
#

require 'mkmf'

module GLib
  @@mkenums = with_config('glib-mkenums', ENV["GLIB_MKENUMS"] || 'glib-mkenums')

  module_function
  def mkenums(prefix, files, g_type_prefix, include_files)
    mkenums_c(prefix, files, include_files)
    mkenums_h(prefix, files, g_type_prefix)
  end

  def mkenums_c(prefix, files, include_files)
    header = "#{prefix}.h"
    c = "#{prefix}.c"

    fhead = "#include \"#{header}\"\n"
    include_files.each do |file|
      fhead << "#include <#{file}>\n"
    end
    fhead << "\n\n"

    config = {
      :fhead => fhead,

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
    run_mkenums(c, config, files)
  end

  def mkenums_h(prefix, files, g_type_prefix)
    header = "#{prefix}.h"
    const = "__#{File.basename(header).upcase.gsub(/-|\./, '_')}__"

    fhead = <<-FHEAD
#ifndef #{const}
#define #{const}

#include <glib-object.h>

G_BEGIN_DECLS
FHEAD

    config = {
      :fhead => fhead,

      :fprod => <<-FPROD,

/* enumerations from "@filename@" */
FPROD
      :vhead => <<-VHEAD,
GType @enum_name@_get_type (void);
#define #{g_type_prefix}@ENUMSHORT@ (@enum_name@_get_type())
VHEAD
      :ftail => <<-FTAIL,

G_END_DECLS

#endif /* #{const}*/
FTAIL
    }
    run_mkenums(header, config, files)
  end

  def run_mkenums(output, config, files)
    add_distcleanfile(output)

    makefile_created = $makefile_created
    $makefile_created = true
    args = []
    params = %w(fhead fprod ftail eprod vhead vprod vtail comments template)
    params.each do |param|
      param = param.intern
      args.concat(["--#{param}", config[param]]) if config.has_key?(param)
    end
    args.concat(files)
    for_read, for_write = IO.pipe
    pid = fork do
      STDOUT.reopen(for_write)
      for_write.close
      system(@@mkenums, *args)
      exit!
    end
    for_write.close
    Process.waitpid(pid)
    File.open(output, "w") do |out|
      out.print(for_read.read)
    end
  ensure
    $makefile_created = makefile_created
  end
end
