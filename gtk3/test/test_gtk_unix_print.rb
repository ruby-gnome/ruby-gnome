class TestGtkUnixPrint < Test::Unit::TestCase
  def test_classes
    if GLib.os_unix?
      assertion = :assert_const_defined
    else
      assertion = :assert_not_const_defined
    end
    send(assertion, Gtk, :PageSetupUnixDialog)
    send(assertion, Gtk, :Printer)
    send(assertion, Gtk, :PrintCapabilities)
    send(assertion, Gtk, :PrintJob)
    send(assertion, Gtk, :PrintUnixDialog)
  end
end
