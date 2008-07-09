class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  def test_stock
    image = Gtk::Image.new(Gtk::Stock::GO_FORWARD, Gtk::IconSize::DIALOG)
    assert_equal(Gtk::Stock::GO_FORWARD, image.stock)
  end
end
