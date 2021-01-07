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

class TestDocument < Test::Unit::TestCase
  include PopplerTestUtils

  sub_test_case("#initialize") do
    sub_test_case(":data") do
      def with_default_internal(encoding)
        original_verbose = $VERBOSE
        original_encoding = Encoding.default_internal
        begin
          $VERBOSE = false
          Encoding.default_internal = encoding
          yield
        ensure
          Encoding.default_internal = original_encoding
          $VERBOSE = original_verbose
        end
      end

      def test_default_internal
        pdf = StringIO.new("".b)
        surface = Cairo::PDFSurface.new(pdf, 100, 100)
        context = Cairo::Context.new(surface)
        context.show_text("Hello")
        surface.finish

        document = with_default_internal(Encoding::UTF_8) do
          Poppler::Document.new(:data => pdf.string)
        end
        assert_equal("Hello", document[0].text)
      end
    end

    sub_test_case(":path") do
      def test_string
        document = Poppler::Document.new(path: multiple_pages_pdf)
        assert_equal("The first page", document[0].text)
      end

      def test_pathname
        document = Poppler::Document.new(path: Pathname(multiple_pages_pdf))
        assert_equal("The first page", document[0].text)
      end
    end
  end

  def test_save
    Dir.mktmpdir do |tmp_dir|
      saved_pdf = File.join(tmp_dir, "saved.pdf")
      document = Poppler::Document.new(text_field_pdf)
      find_first_text_field(document).text = "XXX"
      document.save(saved_pdf)
      reread_document = Poppler::Document.new(saved_pdf)
      assert_equal("XXX", find_first_text_field(reread_document).text)
    end
  end

  def test_save_a_copy
    Dir.mktmpdir do |tmp_dir|
      copied_pdf = File.join(tmp_dir, "copied.pdf")
      document = Poppler::Document.new(text_field_pdf)
      first_text_field = find_first_text_field(document)
      default_text = first_text_field.text
      first_text_field.text = "XXX"
      document.save_a_copy(copied_pdf)
      reread_document = Poppler::Document.new(copied_pdf)
      assert_equal(default_text, find_first_text_field(reread_document).text)
    end
  end

  def test_each
    document = Poppler::Document.new(multiple_pages_pdf)
    texts = []
    document.each do |page|
      texts << page.text
    end
    assert_equal(["The first page", "The second page"],
                 texts)
  end

  def test_each_enumerator
    document = Poppler::Document.new(multiple_pages_pdf)
    assert_equal(["The first page", "The second page"],
                 document.each.collect(&:text))
  end

  def test_enumerable
    document = Poppler::Document.new(multiple_pages_pdf)
    assert_equal(["The first page", "The second page"],
                 document.collect(&:text))
  end

  def test_size
    document = Poppler::Document.new(multiple_pages_pdf)
    assert_equal(2, document.size)
  end

  def test_pages
    document = Poppler::Document.new(multiple_pages_pdf)
    assert_equal(["The first page", "The second page"],
                 document.pages.collect(&:text))
  end

  private
  def find_first_text_field(document)
    document.each do |page|
      page.form_field_mapping.each do |mapping|
        field = mapping.field
        return field if field.is_a?(Poppler::TextField)
      end
    end
  end
end
