=begin
extconf.rb for gnome extention library
=end

require "mkmf"

#
# detect Gnome configurations
#

begin
  config_cmd = with_config("gnome-config", "gnome-config")
  config_libs = "--libs"
  config_cflags = "--cflags"
  config_library = "gnomeui"
  version = `#{config_cmd} --version`
  if not version.chomp.empty? then
    $LDFLAGS += ' ' + `#{config_cmd} #{config_libs} #{config_library}`.chomp
    $CFLAGS += " -DHAVE_GDKIMLIB -I../../gtk/src  -I../../gdkimlib " +
      `#{config_cmd} #{config_cflags} #{config_library}`.chomp
  else
    raise "Can't find a config command"
  end
rescue
  $LDFLAGS = '-L/usr/X11R6/lib -L/usr/local/lib'
  $CFLAGS = '-I/usr/X11R6/lib -I/usr/local/include'
 # $libs = '-lgnomeui -lart_lgpl -lgdk_imlib -lSM -lICE -lgtk -lgdk -lgmodule -lXext -lX11 -lm -lgnome -lgnomesupport -ldb -lglib -ldl '
  $libs = '-lgnomeui -lart_lgpl -lgdk_imlib -lSM -lICE -lgtk -lgdk -lgmodule -lXext -lX11 -lm -lgnome -lgnomesupport -ldb -lglib -ldl -lgtkxmhtml'
end

mdir = $mdir
begin
  $mdir = "gnome/src"
  Dir.chdir "src"

  lib_ary = [ ["X11", "XOpenDisplay"],
              ["Xext", "XShmQueryVersion"],
              ["Xi", "XOpenDevice"],
#              ["glib", "g_print"],
#              ["gdk", "gdk_init"],
#              ["gtk", "gtk_init"],
#              ["gnome", "gnome_init"],
  ]

  lib_ary.each do |ary|

    if not have_library(ary[0], ary[1])
      msg = format("cannot found %s in %s.", ary[1], ary[0])
      if ary[0] == "X11"
        msg += " (or maybe `gnome-config --lib' is incorrect...)"
      end
      raise Interrupt, msg
    end
  end
  have_func("XReadBitmapFileData")

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $objs = [
    "rbgnome",
    "rbgtkdial",
    "rbgtk-clock",
    "rbgtk-pixmap-menuitem",
    "rbgnome-about",
    "rbgnome-animator",
    "rbgnome-app",
    "rbgnome-appbar",
    "rbgnome-app-helper",
    "rbgnome-calculator",
    "rbgnome-canvas",
    "rbgnome-canvas-item",
    "rbgnome-canvas-group",
    "rbgnome-client",
    "rbgnome-color-picker",
    "rbgnome-dateedit",
    "rbgnome-dentry-edit",
    "rbgnome-desktop-entry",
    "rbgnome-dialog",
    "rbgnome-dock",
    "rbgnome-dock-band",
    "rbgnome-dock-item",
    "rbgnome-dock-layout",
    "rbgnome-entry",
    "rbgnome-file-entry",
    "rbgnome-font-picker",
    "rbgnome-href",
    "rbgnome-icon-entry",
    "rbgnome-icon-list",
    "rbgnome-icon-selection",
    "rbgnome-less",
    "rbgnome-messagebox",
    "rbgnome-number-entry",
    "rbgnome-paper-selector",
    "rbgnome-pixmap",
    "rbgnome-pixmap-entry",
    "rbgnome-proc-bar",
    "rbgnome-property-box",
    "rbgnome-scores",
    "rbgnome-spell",
    "rbgnome-stock",
    "rbgnome-config",
    "rbgnome-i18n",
    "rbgnome-uidefs",
    "rbgnome-url",
    "rbgnome-util",
    "rbgtk-xmhtml"
  ].collect{|item| item + obj_ext}

  #
  # create Makefiles
  #
  create_makefile("gnome")
  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  mfile.print "\

rbgnome#{obj_ext}: rbgnome.c rbgnome.h
rbgtkdial#{obj_ext}: rbgtkdial.c rbgnome.h
rbgtk-clock#{obj_ext}: rbgtk-clock.c rbgnome.h
rbgtk-pixmap-menuitem#{obj_ext}: rbgtk-pixmap-menuitem.c rbgnome.h
rbgnome-about#{obj_ext}: rbgnome-about.c rbgnome.h
rbgnome-animator#{obj_ext}: rbgnome-animator.c rbgnome.h
rbgnome-app#{obj_ext}: rbgnome-app.c rbgnome.h
rbgnome-appbar#{obj_ext}: rbgnome-appbar.c rbgnome.h
rbgnome-app-helper#{obj_ext}: rbgnome-app-helper.c rbgnome.h
rbgnome-calculator#{obj_ext}: rbgnome-calculator.c rbgnome.h
rbgnome-canvas#{obj_ext}: rbgnome-canvas.c rbgnome.h
rbgnome-canvas-item#{obj_ext}: rbgnome-canvas-item.c rbgnome.h
rbgnome-canvas-group#{obj_ext}: rbgnome-canvas-group.c rbgnome.h
rbgnome-client#{obj_ext}: rbgnome-client.c rbgnome.h
rbgnome-color-picker#{obj_ext}: rbgnome-color-picker.c rbgnome.h
rbgnome-dateedit#{obj_ext}: rbgnome-dateedit.c rbgnome.h
rbgnome-dentry-edit#{obj_ext}: rbgnome-dentry-edit.c rbgnome.h
rbgnome-desktop-entry#{obj_ext}: rbgnome-desktop-entry.c rbgnome.h
rbgnome-dialog#{obj_ext}: rbgnome-dialog.c rbgnome.h
rbgnome-dock#{obj_ext}: rbgnome-dock.c rbgnome.h
rbgnome-dock-band#{obj_ext}: rbgnome-dock-band.c rbgnome.h
rbgnome-dock-item#{obj_ext}: rbgnome-dock-item.c rbgnome.h
rbgnome-dock-layout#{obj_ext}: rbgnome-dock-layout.c rbgnome.h
rbgnome-entry#{obj_ext}: rbgnome-entry.c rbgnome.h
rbgnome-file-entry#{obj_ext}: rbgnome-file-entry.c rbgnome.h
rbgnome-font-picker#{obj_ext}: rbgnome-font-picker.c rbgnome.h
rbgnome-href#{obj_ext}: rbgnome-href.c rbgnome.h
rbgnome-icon-entry#{obj_ext}: rbgnome-icon-entry.c rbgnome.h
rbgnome-icon-list#{obj_ext}: rbgnome-icon-list.c rbgnome.h
rbgnome-icon-selection#{obj_ext}: rbgnome-icon-selection.c rbgnome.h
rbgnome-less#{obj_ext}: rbgnome-less.c rbgnome.h
rbgnome-messagebox#{obj_ext}: rbgnome-messagebox.c rbgnome.h
rbgnome-number-entry#{obj_ext}: rbgnome-number-entry.c rbgnome.h
rbgnome-paper-selector#{obj_ext}: rbgnome-paper-selector.c rbgnome.h
rbgnome-pixmap#{obj_ext}: rbgnome-pixmap.c rbgnome.h
rbgnome-pixmap-entry#{obj_ext}: rbgnome-pixmap-entry.c rbgnome.h
rbgnome-property-box#{obj_ext}: rbgnome-property-box.c rbgnome.h
rbgnome-proc-bar#{obj_ext}: rbgnome-proc-bar.c rbgnome.h
rbgnome-scores#{obj_ext}: rbgnome-scores.c rbgnome.h
rbgnome-spell#{obj_ext}: rbgnome-spell.c rbgnome.h
rbgnome-stock#{obj_ext}: rbgnome-stock.c rbgnome.h
rbgnome-config#{obj_ext}: rbgnome-config.c rbgnome.h
rbgnome-i18n#{obj_ext}: rbgnome-i18n.c rbgnome.h
rbgnome-uidefs#{obj_ext}: rbgnome-uidefs.c rbgnome.h
rbgnome-url#{obj_ext}: rbgnome-url.c rbgnome.h
rbgnome-util#{obj_ext}: rbgnome-util.c rbgnome.h
rbgtk-xmhtml#{obj_ext}: rbgtk-xmhtml.c rbgnome.h

allclean: clean
	rm -rf *.a
"
  mfile.close
  Dir.chdir ".."

  mfile = File.open("Makefile", "w")
  mfile.print "\

all:
		@cd src; make all
		@if [ ! -r gnome.a ]; then ln -sf src/gnome.a gnome.a; fi 
	
install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make allclean
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log gnome.a
"
  mfile.close

rescue Interrupt
#  if $DEBUG
    print "  [error] " + $!.to_s + "\n"
#  end
  Dir.chdir ".."
ensure
  $mdir = mdir
end
