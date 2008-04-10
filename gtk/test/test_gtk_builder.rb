require 'tempfile'

class TestGtkBuilder < Test::Unit::TestCase
  def test_constants
    access_builder_error = Proc.new do
      assert_kind_of(Gtk::BuilderError, Gtk::BuilderError::INVALID_TAG)
    end

    if Gtk.check_version?(2, 12, 0)
      assert_nothing_raised(&access_builder_error)
    else
      assert_raise(NameError, &access_builder_error)
    end
  end

  def test_new
    create_builder = Proc.new do
      Gtk::Builder.new
    end

    if Gtk.check_version?(2, 12, 0)
      assert_nothing_raised(&create_builder)
    else
      assert_raise(NameError, &create_builder)
    end
  end

  def test_add_from_file
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder.add_from_file(ui_definition_file.path)
    end
    assert_kind_of(Gtk::Dialog, builder["dialog1"])
  end

  def test_add_from_string
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder.add_from_string(ui_definition)
    end
    assert_kind_of(Gtk::VBox, builder["vbox1"])
  end

  def test_add_with_file
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder.add(ui_definition_file.path)
    end
    assert_kind_of(Gtk::HButtonBox, builder["hbuttonbox1"])
  end

  def test_add_with_string
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder.add(ui_definition)
    end
    assert_kind_of(Gtk::Button, builder["ok_button"])
  end

  def test_add_chain
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder << ui_definition_file.path << ui_definition_simple
    end
    assert_kind_of(Gtk::Dialog, builder["dialog1"])
    assert_kind_of(Gtk::Window, builder["main-window"])
  end

  def test_objects
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder << ui_definition_file.path << ui_definition_simple
    end
    assert_equal(["dialog1", "vbox1", "hbuttonbox1", "ok_button",
                  "main-window"].sort,
                 builder.objects.collect {|object| object.name}.sort)
  end

  def test_translation_domain
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nil(builder.translation_domain)
    builder.translation_domain = "ruby-gnome2"
    assert_equal("ruby-gnome2", builder.translation_domain)
  end

  def test_connect_signals
    return unless Gtk.check_version?(2, 12, 0)

    handler_names = []

    builder = Gtk::Builder.new
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal([].sort, handler_names.sort)

    builder << ui_definition
    ok_button = builder["ok_button"]
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal(["ok_button_clicked"].sort, handler_names.sort)
  end

  def test_connect_signals_with_no_signal_ui_definition
    return unless Gtk.check_version?(2, 12, 0)

    handler_names = []

    builder = Gtk::Builder.new
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal([].sort, handler_names.sort)

    builder << ui_definition_simple
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal([].sort, handler_names.sort)
  end

  def test_get_type_from_name
    return unless Gtk.check_version?(2, 12, 0)

    builder = Gtk::Builder.new
    assert_nil(builder.get_type("XXX"))
    assert_equal(Gtk::VBox, builder.get_type("GtkVBox"))
  end

  private
  def ui_definition
    <<-EOX
<interface>
  <object class="GtkDialog" id="dialog1">
    <child internal-child="vbox">
      <object class="GtkVBox" id="vbox1">
        <property name="border-width">10</property>
        <child internal-child="action_area">
          <object class="GtkHButtonBox" id="hbuttonbox1">
            <property name="border-width">20</property>
            <child>
              <object class="GtkButton" id="ok_button">
                <property name="label">gtk-ok</property>
                <property name="use-stock">TRUE</property>
                <signal name="clicked" handler="ok_button_clicked"/>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>
EOX
  end

  def ui_definition_simple
    <<-EOX
<interface>
  <object class="GtkWindow" id="main-window">
  </object>
</interface>
EOX
  end

  def ui_definition_with_signal
    <<-EOX
<interface>
  <object class="GtkWindow" id="main-window">
  </object>
</interface>
EOX
  end

  def ui_definition_file
    xml = Tempfile.new("Gtk::Builder")
    xml.print(ui_definition)
    xml.close
    xml
  end
end
