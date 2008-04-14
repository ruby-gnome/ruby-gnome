module GLibTestUtils
  private
  def only_glib_version(major, minor, micro)
    unless GLib.check_version?(major, minor, micro)
      omit("Require GLib >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_win32
    unless GLib.const_defined?(:Win32)
      omit("Only for Win32 platform")
    end
  end
end
