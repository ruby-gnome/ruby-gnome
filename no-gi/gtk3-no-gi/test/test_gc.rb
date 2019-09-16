class TestGC < Test::Unit::TestCase
  priority :must
  def test_closure
    10.times do
      invisible = Gtk::Invisible.new
      invisible.signal_connect("destroy") {}
      GC.start
    end
  end

  def test_inheritance_and_gc
    button = Class.new(Gtk::Button)
    box = Gtk::Box.new(:horizontal)
    n = 10
    n.times do
      box.add(button.new)
    end
    GC.start
    assert_equal([button] * 10,
                 box.children.collect {|item| item.class})
  end
end
