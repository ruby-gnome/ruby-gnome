class TestGtkWidget < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @widget = Gtk::Invisible.new
  end

  def test_set_window
    attrs = Gdk::WindowAttr.new(100, 100, :only, :temp)
    window = Gdk::Window.new(nil, attrs, 0)
    @widget.window = window
    assert_equal(window, @widget.window)
  end
end
