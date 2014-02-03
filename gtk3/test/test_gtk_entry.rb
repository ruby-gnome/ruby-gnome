class TestGtkEntry < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @entry = Gtk::Entry.new
  end

  def test_cursor_hadjustment
    assert_nil(@entry.cursor_hadjustment)
    adjustment = Gtk::Adjustment.new(0.0, 0.0,
                                     100.0, 100.0,
                                     1.0, 1.0)
    @entry.cursor_hadjustment = adjustment
    assert_equal(adjustment, @entry.cursor_hadjustment)
  end

  def test_completion_accsessors
    entry_completion = Gtk::EntryCompletion.new
    @entry.completion = entry_completion
    assert_equal(Gtk::EntryCompletion, @entry.completion.class)
  end
end
