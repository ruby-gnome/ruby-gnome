class TestGtkAboutDialog < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @dialog = Gtk::AboutDialog.new
  end

  def test_program_name
    assert_equal(File.basename($0), @dialog.program_name)
    @dialog.program_name = "my-program"
    assert_equal("my-program", @dialog.program_name)
  end
end
