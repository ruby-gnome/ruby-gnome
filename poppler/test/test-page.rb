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

class TestPage < Test::Unit::TestCase
  include PopplerTestUtils

  def test_get_image
    document = Poppler::Document.new(image_pdf)
    page, mapping = find_first_image_mapping(document)
    assert_kind_of(Cairo::ImageSurface, page.get_image(mapping.image_id))
    assert_kind_of(Cairo::ImageSurface, mapping.image)
  end

  def test_selection_region
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    rectangle = Poppler::Rectangle.new(0, 0, *page.size)
    region = page.get_selection_region(0.5, :word, rectangle)
    assert_kind_of(Poppler::Rectangle, region[0])
  end

  def test_annot_mapping
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    assert_equal([Poppler::AnnotMapping],
                 page.annot_mapping.collect {|mapping| mapping.class}.uniq)
    mapping = page.annot_mapping[0]
    assert_kind_of(Poppler::Rectangle, mapping.area)
    assert_kind_of(Poppler::Annot, mapping.annot)
  end

  def test_text_layout
    only_poppler_version(0, 16, 0)
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    layout = page.text_layout
    assert_equal([75, 67, 81, 80],
                 layout[0].to_a.collect(&:round))
  end

  sub_test_case("#find_text") do
    def normalize_rectangles(rectangles)
      rectangles.collect do |rectangle|
        rectangle.to_a.collect(&:round)
      end
    end

    test "no options" do
      document = Poppler::Document.new(multiple_pages_pdf)
      page = document[0]
      text_areas = page.find_text("firs")
      assert_equal([[78, 771, 94, 785]],
                   normalize_rectangles(text_areas))
    end

    test "with default option" do
      document = Poppler::Document.new(multiple_pages_pdf)
      page = document[0]
      text_areas = page.find_text("firs", :default)
      assert_equal([[78, 771, 94, 785]],
                   normalize_rectangles(text_areas))
    end

    test "with options and bad text" do
      document = Poppler::Document.new(multiple_pages_pdf)
      page = document[0]
      text_areas = page.find_text("fIrs", [:whole_words_only, :case_sensitive])
      assert_equal([],
                   normalize_rectangles(text_areas))
    end

    test "with options and good text" do
      document = Poppler::Document.new(multiple_pages_pdf)
      page = document[0]
      text_areas = page.find_text("first", [:whole_words_only, :case_sensitive])
      assert_equal([[78, 771, 98, 785]],
                   normalize_rectangles(text_areas))
    end
  end

  sub_test_case("#thumbnail_size") do
    def create_document
      pdf = StringIO.new
      surface = Cairo::PDFSurface.new(pdf, 10, 20)
      yield(surface) if block_given?
      surface.finish
      Poppler::Document.new(pdf.string)
    end

    def only_cairo_version(major, minor, micro=nil)
      unless Cairo.satisfied_version?(major, minor, micro)
        omit("Require cairo >= #{major}.#{minor}.#{micro}")
      end
    end

    def test_exist
      only_cairo_version(1, 15, 4)
      document = create_document do |surface|
        surface.set_thumbnail_size(5, 10)
      end
      page = document[0]
      assert_equal([5, 10], page.thumbnail_size)
    end

    def test_not_exist
      document = create_document
      page = document[0]
      assert_nil(page.thumbnail_size)
    end
  end

  private
  def find_first_image_mapping(document)
    document.each do |page|
      page.image_mapping.each do |mapping|
        return [page, mapping]
      end
    end
    nil
  end
end
