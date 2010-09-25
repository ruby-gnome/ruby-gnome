class TestGdkWindow < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @window = Gtk::Invisible.new.window
  end

  def test_set_composited
    only_gtk_version(2, 12, 0)
    assert_nothing_raised do
      @window.composited = false
    end
  end

  def test_set_opacity
    only_gtk_version(2, 12, 0)
    assert_nothing_raised do
      @window.opacity = 0.5
    end
  end

  def test_set_startup_id
    only_gtk_version(2, 12, 0)
    assert_nothing_raised do
      @window.startup_id = "startup-id"
      @window.startup_id = nil
    end
  end
end
