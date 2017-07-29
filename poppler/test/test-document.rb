class TestDocument < Test::Unit::TestCase
  def test_save
    saved_pdf = File.join(tmp_dir, "saved.pdf")
    FileUtils.rm_f(saved_pdf)

    document = Poppler::Document.new(form_pdf)
    find_first_text_field(document).text_set_text("XXX")
    assert(document.save('file:///' + saved_pdf))
    assert(File.exist?(saved_pdf))

    reread_document = Poppler::Document.new(saved_pdf)
    assert_equal("XXX", find_first_text_field(reread_document).text_get_text)
  end

  def test_save_a_copy
    copied_pdf = File.join(tmp_dir, "copied.pdf")
    FileUtils.rm_f(copied_pdf)

    document = Poppler::Document.new(form_pdf)
    first_text_field = find_first_text_field(document)
    default_text = first_text_field.text_get_text
    first_text_field.text_set_text("XXX")
    assert(document.save_a_copy('file:///' + copied_pdf))
    assert(File.exist?(copied_pdf))

    reread_document = Poppler::Document.new(copied_pdf)
    assert_equal(default_text, find_first_text_field(reread_document).text_get_text)
  end

  private
  def find_first_text_field(document)
    document.each do |page|
      page.form_field_mapping.each do |mapping|
        field = mapping.field
        return field if field.field_type == Poppler::FormFieldType::TEXT
      end
    end
  end
end
