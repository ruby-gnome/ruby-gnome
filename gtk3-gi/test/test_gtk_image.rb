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

  def test_icon_name
    icon_name = "dialog-password"
    image = Gtk::Image.new(:icon_name => icon_name,
                           :size => Gtk::IconSize::DIALOG)
    assert_equal([
                   icon_name,
                   Gtk::IconSize::DIALOG,
                 ],
                 image.icon_name)
  end
end
