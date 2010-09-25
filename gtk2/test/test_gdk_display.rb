class TestGdkDisplay < Test::Unit::TestCase
  include GtkTestUtils

  def test_supports_predicates
    display = Gdk::Display.default

    assert_boolean(display.supports_shapes?)
    assert_boolean(display.supports_input_shapes?)
    only_gtk_version(2, 12, 0)
    assert_boolean(display.supports_composite?)
  end

  def test_startup_notification_id
    only_gtk_version(2, 12, 0)
    only_x11

    display = Gdk::Display.default
    assert_nil(display.startup_notification_id)
  end

  def test_broadcast_startup_message
    omit("Not yet implemented (GNOME bug #528020)")
    only_gtk_version(2, 16, 0)
    only_x11

    id = "#{$$}_TIME_#{Time.now.to_i}"
    screen = Gdk::Screen.default.number.to_s
    assert_nothing_raised do
      Gdk::Display.default.broadcast_startup_message("new",
                                                     "ID" => id,
                                                     "NAME" => "Hello World",
                                                     "SCREEN" => screen)
      Gdk::Display.default.broadcast_startup_message("remove",
                                                     "ID" => id)
    end
  end
end
