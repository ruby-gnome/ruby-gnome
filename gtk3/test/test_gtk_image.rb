class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  def test_stock
    image = Gtk::Image.new(Gtk::Stock::GO_FORWARD, :dialog)
    assert_equal(Gtk::Stock::GO_FORWARD.to_s, image.stock)
  end
end
