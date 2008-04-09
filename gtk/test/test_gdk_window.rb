class TestGdkWindow < Test::Unit::TestCase
  def setup
    @window = Gtk::Invisible.new.window
  end

  def test_set_composited
    set_composited = Proc.new do
      @window.composited = false
    end
    if Gtk.check_version?(2, 12, 0)
      assert_nothing_raised(&set_composited)
    else
      assert_raise(NameError, &set_composited)
    end
  end
end
