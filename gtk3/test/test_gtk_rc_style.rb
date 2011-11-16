class TestGtkRcStyle < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @style = Gtk::RcStyle.new
  end

  def test_color_flags
    assert_equal(Gtk::RC::Flags.new, @style.color_flags(:normal))
    @style.set_color_flags(:normal, [:fg, :bg])
    assert_equal(Gtk::RC::Flags::FG | Gtk::RC::Flags::BG,
                 @style.color_flags(:normal))
  end
end
