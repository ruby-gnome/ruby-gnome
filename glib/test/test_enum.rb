# -*- coding: utf-8 -*-
require 'test/unit'
require 'glib2'

class TestEnum < Test::Unit::TestCase
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

    assert_raise(TypeError) do
      assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS, :unknown)
    end

    assert_raise(TypeError) do
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

    assert_raise(TypeError) do
      assert_key_file_load(GLib::KeyFile::KEEP_COMMENTS |
                           GLib::KeyFile::KEEP_TRANSLATIONS,
                           [:keep_comments, nil, :keep_translations])
    end
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
