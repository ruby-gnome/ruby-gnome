# -*- ruby -*-
#
# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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
require "pathname"

require "rake"

module GNOME2
  module Rake
    class SourceDownloadTask
      include ::Rake::DSL

      def initialize(package)
        @package = package
      end

      def define
        namespace :source do
          namespace :downloader do
            task :before
            define_download_tasks
            download_tasks = @package.external_packages.collect do |package|
              "source:downloader:download:#{package.name}"
            end
            task :download => download_tasks
            task :after
          end

          desc "Dowanload sources"
          task :download => [
            "source:downloader:before",
            "source:downloader:download",
            "source:downloader:after",
          ]
        end
      end

      private
      def define_download_tasks
        namespace :download do
          @package.external_packages.each do |package|
            download_dir = @package.download_dir
            tar_full_path = download_dir + package.archive_base_name

            task :before
            task :after
            desc "Download #{package.label} into #{download_dir}."
            # task package[:name] => [:before, tar_full_path.to_s, :after]
            task package[:name] => tar_full_path.to_s

            directory_path = tar_full_path.dirname
            directory directory_path.to_s
            file tar_full_path.to_s => directory_path.to_s do
              archive_url = package.archive_url
              rake_output_message "Downloading... #{archive_url}"
              download(archive_url, tar_full_path)
            end
          end
        end
      end

      def download(url, output_path)
        OpenURI.module_eval do
          alias_method :redirectable_original?, :redirectable?
          def redirectable?(uri1, uri2)
            redirectable_original?(uri1, uri2) or
              (uri1.scheme.downcase == "http" and
               uri2.scheme.downcase == "https")
          end
        end
        begin
          open(url) do |input|
            output_path.open("wb") do |output_file|
              output_file.print(input.read)
            end
          end
        ensure
          OpenURI.module_eval do
            alias_method :redirectable?, :redirectable_original?
            remove_method :redirectable_original?
          end
        end
      end
    end
  end
end
