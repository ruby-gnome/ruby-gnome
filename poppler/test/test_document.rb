class TestDocument < Test::Unit::TestCase
  def test_save
    saved_pdf = File.join(tmp_dir, "saved.pdf")
    FileUtils.rm_f(saved_pdf)

    document = Poppler::Document.new(form_pdf)
    find_first_text_field(document).text = "XXX"
    assert(document.save(saved_pdf))
    assert(File.exist?(saved_pdf))

    reread_document = Poppler::Document.new(saved_pdf)
    assert_equal("XXX", find_first_text_field(reread_document).text)
  end

  def test_save_a_copy
    return unless later_version?(0, 7, 2)
    copied_pdf = File.join(tmp_dir, "copied.pdf")
    FileUtils.rm_f(copied_pdf)

    document = Poppler::Document.new(form_pdf)
    first_text_field = find_first_text_field(document)
    default_text = first_text_field.text
    first_text_field.text = "XXX"
    assert(document.save_a_copy(copied_pdf))
    assert(File.exist?(copied_pdf))

    reread_document = Poppler::Document.new(copied_pdf)
    assert_equal(default_text, find_first_text_field(reread_document).text)
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
