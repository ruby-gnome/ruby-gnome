# Copyright (C) 2014-2019  Ruby-GNOME2 Project Team
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

class TestFile < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  sub_test_case(".open") do
    sub_test_case(":path") do
      def test_string
        path = __FILE__
        Gio::File.open(path: path) do |file|
          file.read do |input|
            assert_equal(File.read(__FILE__), input.read)
          end
        end
      end

      def test_pathname
        path = Pathname(__FILE__)
        Gio::File.open(path: path) do |file|
          file.read do |input|
            assert_equal(File.read(__FILE__), input.read)
          end
        end
      end
    end
  end

  sub_test_case("instance methods") do
    def setup
      @file = Gio::File.open(path: __FILE__)
    end

    def test_guess_content_type
      omit_on_os_x
      path = fixture_path("content-type", "x-content", "unix-software")
      dir = Gio::File.open(path: path)
      assert_equal(["x-content/unix-software"],
                   dir.guess_content_types.collect(&:to_s))
    end

    sub_test_case("#read") do
      def test_no_block
        input_stream = @file.read
        assert_equal(File.read(__FILE__), input_stream.read)
      end

      def test_with_block
        content = @file.read do |input_stream|
          input_stream.read
        end
        assert_equal(File.read(__FILE__), content)
      end
    end
  end
end
