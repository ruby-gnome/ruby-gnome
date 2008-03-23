require "tempfile"
require "date"
require "glib2"
require "gdk_pixbuf2"
begin
  require "gtk2"
rescue LoadError
end
begin
  require "cairo"
rescue LoadError
end
require "poppler.so"

module Poppler
  LOG_DOMAIN = "Poppler"

  VERSION = version.split(".").collect {|x| x.to_i}

  class Document
    private
    def pdf_data?(data)
      /\A%PDF-1\.\d/ =~ data
    end

    def ensure_uri(uri)
      if pdf_data?(uri)
        @pdf = Tempfile.new("ruby-poppler-pdf")
        @pdf.binmode
        @pdf.print(uri)
        @pdf.close
        uri = @pdf.path
      end

      if GLib.path_is_absolute?(uri)
        GLib.filename_to_uri(uri)
      elsif /\A[a-zA-Z][a-zA-Z\d\-+.]*:/.match(uri)
        uri
      else
        GLib.filename_to_uri(File.expand_path(uri))
      end
    end
  end

  if defined?(TextField)
    class TextField
      def multiline?
        type == FormTextType::MULTILINE
      end

      def file_select?
        type == FormTextType::FILE_SELECT
      end

      def normal?
        type == FormTextType::NORMAL
      end
    end
  end

  if defined?(ChoiceField)
    class ChoiceField
      def combo?
        type == FormChioceType::COMBO
      end

      def list?
        type == FormChoiceType::LIST
      end
    end
  end
end

if Poppler.cairo_available?
  module Cairo
    class Context
      def render_poppler_page(page, *args, &block)
        page.render(self, *args, &block)
      end

      def render_poppler_page_selection(page, *args, &block)
        page.render_selection(self, *args, &block)
      end
    end
  end
end

GLib::Log.set_log_domain(Poppler::LOG_DOMAIN)
