class TestGtkEntry < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @entry = Gtk::Entry.new
  end

  def test_cursor_hadjustment
    only_gtk_version(2, 12)

    assert_nil(@entry.cursor_hadjustment)
    adjustment = Gtk::Adjustment.new(0.0, 0.0,
                                     100.0, 100.0,
                                     1.0, 1.0)
    @entry.cursor_hadjustment = adjustment
    assert_equal(adjustment, @entry.cursor_hadjustment)
  end
end
