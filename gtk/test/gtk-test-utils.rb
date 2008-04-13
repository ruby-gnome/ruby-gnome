module GtkTestUtils
  private
  def only_gtk_version(major, minor, micro)
    unless Gtk.check_version?(major, minor, micro)
      omit("Require GTK+ >= #{major}.#{minor}.#{micro}")
    end
  end
end
