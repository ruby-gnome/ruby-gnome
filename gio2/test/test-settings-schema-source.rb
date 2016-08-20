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

class TestSettingsSchemaSource < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  sub_test_case "#list_schemas" do
    setup do
      only_gio_version(2, 42, 0)
      @default_source = Gio::SettingsSchemaSource.default
      @source = Gio::SettingsSchemaSource.new(fixture_path("schema", "source"),
                                              @default_source,
                                              true)
    end

    test "recursive: default" do
      assert_equal(@source.list_schemas(:recursive => true),
                   @source.list_schemas)
    end

    test "recursive: true" do
      non_relocatables_expected, relocatables_expected =
        @default_source.list_schemas
      non_relocatables_expected << "jp.ruby-gnome2.test.source"
      non_relocatables_actual, relocatables_actual =
        @source.list_schemas(:recursive => true)
      assert_equal([
                     non_relocatables_expected.sort,
                     relocatables_expected,
                   ],
                   [
                     non_relocatables_actual.sort,
                     relocatables_actual,
                   ])
    end

    test "recursive: false" do
      assert_equal([["jp.ruby-gnome2.test.source"], []],
                   @source.list_schemas(:recursive => false))
    end
  end
end
