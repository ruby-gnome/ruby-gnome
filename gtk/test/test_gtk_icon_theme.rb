class TestGtkThemeChooseIcon < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @theme = Gtk::IconTheme.default
  end

  def test_choose_icon
    only_gtk_version(2, 12)

    assert_nil(@theme.choose_icon("non-existent", 100))

    icon = @theme.choose_icon("undo", 10)
    assert_not_nil(icon)
    assert_match(/undo/, icon.filename)

    assert_not_nil(@theme.choose_icon("undo", 29, [:use_builtin, :no_svg]))
  end

  def test_contexts
    only_gtk_version(2, 12)

    assert_operator(@theme.contexts, :include?, "Apps")
  end
end
