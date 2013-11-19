class TestGtkIconTheme < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @theme = Gtk::IconTheme.default
  end

  def test_choose_icon
    assert_nil(@theme.choose_icon("non-existent", 100))

    icon = @theme.choose_icon("face-cool", 10)
    assert_not_nil(icon)
    assert_match(/face-cool/, icon.filename)

    assert_not_nil(@theme.choose_icon("face-cool", 29, [:use_builtin, :no_svg]))
  end

  def test_contexts
    assert_operator(@theme.contexts, :include?, "MimeTypes")
  end

  def test_icons
    assert_operator(@theme.icons, :include?, "face-cool")
    assert_operator(@theme.icons("Actions"), :include?, "find")
  end
end
