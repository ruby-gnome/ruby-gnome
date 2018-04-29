class TestGtkMenuItem < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "without arguments" do
      item = Gtk::MenuItem.new()
      assert_equal("", item.label)
    end

    test "with label" do
      item = Gtk::MenuItem.new(:label => "Label")
      assert_equal("Label", item.label)
    end

    test "with mnemonic" do
      item = Gtk::MenuItem.new(:label => "Label", :use_underline => true)
      assert_equal("Label", item.label)
      assert(item.use_underline?)
    end
  end
end
