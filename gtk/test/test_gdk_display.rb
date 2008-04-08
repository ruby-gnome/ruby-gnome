class TestGdkDisplay < Test::Unit::TestCase
  def test_supports_predicates
    display = Gdk::Display.default

    assert_boolean(display.supports_shapes?)
    assert_boolean(display.supports_input_shapes?)
    if Gtk.check_version?(2, 12, 0)
      assert_boolean(display.supports_composite?)
    else
      assert_respond_to(display, :supports_composite?)
    end
  end

  private
  def assert_boolean(expected)
    assert([true, false].include?(expected),
           build_message(nil,
                         "<true or false> expected but was\n<?>",
                         expected))
  end
end
