=begin
extconf.rb for libart extention library
=end

require "mkmf"

config_cmd = with_config("libart2-config", "libart2-config")

begin
  version = `#{config_cmd} --version`
  if not version.chomp.empty?
    $libs += ' ' + `#{config_cmd} --libs`.chomp
    $CFLAGS += ' ' + `#{config_cmd} --cflags`.chomp
  end
rescue
  $libs = '-L/usr/local/lib -lart_lgpl_2 -lm'
  $CFLAGS = '-I/usr/local/include/libart-2.0'
end

$libs += " -lz "
have_library("png", "png_create_write_struct")
have_library("jpeg", "jpeg_set_defaults")

#
# create Makefiles
#
mdir = $mdir
begin
  $mdir = "libart/src"
  Dir.chdir "src"

  lib_ary = [ ["art_lgpl_2", "art_affine_identity"],
  ]

  lib_ary.each do |ary|

    if not have_library(ary[0], ary[1])
      msg = format("cannot found %s in %s.", ary[1], ary[0])
      if ary[0] == "art_lgpl_2"
        msg += " (or maybe `libart2-config --lib' is incorrect...)"
      end
      raise Interrupt, msg
    end
  end

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
	item.gsub(/\.c$/, obj_ext)
  end

  #
  # create Makefiles
  #
  srcdir = File.dirname($0) == "." ? "." : "../src"
  create_makefile("libart2", srcdir)
  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  $source_files.each do |e|
	mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbart.h\n"
  end
  mfile.print "\
allclean: clean
	rm -rf *.a
"
  mfile.close
  Dir.chdir ".."

  mfile = File.open("Makefile", "w")
  mfile.print "\
all:
		@cd src; make all
		@if [ ! -r libart.a ]; then ln -sf src/libart.a libart.a; fi 
	
install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make allclean
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log libart.a
"
  mfile.close

rescue Interrupt
    print "  [error] " + $!.to_s + "\n"
  Dir.chdir ".."
ensure
  $mdir = mdir
end
