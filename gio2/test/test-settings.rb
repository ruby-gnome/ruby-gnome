# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestSettings < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  sub_test_case "accessor" do
    setup do
      @settings = Gio::Settings.new("jp.ruby-gnome2.test.settings")
    end

    test "string" do
      @settings.reset("string")
      assert_equal("default-string",
                   @settings["string"])
      @settings["string"] = "new-string"
      assert_equal("new-string",
                   @settings["string"])
    end

    test "boolean" do
      @settings.reset("boolean")
      assert_equal(true,
                   @settings["boolean"])
      @settings["boolean"] = false
      assert_equal(false,
                   @settings["boolean"])
    end
  end

  sub_test_case ".new" do
    test "minimum" do
      settings = Gio::Settings.new("jp.ruby-gnome2.test.settings")
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":path" do
      settings = Gio::Settings.new("jp.ruby-gnome2.test.settings",
                                   :path => "/jp/ruby-gnome2/test/settings/")
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":backend" do
      keyfile = Tempfile.new(["settings", ".ini"])
      backend = Gio::keyfile_settings_backend_new(keyfile.path, "/", "keyfile_settings")
      settings = Gio::Settings.new("jp.ruby-gnome2.test.settings",
                                   :backend => backend)

      settings.reset("string")
      assert_equal("default-string", settings["string"])
      settings["string"] = "new-string"
      assert_equal("new-string", settings["string"])

      keyfile_content_ref = <<-KEYFILE
[jp/ruby-gnome2/test/settings]
string='new-string'
      KEYFILE
      keyfile_content = File.read(keyfile.path)
      assert_equal(keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test ":backend and :path" do
      keyfile = Tempfile.new(["settings", ".ini"])
      backend = Gio::keyfile_settings_backend_new(keyfile.path, "/", "keyfile_settings")
      settings = Gio::Settings.new("jp.ruby-gnome2.test.settings",
                                   :backend => backend,
                                   :path => "/jp/ruby-gnome2/test/settings/")

      settings.reset("string")
      assert_equal("default-string", settings["string"])
      settings["string"] = "new-string"
      assert_equal("new-string", settings["string"])

      keyfile_content_ref = <<-KEYFILE
[jp/ruby-gnome2/test/settings]
string='new-string'
      KEYFILE
      keyfile_content = File.read(keyfile.path)
      assert_equal(keyfile_content_ref, keyfile_content)
    end

    test "full" do
      keyfile = Tempfile.new(["settings", ".ini"])
      backend = Gio::keyfile_settings_backend_new(keyfile.path, "/", "keyfile_settings")
      schema_source = Gio::SettingsSchemaSource.new(fixture_path("schema"), nil, true)
      schema = schema_source.lookup("jp.ruby-gnome2.test.settings", true)
      settings = Gio::Settings.new(schema,
                                   backend,
                                   "/jp/ruby-gnome2/test/settings/")

      settings.reset("string")
      assert_equal("default-string", settings["string"])
      settings["string"] = "new-string"
      assert_equal("new-string", settings["string"])

      keyfile_content_ref = <<-KEYFILE
[jp/ruby-gnome2/test/settings]
string='new-string'
      KEYFILE
      keyfile_content = File.read(keyfile.path)
      assert_equal(keyfile_content_ref, keyfile_content)
    end
  end
end
