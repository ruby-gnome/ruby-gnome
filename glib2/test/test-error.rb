# Copyright (C) 2021  Ruby-GNOME Project Team
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

class TestError < Test::Unit::TestCase
  sub_test_case("error class") do
    def test_code
      assert_equal(nil,
                   GLib::BookmarkFileError.new.code)
    end

    def test_domain
      assert_equal("g-bookmark-file-error-quark",
                   GLib::BookmarkFileError.new.domain)
    end
  end

  sub_test_case("error code class") do
    def test_code
      assert_equal(GLib::BookmarkFileError::READ,
                   GLib::BookmarkFileError::Read.new.code)
    end

    def test_domain
      assert_equal("g-bookmark-file-error-quark",
                   GLib::BookmarkFileError::Read.new.domain)
    end
  end
end
