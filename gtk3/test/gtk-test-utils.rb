require 'tempfile'

module GtkTestUtils
  private
  def only_gtk_version(major, minor, micro=nil)
    micro ||= 0
    unless Gtk.check_version?(major, minor, micro)
      omit("Require GTK+ >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_x11
    omit("Only for X11 backend") unless Gdk.windowing_x11?
  end
end
