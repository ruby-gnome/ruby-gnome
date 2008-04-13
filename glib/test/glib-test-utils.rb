module GLibTestUtils
  private
  def only_glib_version(major, minor, micro)
    unless GLib.check_version?(major, minor, micro)
      omit("Require GLib >= #{major}.#{minor}.#{micro}")
    end
  end
end
