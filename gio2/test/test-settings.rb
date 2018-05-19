# Copyright (C) 2016-2018  Ruby-GNOME2 Project Team
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

  def gen_keyfile_and_backend
    keyfile = Tempfile.new(["settings", ".ini"])
    backend = Gio::keyfile_settings_backend_new(keyfile.path, "/", "keyfile_settings")
    [keyfile, backend]
  end

  def check_settings_with_backend(settings)
    settings.reset("string")
    assert_equal("default-string", settings["string"])
    settings["string"] = "new-string"
    assert_equal("new-string", settings["string"])
  end

  sub_test_case ".new" do
    setup do
      @path = "/jp/ruby-gnome2/test/settings/"
      @schema_id = "jp.ruby-gnome2.test.settings"
      @keyfile_content_ref = <<-KEYFILE
[jp/ruby-gnome2/test/settings]
string='new-string'
      KEYFILE
    end

    def need_keyfile_settings_backend
      unless Gio.respond_to?(:keyfile_settings_backend_new)
        omit("Need Gio.keyfile_settings_backend_new")
      end
    end

    test "minimum" do
      settings = Gio::Settings.new(@schema_id)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":path" do
      settings = Gio::Settings.new(@schema_id,
                                   :path => @path)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":backend" do
      need_keyfile_settings_backend
      keyfile, backend = gen_keyfile_and_backend
      settings = Gio::Settings.new(@schema_id, :backend => backend)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test ":backend and :path" do
      need_keyfile_settings_backend

      keyfile, backend = gen_keyfile_and_backend
      settings = Gio::Settings.new(@schema_id,
                                   :backend => backend, :path => @path)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test "full" do
      need_keyfile_settings_backend
      keyfile, backend = gen_keyfile_and_backend
      schema_dir = fixture_path("schema", "default")
      schema_source = Gio::SettingsSchemaSource.new(schema_dir, nil, true)
      schema = schema_source.lookup(@schema_id, true)
      settings = Gio::Settings.new(schema, backend, @path)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end
  end

  sub_test_case ".new hash arguments" do
    setup do
      @path = "/jp/ruby-gnome2/test/settings/"
      @schema_id = "jp.ruby-gnome2.test.settings"
      @keyfile_content_ref = <<-KEYFILE
[jp/ruby-gnome2/test/settings]
string='new-string'
      KEYFILE
    end

    def need_keyfile_settings_backend
      unless Gio.respond_to?(:keyfile_settings_backend_new)
        omit("Need Gio.keyfile_settings_backend_new")
      end
    end

    test ":schema_id" do
      settings = Gio::Settings.new(:schema_id => @schema_id)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":schema_id and :path" do
      settings = Gio::Settings.new(:schema_id => @schema_id, :path => @path)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":schema_id and :backend" do
      need_keyfile_settings_backend
      keyfile, backend = gen_keyfile_and_backend
      settings = Gio::Settings.new(:schema_id => @schema_id, :backend => backend)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test ":schema_id and :backend and :path" do
      need_keyfile_settings_backend

      keyfile, backend = gen_keyfile_and_backend
      settings = Gio::Settings.new(:schema_id => @schema_id,
                                   :backend => backend,
                                   :path => @path)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test ":schema" do
      schema_dir = fixture_path("schema", "default")
      schema_source = Gio::SettingsSchemaSource.new(schema_dir, nil, true)
      schema = schema_source.lookup(@schema_id, true)
      settings = Gio::Settings.new(:schema => schema)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":schema and :path" do
      schema_dir = fixture_path("schema", "default")
      schema_source = Gio::SettingsSchemaSource.new(schema_dir, nil, true)
      schema = schema_source.lookup(@schema_id, true)
      settings = Gio::Settings.new(:schema => schema,
                                   :path => @path)
      settings.reset("string")
      assert_equal("default-string", settings["string"])
    end

    test ":schema and :backend" do
      need_keyfile_settings_backend
      keyfile, backend = gen_keyfile_and_backend
      schema_dir = fixture_path("schema", "default")
      schema_source = Gio::SettingsSchemaSource.new(schema_dir, nil, true)
      schema = schema_source.lookup(@schema_id, true)
      settings = Gio::Settings.new(:schema => schema,
                                   :backend => backend)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end

    test ":schema and :backend and :path" do
      need_keyfile_settings_backend
      keyfile, backend = gen_keyfile_and_backend
      schema_dir = fixture_path("schema", "default")
      schema_source = Gio::SettingsSchemaSource.new(schema_dir, nil, true)
      schema = schema_source.lookup(@schema_id, true)
      settings = Gio::Settings.new(:schema => schema,
                                   :backend => backend,
                                   :path => @path)

      check_settings_with_backend(settings)

      keyfile_content = File.read(keyfile.path)
      assert_equal(@keyfile_content_ref, keyfile_content)
      FileUtils.rm_f(keyfile)
    end
  end
end
