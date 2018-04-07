class TestGtkObject < Test::Unit::TestCase
  include GtkTestUtils

  def test_defined
    assert_equal("Gtk::Object", Gtk::Object.name)
  end
end
