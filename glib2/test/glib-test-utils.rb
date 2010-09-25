module GLibTestUtils
  private
  def only_glib_version(major, minor, micro)
    unless GLib.check_version?(major, minor, micro)
      omit("Require GLib >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_win32
    omit("Only for Win32 platform") unless GLib.os_win32?
  end
end
