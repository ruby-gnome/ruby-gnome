class TestGtkIconTheme < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @theme = Gtk::IconTheme.default
  end

  def test_choose_icon
    only_gtk_version(2, 12)

    assert_nil(@theme.choose_icon("non-existent", 100))

    icon = @theme.choose_icon("face-cool", 10)
    assert_not_nil(icon)
    assert_match(/face-cool/, icon.filename)

    assert_not_nil(@theme.choose_icon("face-cool", 29, [:use_builtin, :no_svg]))
  end

  def test_contexts
    only_gtk_version(2, 12)

    assert do
      @theme.contexts.include?("MimeTypes")
    end
  end

  class TestIcons < self
    def test_no_argument
      assert do
        @theme.icons.include?("face-cool")
      end
    end

    def test_context
      assert do
        @theme.icons("Emotes").any? {|name| name.start_with?("face-cool")}
      end
    end
  end
end
