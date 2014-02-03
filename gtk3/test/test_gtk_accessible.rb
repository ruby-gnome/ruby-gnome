class TestGtkAccessible < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @accessible = Gtk::Accessible.new
    @widget = Gtk::Box.new(:horizontal)
  end

  def test_widget_accessors
    assert_nothing_raised do
      @accessible.widget = @widget
      @accessible.widget
    end
  end
end
