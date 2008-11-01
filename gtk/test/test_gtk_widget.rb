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

  def test_modify_color_component_with_nil_color
    assert_nothing_raised do
      @widget.modify_base(:normal, nil)
    end

    assert_nothing_raised do
      @widget.modify_text(:normal, nil)
    end

    assert_nothing_raised do
      @widget.modify_fg(:normal, nil)
    end

    assert_nothing_raised do
      @widget.modify_bg(:normal, nil)
    end
  end
end
