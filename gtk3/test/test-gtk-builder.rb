# -*- coding: utf-8 -*-

class TestGtkBuilder < Test::Unit::TestCase
  include GtkTestUtils

  def test_constants
    assert_const_defined(Gtk::BuilderError, :INVALID_TAG)
  end

  sub_test_case ".new" do
    test "no argument" do
      builder = Gtk::Builder.new
      assert_equal([], builder.objects)
    end

    test "file" do
      only_gtk_version(3, 10, 0)
      builder = Gtk::Builder.new(:file => ui_definition_file.path)
      assert_kind_of(Gtk::Dialog, builder["dialog1"])
    end

    test "path" do
      only_gtk_version(3, 10, 0)
      builder = Gtk::Builder.new(:path => ui_definition_file.path)
      assert_kind_of(Gtk::Dialog, builder["dialog1"])
    end

    test "resource" do
      only_gtk_version(3, 12, 0)
      register_resource(fixture_path("simple_window.gresource")) do
        resource_path = "/simple_window/simple_window.ui"
        builder = Gtk::Builder.new(:resource => resource_path)
        assert_kind_of(Gtk::Window, builder["window"])
      end
    end

    test "string" do
      only_gtk_version(3, 10, 0)
      builder = Gtk::Builder.new(:string => ui_definition_simple)
      assert_kind_of(Gtk::Window, builder["main-window"])
    end
  end

  sub_test_case "#add" do
    sub_test_case ":file" do
      test "all" do
        builder = Gtk::Builder.new
        builder.add(:file => ui_definition_file.path)
        assert_kind_of(Gtk::Dialog, builder["dialog1"])
      end

      test ":object_ids" do
        builder = Gtk::Builder.new
        builder.add(:file => ui_definition_file.path,
                    :object_ids => ["ok_button"])
        assert_equal(["ok_button"],
                     builder.objects.collect(&:builder_name).sort)
      end
    end

    sub_test_case ":path" do
      test "all" do
        builder = Gtk::Builder.new
        builder.add(:path => ui_definition_file.path)
        assert_kind_of(Gtk::Dialog, builder["dialog1"])
      end

      test ":object_ids" do
        builder = Gtk::Builder.new
        builder.add(:path => ui_definition_file.path,
                    :object_ids => ["ok_button"])
        assert_equal(["ok_button"],
                     builder.objects.collect(&:builder_name).sort)
      end
    end

    test "path" do
      builder = Gtk::Builder.new
      builder.add(ui_definition_file.path)
      assert_kind_of(Gtk::ButtonBox, builder["hbuttonbox1"])
    end

    sub_test_case ":resource" do
      setup do
        only_gtk_version(3, 12, 0)
      end

      test "all" do
        builder = Gtk::Builder.new
        register_resource(fixture_path("simple_window.gresource")) do
          resource_path = "/simple_window/simple_window.ui"
          builder.add(:resource => resource_path)
          assert_kind_of(Gtk::Window, builder["window"])
        end
      end

      test ":object_ids" do
        builder = Gtk::Builder.new
        register_resource(fixture_path("simple_window.gresource")) do
          resource_path = "/simple_window/simple_window.ui"
          builder.add(:resource => resource_path,
                      :object_ids => ["label"])
          assert_equal(["label"],
                       builder.objects.collect(&:builder_name).sort)
        end
      end
    end

    test "resource" do
      only_gtk_version(3, 12, 0)
      builder = Gtk::Builder.new
      register_resource(fixture_path("simple_window.gresource")) do
        resource_path = "/simple_window/simple_window.ui"
        builder.add(resource_path)
        assert_kind_of(Gtk::Window, builder["window"])
      end
    end

    sub_test_case ":string" do
      test "ascii only" do
        builder = Gtk::Builder.new
        builder.add(:string => ui_definition_simple)
        assert_kind_of(Gtk::Window, builder["main-window"])
      end

      test "multibyte characters" do
        builder = Gtk::Builder.new
        comment = "<!-- 日本語 -->\n" * 100
        builder.add(:string => "#{comment}#{ui_definition_simple}")
        assert_kind_of(Gtk::Window, builder["main-window"])
      end

      test ":object_ids" do
        builder = Gtk::Builder.new
        builder.add(:string => ui_definition,
                    :object_ids => ["ok_button"])
        assert_equal(["ok_button"],
                     builder.objects.collect(&:builder_name).sort)
      end
    end

    test "string" do
      builder = Gtk::Builder.new
      builder.add(ui_definition)
      assert_kind_of(Gtk::Button, builder["ok_button"])
    end
  end

  def test_add_chain
    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder << ui_definition_file.path << ui_definition_simple
    end
    assert_kind_of(Gtk::Dialog, builder["dialog1"])
    assert_kind_of(Gtk::Window, builder["main-window"])
  end

  def test_objects
    builder = Gtk::Builder.new
    assert_nothing_raised do
      builder << ui_definition_file.path << ui_definition_simple
    end
    assert_equal(["dialog1", "vbox1", "hbuttonbox1", "ok_button",
                  "main-window"].sort,
                 builder.objects.collect {|object| object.builder_name}.sort)
  end

  def test_translation_domain
    builder = Gtk::Builder.new
    assert_nil(builder.translation_domain)
    builder.translation_domain = "ruby-gnome2"
    assert_equal("ruby-gnome2", builder.translation_domain)
  end

  def test_connect_signals
    handler_names = []

    builder = Gtk::Builder.new
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal([].sort, handler_names.sort)

    builder << ui_definition
    builder.connect_signals do |name|
      handler_names << name
      Proc.new {}
    end
    assert_equal(["ok_button_clicked"].sort, handler_names.sort)
  end

  def test_connect_signals_with_no_signal_ui_definition
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
    builder = Gtk::Builder.new
    assert_nil(builder.get_type("XXX"))
    assert_equal(Gtk::Box.gtype, builder.get_type("GtkBox"))
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

  def register_resource(path)
    resource = Gio::Resource.load(path)
    Gio::Resources.register(resource)
    begin
      yield
    ensure
      Gio::Resources.unregister(resource)
    end
  end
end
