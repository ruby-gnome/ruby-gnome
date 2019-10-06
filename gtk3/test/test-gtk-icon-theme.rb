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

  class TestIcons < self
    def test_no_argument
      assert_operator(@theme.icons, :include?, "face-cool")
    end

    def test_context
      assert_operator(@theme.icons("Actions"), :include?, "gtk-edit")
    end
  end

  sub_test_case "#lookup_icon" do
    sub_test_case "icon" do
      test "String" do
        assert_not_nil(@theme.lookup_icon("edit-find",
                                          16, :generic_fallback))
      end

      test "Symbol" do
        assert_not_nil(@theme.lookup_icon("edit-find",
                                          16, :generic_fallback))
      end

      test "Gio::Icon" do
        icon = Gio::ThemedIcon.new("edit-find")
        assert_not_nil(@theme.lookup_icon(icon, 16))
      end
    end
  end
end
