class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "empty" do
      image = Gtk::Image.new
      assert_equal(nil, image.pixbuf)
    end

    test "stock" do
      image = Gtk::Image.new(:stock => Gtk::Stock::GO_FORWARD,
                             :size => :dialog)
      assert_equal([
                     Gtk::Stock::GO_FORWARD.to_s,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.stock)
    end

    test "icon_name" do
      icon_name = "dialog-password"
      image = Gtk::Image.new(:icon_name => icon_name,
                             :size => Gtk::IconSize::DIALOG)
      assert_equal([
                     icon_name,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.icon_name)
    end

    test "icon" do
      icon = Gio::ThemedIcon.new("document-open")
      image = Gtk::Image.new(:icon => icon,
                             :size => :dialog)
      assert_equal([
                     icon,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.icon)
    end

    test "resource" do
      only_gtk_version(3, 18, 0)
      resource = Gio::Resource.load(fixture_path("image.gresource"))
      Gio::Resources.register(resource)
      begin
        resource_path = "/org/ruby/gnome/gnome-logo-icon.png"
        image = Gtk::Image.new(:resource => resource_path,
                               :size => :dialog)
        assert_equal(resource_path,
                     image.resource)
      ensure
        Gio::Resources.unregister(resource)
      end
    end
  end
end
