require 'test/unit'
require 'gtk2'

class TestGC < Test::Unit::TestCase
  priority :must
  def test_closure
    10.times do
      invisible = Gtk::Invisible.new
      invisible.signal_connect("destroy") {}
      GC.start
    end
  end
end
