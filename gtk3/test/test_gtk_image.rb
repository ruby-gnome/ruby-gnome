class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  def test_stock
    image = Gtk::Image.new(:stock => :gtk_go_forward, :size => :dialog)
    assert_equal(Gtk::Stock::GO_FORWARD.to_s, image.stock)
  end
end
