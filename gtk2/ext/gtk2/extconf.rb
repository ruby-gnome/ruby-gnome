=begin
extconf.rb for Ruby/GTK extention library
=end

require 'pathname'

source_dir = Pathname(__FILE__).dirname
base_dir = source_dir.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gtk2"
package_id = "gtk+-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package('gthread-2.0')
PKGConfig.have_package(package_id) or exit 1

have_header("st.h")
have_header("ruby/st.h")

STDOUT.print("checking for target... ")
STDOUT.flush
target = PKGConfig.variable(package_id, "target")
$defs << "-DRUBY_GTK2_TARGET=\\\"#{target}\\\""
STDOUT.print(target, "\n")

gdkincl = nil
tmpincl = $CFLAGS.gsub(/-D\w+/, '').split(/-I/) + ['/usr/include']
tmpincl.each do |i|
  i.strip!

  if FileTest.exist?(i + "/gdk/gdkkeysyms.h")
    gdkincl = i + "/gdk"
    break
  end
end
raise "can't find gdkkeysyms.h" if gdkincl.nil?

gtk_header = "gtk/gtk.h"
have_func('gtk_plug_get_type', gtk_header)
have_func('gtk_socket_get_type', gtk_header)
have_func('pango_render_part_get_type', gtk_header)
have_header('gtk/gtkfilesystem.h') do |src|
  "#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED\n#{src}"
end

xlib_header = "X11/Xlib.h"
if target == "x11" and have_header('X11/Xlib.h') and have_library("X11")
  have_func("XReadBitmapFileData", xlib_header)
  have_func("XGetErrorText", xlib_header)
end

if target != "win32" and PKGConfig.have_package('gtk+-unix-print-2.0')
  $defs.push("-DHAVE_GTK_UNIX_PRINT")
end

have_func("rb_errinfo")

options = {}
rcairo_source_dir_names = ["rcairo"]
if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
  rcairo_source_dir_names.unshift("rcairo.win32")
end
rcairo_source_dir_names.each do |rcairo_source_dir_name|
  rcairo_source_dir = top_dir.parent.expand_path + rcairo_source_dir_name
  if rcairo_source_dir.exist?
    options[:rcairo_source_dir] = rcairo_source_dir.to_s
    break
  end
end
check_cairo(options)

["glib2", "pango"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

create_pkg_config_file("Ruby/GTK2", package_id, ruby_gnome2_version)

class InitCreator
  def initialize(output)
    @output = output
    @targets = []
  end

  def <<(target)
    @targets << target
  end

  def run
    inits = Hash.new

    except_targets = ["Init_gtk2()", "Init_gtk_gdk()", "Init_gtk_gtk()"]
    @targets.each do |target|
      target.each_line do |line|
        if /^(Init_.*\(\))(.*)/ =~ line
          init = $1
          unless except_targets.include?(init)
            flag = $2
            if flag.size > 0
              if flag =~ /\/\*\s*(\w*)\s*\*\//
                inits[$1] = Array.new unless inits[$1]
                inits[$1] << init
              end
            else
              inits[""] = Array.new unless inits[""]
              inits[""] << init
            end
          end
        end
      end
    end

    print_data(inits, "", nil, true)
    print_data(inits, "GTK_DISABLE_DEPRECATED", "ifndef", true)
    print_data(inits, "GTK_ENABLE_BROKEN", "ifdef", true)

    print "void Init_gtk_inits()\n"
    print "{\n"
    print_data(inits, "", nil)
    print_data(inits, "GTK_DISABLE_DEPRECATED", "ifndef")
    print_data(inits, "GTK_ENABLE_BROKEN", "ifdef")
    print "}\n"
  end

  def print_data(array, type, defs, extern = false)
    if array[type]
      dependencies = {
        "Init_gtk_gdk_gc()" => ["Init_gtk_gdk_draw()"],
      }
      extern_def = "extern void" if extern
      print "##{defs} #{type}\n" if defs
      sorted_array = array[type].dup
      dependencies.each do |key, values|
        key_index = sorted_array.index(key)
        values.each do |value|
          value_index = sorted_array.index(value)
          next if value_index.nil?
          sorted_array.delete(value)
          sorted_array[key_index - 1, 0] = value
          key_index = sorted_array.index(key)
        end
      end
      sorted_array.each do |val|
        print "#{extern_def}   #{val};\n"
      end
      print "#endif\n" if defs
    end
  end

  def print(*args)
    @output.print(*args)
  end
end

rbgtkinits_c_path = Pathname("rbgtkinits.c")
rbgtkinits_c_path.open("w") do |rbgtkinits_c|
  init_creator = InitCreator.new(rbgtkinits_c)
  source_dir.each_entry do |entry|
    next unless entry.extname == ".c"
    init_creator << (source_dir + entry)
  end
  init_creator.run
end

rbgdkkeysyms_h_path = Pathname("rbgdkkeysyms.h")
rbgdkkeysyms_h_path.open("w") do |rbgdkkeysyms_h|
  gdkkeysyms_h_path = Pathname(gdkincl) + "gdkkeysyms.h"
  gdkkeysyms_h_path.each_line do |line|
    if /^#define\s+(GDK_\w+)\s+\d+/ =~ line
      define_line = "rb_define_const(mGdkKeyval, \"#{$1}\", INT2FIX(#{$1}));"
      rbgdkkeysyms_h.puts(define_line)
    end
  end
end

add_distcleanfile("rbgdkkeysyms.h")
add_distcleanfile("rbgtkinits.c")

ensure_objs

$defs << "-DRUBY_GTK2_COMPILATION"
create_makefile(module_name)
