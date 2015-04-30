class TestGtkWidget < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @widget = Gtk::Invisible.new
  end

  def test_set_window
    attrs = Gdk::WindowAttr.new(100, 100, :input_only, :temp)
    window = Gdk::Window.new(nil, attrs, 0)
    @widget.window = window
    assert_equal(window, @widget.window)
  end

  def test_modify_color_component_with_nil_color
    assert_nothing_raised do
      @widget.override_color(:normal, nil)
    end

    assert_nothing_raised do
      @widget.override_background_color(:normal, nil)
    end
  end

  sub_test_case "#drag_dest_set" do
    test "targets - TargetEntry" do
      target = "text/uri-list"
      info = 0
      @widget.drag_dest_set(:all,
                            [Gtk::TargetEntry.new(target, 0, info)],
                            :copy)
      list = @widget.drag_dest_get_target_list
      assert_equal(info, list.find(target))
    end

    test "targets - Array" do
      target = "text/uri-list"
      info = 0
      @widget.drag_dest_set(:all,
                            [[target, 0, info]],
                            :copy)
      list = @widget.drag_dest_get_target_list
      assert_equal(info, list.find(target))
    end
  end
end
