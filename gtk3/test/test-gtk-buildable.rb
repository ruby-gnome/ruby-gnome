class TestGtkBuildable < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @buildable = Gtk::Box.new(:horizontal)
  end

  def test_builder_name
    assert_nil(@buildable.builder_name)
    @buildable.builder_name = "YYY"
    assert_equal("YYY", @buildable.builder_name)

    @buildable.builder_name = ""
    assert_equal("", @buildable.builder_name)
  end

  def test_add_child
    assert_equal([], @buildable.children)

    builder = Gtk::Builder.new
    entry = Gtk::Entry.new
    @buildable.add_child(builder, entry)
    assert_equal([entry], @buildable.children)
  end

  def test_set_buildable_property
    assert_nil(@buildable.parent)

    builder = Gtk::Builder.new
    parent = Gtk::Box.new(:horizontal)
    @buildable.set_buildable_property(builder, "parent", parent)
    assert_equal(parent, @buildable.parent)
  end

  def test_construct_child
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
    assert_nil(@buildable.parent)

    @buildable = Gtk::Dialog.new
    builder = Gtk::Builder.new
    assert_kind_of(Gtk::Box, @buildable.get_internal_child(builder, "vbox"))
  end
end
