# Copyright (C) 2008-2021  Ruby-GNOME Project Team
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

require "open-uri"
require "fileutils"

require "poppler"

module PopplerTestUtils
  def ensure_dir(dir)
    FileUtils.mkdir_p(dir)
    dir
  end

  def test_dir
    File.expand_path(File.dirname(__FILE__))
  end

  def fixtures_dir
    ensure_dir(File.join(test_dir, "fixtures"))
  end

  def fixture_path(*components)
    File.join(fixtures_dir, *components)
  end

  def form_pdf
    fixture_path("form.pdf")
  end

  def text_field_pdf
    fixture_path("text-field.pdf")
  end

  def image_pdf
    fixture_path("image.pdf")
  end

  def image_png
    fixture_path("image.png")
  end

  def outline_pdf
    fixture_path("outline.pdf")
  end

  def multiple_pages_pdf
    fixture_path("multiple-pages.pdf")
  end

  def only_poppler_version(major, minor, micro=nil)
    micro ||= 0
    unless Poppler::Version.or_later?(major, minor, micro)
      omit("Require Poppler >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_old_poppler_version(major, minor, micro=nil)
    micro ||= 0
    if Poppler::Version.or_later?(major, minor, micro)
      omit("Require Poppler < #{major}.#{minor}.#{micro}")
    end
  end
end
