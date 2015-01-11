class TestGtkMenuItem < Test::Unit::TestCase
  include GtkTestUtils

  def test_new_with_label
    item = Gtk::MenuItem.new("Label")
    assert_equal("Label", item.label)
  end

  def test_new_with_nil_label
    item = Gtk::MenuItem.new("")
    assert_equal("", item.label)
  end
end
