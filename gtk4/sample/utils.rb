
def load_gtk4
  ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
  ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

  glib_base = File.join(ruby_gnome2_base, "glib2")
  gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
  atk_base = File.join(ruby_gnome2_base, "atk")
  cairo_gobject_base = File.join(ruby_gnome2_base, "cairo-gobject")
  pango_base = File.join(ruby_gnome2_base, "pango")
  gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
  gio2_base = File.join(ruby_gnome2_base, "gio2")
  gdk4_base = File.join(ruby_gnome2_base, "gdk4")
  gtk4_base = File.join(ruby_gnome2_base, "gtk4")

  [
    [glib_base, "glib2"],
    [gobject_introspection_base, "gobject-introspection"],
    [atk_base, "atk"],
    [cairo_gobject_base, "cairo-gobject"],
    [pango_base, "pango"],
    [gdk_pixbuf_base, "gdk_pixbuf2"],
    [gio2_base, "gio2"],
    [gdk4_base, "gdk4"],
    [gtk4_base, "gtk4"]
  ].each do |target, module_name|
    if File.exist?(File.join(target, "Makefile"))
      if system("which make > /dev/null")
        `make -C #{target.dump} > /dev/null` or exit(false)
      end
      $LOAD_PATH.unshift(File.join(target, "ext", module_name))
    end
    $LOAD_PATH.unshift(File.join(target, "lib"))
  end

  require "gtk4"
end

