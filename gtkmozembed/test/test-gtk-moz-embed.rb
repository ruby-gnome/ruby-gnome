class TestGtkMozEmbed < Test::Unit::TestCase
  include GtkMozEmbedTestUtils

  def setup
    @embed = Gtk::MozEmbed.new
  end

  def test_flags_class
    assert_const_defined(Gtk::MozEmbed, :ProgressFlags)
    assert_const_defined(Gtk::MozEmbed, :StatusFlags)
    assert_const_defined(Gtk::MozEmbed, :ReloadFlags)
    assert_const_defined(Gtk::MozEmbed, :ChromeFlags)
  end

  def test_flags_shortcut
    assert_const_defined(Gtk::MozEmbed, :START)

    assert_const_defined(Gtk::MozEmbed, :FAILED_DNS)
    assert_const_defined(Gtk::MozEmbed, :FAILED_USER_CANCELED)

    assert_const_defined(Gtk::MozEmbed, :RELOAD_NORMAL)
    assert_const_defined(Gtk::MozEmbed, :RELOAD_BYPASS_CACHE)
    assert_const_defined(Gtk::MozEmbed, :RELOAD_BYPASS_PROXY)
    assert_const_defined(Gtk::MozEmbed, :RELOAD_BYPASS_PROXY_AND_CACHE)
    assert_const_defined(Gtk::MozEmbed, :RELOAD_CHARSET_CHANGE)

    assert_const_defined(Gtk::MozEmbed, :DEFAULT_CHROME)
    assert_const_defined(Gtk::MozEmbed, :WINDOW_CLOSE_ON)
    assert_const_defined(Gtk::MozEmbed, :MENU_BARON)
    assert_const_defined(Gtk::MozEmbed, :PERSONAL_TOOLBAR_ON)
    assert_const_defined(Gtk::MozEmbed, :SCROLLBARS_ON)
    assert_const_defined(Gtk::MozEmbed, :EXTRA_CHROME_ON)
    assert_const_defined(Gtk::MozEmbed, :ALL_CHROME)
    assert_const_defined(Gtk::MozEmbed, :WINDOW_RAISED)
    assert_const_defined(Gtk::MozEmbed, :CENTER_SCREEN)
    assert_const_defined(Gtk::MozEmbed, :MODAL)
    assert_const_defined(Gtk::MozEmbed, :OPEN_AS_DIALOG)
  end
end
