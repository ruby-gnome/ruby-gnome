=begin
extconf.rb for Ruby/RSVG extention library
=end

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

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

pkg_infos   = [
  ['librsvg-2.0', []],
]

pkg_infos.each do |name, version|
	PKGConfig.have_package(name, *version) or exit 1
end

check_win32
have_func("rsvg_set_default_dpi_x_y")
have_func("rsvg_handle_set_dpi_x_y")

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
  ].each{|d,l|
    $libs << " -l#{l}"
    $LDFLAGS << " -L#{top}/#{d}"
  }
end

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  enum_type_prefix = "librsvg-enum-types"
  if !have_header("#{enum_type_prefix}.h")
    include_paths = `pkg-config librsvg-2.0 --cflags-only-I`
    include_path = include_paths.split.find do |x|
      /librsvg/.match(x)
    end.sub(/^-I/, "")
    headers = Dir.glob(File.join(include_path, "librsvg", "*.h"))
    mkenums_h(enum_type_prefix, headers)
    mkenums_c(enum_type_prefix, headers)
  end

  if $distcleanfiles
    %w(c h).each do |ext|
      $distcleanfiles << "#{enum_type_prefix}.#{ext}"
    end
  end
  
  create_makefile("rsvg2", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile
