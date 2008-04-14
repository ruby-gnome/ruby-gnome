module GtkTestUtils
  private
  def only_gtk_version(major, minor, micro)
    unless Gtk.check_version?(major, minor, micro)
      omit("Require GTK+ >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_x11
    omit("Only for X11 backend") unless Gdk.windowing_x11?
  end

  def assert_boolean(expected)
    assert([true, false].include?(expected),
           build_message(nil,
                         "<true or false> expected but was\n<?>",
                         expected))
  end
end
