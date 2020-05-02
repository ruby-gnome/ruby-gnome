# Copyright (C) 2014-2020  Ruby-GNOME Project Team
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

class TestHandle < Test::Unit::TestCase
  include Rsvg2TestUtils

  sub_test_case ".new_from_file" do
    sub_test_case "options" do
      def setup
        setup_svg
      end

      def setup_svg
        @svg_path = "tmp/empty.svg"
        return if File.exist?(@svg_path)

        FileUtils.mkdir_p(File.dirname(@svg_path))
        File.open(@svg_path, "w") do |svg|
          svg.puts(<<-SVG)
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg>
</svg>
          SVG
        end
      end

      def test_empty_options
        handle = Rsvg::Handle.new(:file => @svg_path)
        assert_equal([0, 0, 0.0, 0.0],
                     handle.dimensions.to_a)
      end
    end

    sub_test_case ":flags => :unlimited" do
      def setup
        setup_large_svg
      end

      def setup_large_svg
        @large_svg_path = "tmp/large-file.svg"
        return if File.exist?(@large_svg_path)

        FileUtils.mkdir_p(File.dirname(@large_svg_path))
        large_byte = (10 * 1024 * 1024).to_i
        File.open(@large_svg_path, "w") do |svg|
          svg.puts(<<-SVG_HEADER)
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg>
          SVG_HEADER
          large_byte.times do
            svg.puts("\n")
          end
          svg.puts(<<-SVG_FOOTER)
</svg>
          SVG_FOOTER
        end
      end

      def test_unlimited
        only_rsvg_version(2, 40, 3)
        handle = Rsvg::Handle.new(:file => @large_svg_path,
                                  :flags => :flag_unlimited)
        assert_equal([0, 0, 0.0, 0.0],
                     handle.dimensions.to_a)
      end

      def test_base_uri
        handle = Rsvg::Handle.new
        uri_string = GLib.filename_to_uri(File.expand_path("test_base_uri"))
        handle.base_uri = uri_string
        assert_equal(uri_string, handle.base_uri)
      end
    end
  end

  test "#stylesheet=" do
    only_rsvg_version(2, 48)
    svg = <<-SVG
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 200 200">
  <rect id="rectangle" x="0" y="10" width="100" height="100"/>
</svg>
    SVG
    handle = Rsvg::Handle.new(data: svg)
    handle.stylesheet = "rect {fill: red;}"
    # TODO: Assert rendered color
  end
end
