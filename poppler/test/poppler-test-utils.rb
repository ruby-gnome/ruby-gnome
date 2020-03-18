require "open-uri"
require "fileutils"

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
