# -*- coding: utf-8 -*-

require 'glib-mkenums'

class TestGLibMkEnums < Test::Unit::TestCase
  def test_parse_flags
    source = <<-EOS
  G_MARKUP_COLLECT_INVALID,
  G_MARKUP_COLLECT_STRING,
  G_MARKUP_COLLECT_STRDUP,
  G_MARKUP_COLLECT_BOOLEAN,
  G_MARKUP_COLLECT_TRISTATE,

  G_MARKUP_COLLECT_OPTIONAL = (1 << 16)
EOS
    enum = GLib::EnumDefinition.new("GMarkupCollectType", source, 'G_TYPE_')
    assert_equal([["G_MARKUP_COLLECT_INVALID", "invalid"],
                  ["G_MARKUP_COLLECT_STRING", "string"],
                  ["G_MARKUP_COLLECT_STRDUP", "strdup"],
                  ["G_MARKUP_COLLECT_BOOLEAN", "boolean"],
                  ["G_MARKUP_COLLECT_TRISTATE", "tristate"],
                  ["G_MARKUP_COLLECT_OPTIONAL", "optional"]],
                 enum.constants)
  end
end
