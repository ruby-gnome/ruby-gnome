class TestGtkWindowAttribute < Test::Unit::TestCase
  include GtkTestUtils

  def test_initialize
    attrs = Gdk::WindowAttr.new(100, 100, :only, :temp)
    assert_equal([100, 100, Gdk::Window::INPUT_ONLY, Gdk::Window::TEMP],
                 [attrs.width, attrs.height, attrs.wclass, attrs.window_type])
  end
end
