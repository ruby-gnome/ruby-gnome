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
      compile_gschema("new")
      @settings = Gio::Settings.new("jp.ruby-gnome2.test.value")
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

  def test_new
    compile_gschema("new")
    settings = Gio::Settings.new("jp.ruby-gnome2.test.value")
    settings.reset("string")
    assert_equal("default-string", settings["string"])
  end

  def test_new_with_path
    compile_gschema("with_path")
    settings = Gio::Settings.new("jp.ruby-gnome2.test.value",
                                 :path => "/jp/ruby-gnome2/test/value/")
    settings.reset("string")
    assert_equal("default-string", settings["string"])
  end

  private

  def compile_gschema(test_name)
    base = File.expand_path(File.dirname(__FILE__))
    settings_schema_dir = File.join(base, "fixture", "schema", "settings")
    tests_schema_dir = File.join(settings_schema_dir, test_name)
    Dir.chdir(tests_schema_dir) do
      system("rake") or exit(false)
    end
    ENV["GSETTINGS_SCHEMA_DIR"] = tests_schema_dir
  end
end
