# Copyright (C) 2013-2016  Ruby-GNOME2 Project Team
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

class TestGtkContainer < Test::Unit::TestCase
  include GtkTestUtils

  class TestClassMethods < self
    def test_child_properties
      assert_equal(["x", "y"],
                   Gtk::Layout.child_properties.collect(&:name))
    end
  end

  class TestAdd < self
    def setup
      @container = Gtk::Layout.new
    end

    def test_shift
      element1 = Gtk::EventBox.new
      element2 = Gtk::EventBox.new
      element3 = Gtk::EventBox.new
      @container << element1 << element2 << element3
      assert_equal([element1, element2, element3],
                   @container.children)
    end
  end

  class TestEach < self
    def setup
      @container = Gtk::Layout.new
      @element1 = Gtk::EventBox.new
      @element2 = Gtk::EventBox.new
      @element3 = Gtk::EventBox.new
      @container << @element1 << @element2 << @element3
    end

    def test_enumerable
      assert_equal([@element1, @element2, @element3],
                   @container.to_a)
    end
  end

  class TestEachAll < self
    def setup
      @scrolled_window = Gtk::ScrolledWindow.new
    end

    def test_enumerable
      assert_equal([@scrolled_window.vscrollbar, @scrolled_window.hscrollbar],
                   @scrolled_window.each_all.to_a)
    end
  end

  class TestFocusChain < self
    def setup
      @container = Gtk::Layout.new
    end

    def test_set_explicitly
      @container.focus_chain = []
      assert_equal([], @container.focus_chain)
    end

    def test_not_set_explicitly
      assert_nil(@container.focus_chain)
    end
  end

  class TestTemplate < self
    def test_resource
      only_gtk_version(3, 12, 0)

      Dir.mktmpdir do |dir|
        Dir.chdir(dir) do
          class_name = "MyWindowResource"
          label_value = "My Label"

          gresource_path = "template.gresource"
          gresource_xml_path = "#{gresource_path}.xml"
          template_ui_path = "template.ui"
          File.open(gresource_xml_path, "w") do |gresource_xml|
            gresource_xml.puts(<<-XML)
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
  <gresource prefix="/template">
    <file>#{template_ui_path}</file>
  </gresource>
</gresources>
            XML
          end
          File.open(template_ui_path, "w") do |template_ui|
            template_ui.puts(ui_definition(class_name, label_value))
          end

          assert do
            system("glib-compile-resources", gresource_xml_path)
          end

          resource = Gio::Resource.load(gresource_path)
          Gio::Resources.register(resource)
          begin
            custom_class = Class.new(Gtk::Window) do
              type_register(class_name)

              singleton_class.__send__(:define_method, :init) do
                set_template(:resource => "/template/#{template_ui_path}")
                bind_template_child("label")
              end
            end
            window = custom_class.new
            assert_equal(label_value, window.label.label)
          ensure
            Gio::Resources.unregister(resource)
          end
        end
      end
    end

    def ui_definition(class_name, label)
      <<-DEFINITION
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk+" version="3.12"/>
  <template class="#{class_name}" parent="GtkWindow">
    <property name="can_focus">False</property>
    <child>
      <object class="GtkLabel" id="label">
        <property name="visible">True</property>
        <property name="can_focus">False</property>
        <property name="label" translatable="yes">#{label}</property>
        <property name="ellipsize">end</property>
      </object>
    </child>
  </template>
</interface>
      DEFINITION
    end

    def test_data
      only_gtk_version(3, 12, 0)

      class_name = "MyWindowData"
      label_value = "My Label"
      template_data = ui_definition(class_name, label_value)
      custom_class = Class.new(Gtk::Window) do
        type_register(class_name)

        singleton_class.__send__(:define_method, :init) do
          set_template(:data => template_data)
          bind_template_child("label")
        end
      end
      window = custom_class.new
      assert_equal(label_value, window.label.label)
    end
  end
end
