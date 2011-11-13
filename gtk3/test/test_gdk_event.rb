class TestGdkEvent < Test::Unit::TestCase
  include GtkTestUtils

  def test_motion_request
    only_gtk_version(2, 12, 0)
    motion = Gdk::EventMotion.new
    assert_nothing_raised do
      motion.request
    end
  end
end
