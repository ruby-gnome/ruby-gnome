class TestGtkUnixPrint < Test::Unit::TestCase
  def test_classes
    if GLib.os_unix?
      assertion = :assert_const_defined
    else
      assertion = :assert_not_const_defined
    end
    send(assertion, :PageSetupUnixDialog)
    send(assertion, :Printer)
    send(assertion, :PrintCapabilities)
    send(assertion, :PrintJob)
    send(assertion, :PrintUnixDialog)
  end

  def assert_const_defined(name)
    assert_operator(Gtk, :const_defined?, name)
  end

  def assert_not_const_defined(name)
    assert(Gtk.const_defined?(name), "!Gtk.const_defined?(#{name.inspect})")
  end
end
