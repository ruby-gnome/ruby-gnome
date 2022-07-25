class TestGC < Test::Unit::TestCase
  priority :must
  def test_closure
    klass = Class.new
    n_tries = 100
    n_tries.times do
      widget = Gtk::Box.new(:horizontal)
      closed_object = klass.new
      widget.signal_connect("hide") {p closed_object}
    end
    GC.start
    n_closed_objects = ObjectSpace.each_object(klass) {}
    assert do
      n_closed_objects < n_tries
    end
  end

  def test_inheritance_and_gc
    button = Class.new(Gtk::Button)
    box = Gtk::Box.new(:horizontal)
    n = 10
    n.times do
      box.append(button.new)
    end
    GC.start
    assert_equal([button] * n,
                 box.children.collect {|item| item.class})
  end
end
