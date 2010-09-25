class TestSelectionData < Test::Unit::TestCase
  def setup
    @window = Gtk::Invisible.new.window
  end

  def test_type_integer
    name = "ID"
    value = 10
    Gdk::Property.change(@window, name,
                         Gdk::Selection::TYPE_INTEGER,
                         :replace, value)
    assert_equal([Gdk::Selection::TYPE_INTEGER, [value]],
                 Gdk::Property.get(@window, name,
                                   Gdk::Selection::TYPE_INTEGER,
                                   false)[0, 2])
  end
end
