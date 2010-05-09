=begin
extconf.rb for Ruby/BonoboUI2 extention library
=end

PACKAGE_NAME = "bonoboui2"
PACKAGE_ID   = "libbonoboui-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/bonobo/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect BonoboUI configurations
#

(PKGConfig.have_package('libgnomeui-2.0') and 
 PKGConfig.have_package('libbonoboui-2.0')) or exit 1
check_win32

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("gnome2", "gnome/src", TOPDIR)

begin
  srcdir = File.dirname($0) == "." ? "." :
    File.expand_path(File.dirname($0) + "/src")

  create_pkg_config_file("Ruby/BonoboUI2", PACKAGE_ID)

  Dir.mkdir "src" unless FileTest.exist? "src"
  Dir.chdir "src"

  begin
    File.delete("rbbonobouiinits.c") if FileTest.exist?("rbbonobouiinits.c")

    obj_ext = ".#{$OBJEXT}"

    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.entries(srcdir).select{|fname| /\.c$/ =~ fname }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end
    add_obj("rbbonobouiinits#{obj_ext}")
    
    make_version_header("BONOBOUI", PACKAGE_ID, '.')

    #
    # create Makefiles
    #
    create_makefile(PACKAGE_NAME, srcdir)

    mfile = File.open("Makefile", "a")
    $source_files.each do |e|
      mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbbonoboui.h\n"
    end
    mfile.print "\
rbbonobouiinits.c:;    $(RUBY) $(srcdir)/makeinits.rb $(srcdir)/*.c > $@
allclean: clean
	rm -rf *.a
"
    mfile.close
  ensure
    Dir.chdir ".."
  end

  create_top_makefile

rescue Interrupt
#  if $DEBUG
    print "  [error] " + $!.to_s + "\n"
#  end
end
