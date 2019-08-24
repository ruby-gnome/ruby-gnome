# Copyright (C) 2015-2019  Ruby-GNOME Project Team
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

class TestEnum < Test::Unit::TestCase
  sub_test_case(".try_convert") do
    def test_nil
      assert_nil(GLib::NormalizeMode.try_convert(nil))
    end

    def test_enum
      assert_equal(GLib::NormalizeMode::NFD,
                   GLib::NormalizeMode.try_convert(GLib::NormalizeMode::NFD))
    end

    def test_integer
      assert_equal(GLib::NormalizeMode::NFD,
                   GLib::NormalizeMode.try_convert(GLib::NormalizeMode::NFD.to_i))
    end

    def test_string
      assert_equal(GLib::NormalizeMode::NFD,
                   GLib::NormalizeMode.try_convert("nfd"))
    end

    def test_symbol
      assert_equal(GLib::NormalizeMode::NFD,
                   GLib::NormalizeMode.try_convert(:nfd))
    end

    def test_nonexistent
      assert_nil(GLib::NormalizeMode.try_convert(:nonexistent))
    end

    def test_unconvertable
      assert_nil(GLib::NormalizeMode.try_convert([]))
    end
  end

  def test_enum_by_symbol
    original = [0x00c1].pack("U*") # A with acute

    assert_equal(GLib::UTF8.normalize(original, GLib::NormalizeMode::NFD),
                 GLib::UTF8.normalize(original, :nfd))
    assert_equal(GLib::UTF8.normalize(original, GLib::NormalizeMode::NFD),
                 GLib::UTF8.normalize(original, :NFD))

    assert_raise(TypeError) do
      GLib::UTF8.normalize(original, :unknown)
    end
  end

  def test_enum_by_string
    original = [0x00c1].pack("U*") # A with acute

    assert_equal(GLib::UTF8.normalize(original, GLib::NormalizeMode::NFD),
                 GLib::UTF8.normalize(original, "nfd"))
    assert_equal(GLib::UTF8.normalize(original, GLib::NormalizeMode::NFD),
                 GLib::UTF8.normalize(original, "NFD"))

    assert_raise(TypeError) do
      GLib::UTF8.normalize(original, "unknown")
    end
  end

  def test_flags_simple
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, :keep_comments)
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, :KEEP_COMMENTS)
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, "keep_comments")
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, "KEEP_COMMENTS")
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, "keep COMMENTS")

    assert_raise(ArgumentError) do
      assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, :unknown)
    end

    assert_raise(ArgumentError) do
      assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, "UNKNOWN")
    end
  end

  def test_flags_by_array
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                         GLib::KeyFile::KEEP_TRANSLATIONS,
                         [:keep_comments, :keep_translations])
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                         GLib::KeyFile::KEEP_TRANSLATIONS,
                         [:keep_COMMENTS, "KEEP_TRANSLATIONS"])
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                         GLib::KeyFile::KEEP_TRANSLATIONS,
                         ["keep_comments", "KEEP_translations"])
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                         GLib::KeyFile::KEEP_TRANSLATIONS,
                         [:keep_comments, GLib::KeyFile::KEEP_TRANSLATIONS])
    assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                         GLib::KeyFile::KEEP_TRANSLATIONS,
                         [:keep_comments, nil, :keep_translations])
  end

  def test_flags_or
    assert_equal(GLib::KeyFile::KEEP_COMMENTS,
                 GLib::KeyFile::KEEP_COMMENTS | [])
    assert_equal(GLib::KeyFile::KEEP_COMMENTS |
                 GLib::KeyFile::KEEP_TRANSLATIONS ,
                 GLib::KeyFile::KEEP_COMMENTS | [:keep_translations])
  end

  private
  def assert_key_file_load(flags, convenience_flags)
    data = <<-EOD
[SECTION]
KEY=VALUE
# comment

KEY[ja]=値
EOD

    expected_key_file = GLib::KeyFile.new
    expected_key_file.load_from_data(data, flags)

    actual_key_file = GLib::KeyFile.new
    actual_key_file.load_from_data(data, convenience_flags)

    assert_equal(expected_key_file.get_value("SECTION", "KEY"),
                 actual_key_file.get_value("SECTION", "KEY"))

    assert_equal(expected_key_file.to_data,
                 actual_key_file.to_data)
  end
end
