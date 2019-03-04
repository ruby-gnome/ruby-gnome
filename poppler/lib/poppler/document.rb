# Copyright (C) 2017-2019  Ruby-GNOME2 Project Team
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

require "tempfile"

module Poppler
  class Document
    include Enumerable

    alias_method :initialize_raw, :initialize
    def initialize(*args)
      if args.size == 1 and args[0].is_a?(Hash)
        options = args[0]
      else
        uri_or_data, password = args
        if pdf_data?(uri_or_data)
          options = {
            :data => uri_or_data,
            :password => password
          }
        else
          options = {
            :uri => ensure_uri(uri_or_data),
            :password => password
          }
        end
      end

      data = options[:data]
      uri = options[:uri]
      path = options[:path]
      stream = options[:stream]
      length = options[:length]
      file = options[:file]
      password = options[:password]

      if data
        # Workaround: poppler_document_new_from_data()'s .gir
        # accepts PDF data as UTF-8 string. PDF data is not UTF-8
        # string. So UTF-8 validation is failed.
        #
        # TODO: Enable the following:
        # initialize_new_from_data(data, password)

        @file = Tempfile.new(["poppler", ".pdf"])
        @file.binmode
        @file.write(data)
        @file.close
        initialize_new_from_file(ensure_uri(@file.path), password)
      elsif uri
        initialize_new_from_file(uri, password)
      elsif path
        uri = ensure_uri(path)
        initialize_new_from_file(uri, password)
      elsif stream
        if length.nil?
          raise(ArgumentError,
                "must specify :length for :stream: #{options.inspect}")
        end
        initialize_new_from_stream(stream, length, password)
      elsif file
        case file
        when String, Pathname
          initialize(path: file, password: password)
        else
          initialize_new_from_gfile(file, password)
        end
      else
        message =
          "must specify one of :data, :uri, :path, :stream or :file: " +
          options.inspect
        raise(ArgumentError, message)
      end
    end

    alias_method :[], :get_page

    def each
      return to_enum(__method__) unless block_given?

      n_pages.times do |i|
        yield get_page(i)
      end
    end

    alias_method :save_raw, :save
    def save(uri)
      save_raw(ensure_uri(uri))
    end

    alias_method :save_a_copy_raw, :save_a_copy
    def save_a_copy(uri)
      save_a_copy_raw(ensure_uri(uri))
    end

    def index_iter
      IndexIter.new(self)
    end

    alias_method :size, :n_pages

    alias_method :pages, :to_a

    private
    def pdf_data?(data)
      data.start_with?("%PDF-1.")
    end

    def ensure_uri(uri)
      uri = uri.to_path if uri.respond_to?(:to_path)
      if GLib.path_is_absolute?(uri)
        GLib.filename_to_uri(uri)
      elsif /\A[a-zA-Z][a-zA-Z\d\-+.]*:/.match(uri)
        uri
      else
        GLib.filename_to_uri(File.expand_path(uri))
      end
    end
  end
end
