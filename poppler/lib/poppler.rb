# Copyright(C) 2006-2009 Ruby-GNOME2 Project.
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License.
#
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

require "tempfile"
require "date"
require "glib2"
require "gdk_pixbuf2"
begin
  require "gtk2"
rescue LoadError
rescue
  if defined?(Gtk::InitError) and $!.class == Gtk::InitError
    # ignore
  else
    raise
  end
end
begin
  require "cairo"
rescue LoadError
end

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, micro, = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/poppler.so"
rescue LoadError
  require "poppler.so"
end

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
