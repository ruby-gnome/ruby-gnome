class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  def test_stock
    image = Gtk::Image.new(:stock => Gtk::Stock::GO_FORWARD,
                           :size => :dialog)
    assert_equal([
                   Gtk::Stock::GO_FORWARD.to_s,
                   Gtk::IconSize::DIALOG,
                 ],
                 image.stock)
  end
end
