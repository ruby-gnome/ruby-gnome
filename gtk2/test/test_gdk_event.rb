class TestGdkEvent < Test::Unit::TestCase
  include GtkTestUtils

  class TestMotion < self
    def test_request
      only_gtk_version(2, 12, 0)
      motion = Gdk::EventMotion.new
      assert_nothing_raised do
        motion.request
      end
    end
  end

  class TestButton < self
    def setup
      @button = Gdk::EventButton.new(:button_press)
    end

    def test_window
      assert_nil(@button.window)
    end
  end
end
