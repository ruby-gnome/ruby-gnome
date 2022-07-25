# Copyright (C) 2008-2022  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

class TestGtkBuilder < Test::Unit::TestCase
  include GtkTestUtils

  def teardown
    GC.start
  end

  def test_constants
    assert_const_defined(Gtk::BuilderError, :INVALID_TAG)
  end

  sub_test_case ".new" do
    test "no argument" do
      builder = Gtk::Builder.new
      assert_equal([], builder.objects)
    end

    test "file" do
      builder = Gtk::Builder.new(:file => ui_definition_file.path)
      assert_kind_of(Gtk::Dialog, builder["dialog1"])
    end

    test "path" do
      builder = Gtk::Builder.new(:path => ui_definition_file.path)
      assert_kind_of(Gtk::Dialog, builder["dialog1"])
    end

    test "resource" do
      register_resource(fixture_path("simple_window.gresource")) do
        resource_path = "/simple_window/simple_window.ui"
        builder = Gtk::Builder.new(:resource => resource_path)
        assert_kind_of(Gtk::Window, builder["window"])
      end
    end

    test "string" do
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
                    :object_ids => ["button_ok"])
        assert_equal([builder["button_ok"]],
                     builder.objects)
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
                    :object_ids => ["button_ok"])
        assert_equal([builder["button_ok"]],
                     builder.objects)
      end
    end

    test "path" do
      builder = Gtk::Builder.new
      builder.add(ui_definition_file.path)
      assert_kind_of(Gtk::Dialog, builder["dialog1"])
    end

    sub_test_case ":resource" do
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
          assert_equal([builder["label"]],
                       builder.objects)
        end
      end
    end

    test "resource" do
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
                    :object_ids => ["button_ok"])
        assert_equal([builder["button_ok"]],
                     builder.objects)
      end
    end

    test "string" do
      builder = Gtk::Builder.new
      builder.add(ui_definition)
      assert_kind_of(Gtk::Button, builder["button_ok"])
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
    builder << ui_definition_file.path << ui_definition_simple
    assert_equal([
                   builder["dialog1"],
                   builder["button_cancel"],
                   builder["button_ok"],
                   builder["main-window"],
                 ].sort_by(&:object_id),
                 builder.objects.sort_by(&:object_id))
  end

  def test_translation_domain
    builder = Gtk::Builder.new
    assert_nil(builder.translation_domain)
    builder.translation_domain = "ruby-gnome2"
    assert_equal("ruby-gnome2", builder.translation_domain)
  end

  def test_signal_no_object
    builder = Gtk::Builder.new
    builder << ui_definition
    def builder.button_cancel_clicked(*args)
      @button_cancel_clicked = true
    end
    def builder.button_cancel_clicked?
      @button_cancel_clicked
    end
    builder["button_cancel"].signal_emit("clicked")
    assert do
      builder.button_cancel_clicked?
    end
  end

  def test_signal_connect_object
    builder = Gtk::Builder.new
    builder << ui_definition
    button_ok = builder["button_ok"]
    def button_ok.button_ok_clicked(*args)
      @button_ok_clicked = true
    end
    def button_ok.button_ok_clicked?
      @button_ok_clicked
    end
    button_ok.signal_emit("clicked")
    assert do
      button_ok.button_ok_clicked?
    end
  end

  def test_signal_corrent_object
    object = Class.new(GLib::Object).new
    def object.button_cancel_clicked(*args)
      @button_cancel_clicked = true
    end
    def object.button_cancel_clicked?
      @button_cancel_clicked
    end
    builder = Gtk::Builder.new(object: object)
    builder << ui_definition
    builder["button_cancel"].signal_emit("clicked")
    assert do
      object.button_cancel_clicked?
    end
  end

  def test_get_type_from_name
    builder = Gtk::Builder.new
    assert_equal([
                   GLib::Type["void"],
                   Gtk::Box.gtype,
                 ],
                 [
                   builder.get_type("XXX"),
                   builder.get_type("GtkBox"),
                 ])
  end

  private
  def ui_definition
    <<-EOX
<interface>
  <object class="GtkDialog" id="dialog1">
    <child type="action">
      <object class="GtkButton" id="button_cancel">
        <signal name="clicked" handler="button_cancel_clicked"/>
      </object>
    </child>
    <child type="action">
      <object class="GtkButton" id="button_ok">
        <signal name="clicked" object="button_ok" handler="button_ok_clicked"/>
      </object>
    </child>
    <action-widgets>
      <action-widget response="cancel">button_cancel</action-widget>
      <action-widget response="ok" default="true">button_ok</action-widget>
    </action-widgets>
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
