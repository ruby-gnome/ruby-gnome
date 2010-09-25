class TestGdkColor < Test::Unit::TestCase
  def test_to_s
    color = Gdk::Color.new(0xffff, 0x1234, 0xabcd)
    if Gtk.check_version?(2, 12, 0)
      assert_equal("#ffff1234abcd", color.to_s)
    else
      assert_match(/\A#<Gdk::Color:.*>\z/, color.to_s)
    end
  end
end
