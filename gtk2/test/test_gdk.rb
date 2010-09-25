class TestGdk < Test::Unit::TestCase
  include GtkTestUtils

  def test_notify_startup_complete
    assert_nothing_raised do
      Gdk.notify_startup_complete
    end

    notify_with_id = Proc.new do
      Gdk.notify_startup_complete("startup-id")
    end
    if Gtk.check_version?(2, 12, 0)
      assert_nothing_raised(&notify_with_id)
    else
      assert_raise(NotImplementedError, &notify_with_id)
    end
  end
end
