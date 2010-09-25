class TestGtkBuildable < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @buildable = Gtk::HBox.new
  end

  def test_name
    only_gtk_version(2, 12, 0)

    assert_equal("", @buildable.name)
    @buildable.name = "YYY"
    assert_equal("YYY", @buildable.name)

    @buildable.name = nil
    assert_equal("", @buildable.name)
  end

  def test_add_child
    only_gtk_version(2, 12, 0)

    assert_equal([], @buildable.children)

    builder = Gtk::Builder.new
    entry = Gtk::Entry.new
    @buildable.add_child(builder, entry)
    assert_equal([entry], @buildable.children)
  end

  def test_set_buildable_property
    only_gtk_version(2, 12, 0)

    assert_nil(@buildable.parent)

    builder = Gtk::Builder.new
    parent = Gtk::HBox.new
    @buildable.set_buildable_property(builder, "parent", parent)
    assert_equal(parent, @buildable.parent)
  end

  def test_construct_child
    only_gtk_version(2, 12, 0)

    assert_nil(@buildable.parent)

    @buildable = Gtk::UIManager.new
    @buildable.add_ui(<<-EOU)
<ui>
  <toolbar name='Toolbar'>
  </toolbar>
</ui>
EOU
    builder = Gtk::Builder.new
    assert_kind_of(Gtk::Toolbar, @buildable.construct_child(builder, "Toolbar"))
  end

  def test_get_internal_child
    only_gtk_version(2, 12, 0)

    assert_nil(@buildable.parent)

    @buildable = Gtk::Dialog.new
    builder = Gtk::Builder.new
    assert_kind_of(Gtk::VBox, @buildable.get_internal_child(builder, "vbox"))
  end
end
