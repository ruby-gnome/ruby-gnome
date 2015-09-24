# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
