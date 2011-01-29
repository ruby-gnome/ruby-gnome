# Copyright(C) 2010 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require 'open-uri'
require 'rubygems'
require 'mechanize'

class GNOME2Win32BinaryDownloader
  class << self
    def download(options)
      downloader = new(options)
      packages = options[:packages] || [options[:package]]
      packages.compact.each do |package|
        downloader.download_package(package)
      end
      dependencies = options[:dependencies] || [options[:dependency]]
      dependencies.compact.each do |dependency|
        downloader.download_dependency(dependency)
      end
    end
  end

  URL_BASE = "http://ftp.gnome.org/pub/gnome/binaries/win32"
  def initialize(options={})
    @output_dir = options[:output_dir] || File.join("vendor", "local")
  end

  def download_package(package)
    version_page = agent.get("#{URL_BASE}/#{package}")
    latest_version_link = version_page.links.sort_by do |link|
      if /\A(\d+\.\d+)\/\z/ =~ link.href
        $1.split(/\./).collect {|component| component.to_i}
      else
        [-1]
      end
    end.last

    latest_version_page = latest_version_link.click
    latest_version = latest_version_page.links.collect do |link|
      if /_([\d\.\-]+)_win32\.zip\z/ =~ link.href
        version = $1
        [version.split(/[\.\-]/).collect {|component| component.to_i}, version]
      else
        [[-1], nil]
      end
    end.sort_by do |normalized_version, version|
      normalized_version
    end.last[1]

    latest_version_page.links.each do |link|
      if /_#{Regexp.escape(latest_version)}_win32\.zip\z/ =~ link.href
        click_zip_link(link)
      end
    end
  end

  def download_dependency(dependency)
    dependencies_page = agent.get("#{URL_BASE}/dependencies")
    latest_version = dependencies_page.links.collect do |link|
      if /\A#{Regexp.escape(dependency)}_([\d\.\-]+)_win32\.zip\z/ =~ link.href
        version = $1
        [version.split(/[\.\-]/).collect {|component| component.to_i}, version]
      else
        [[-1], nil]
      end
    end.sort_by do |normalized_version, version|
      normalized_version
    end.last[1]
    dependencies_page.links.each do |link|
      if /\A#{Regexp.escape(dependency)}(?:-dev)?_#{Regexp.escape(latest_version)}_win32.zip/ =~ link.href
        click_zip_link(link)
      end
    end
  end

  private
  def agent
    @agent ||= Mechanize.new
  end

  def click_zip_link(link)
    zip = link.click
    FileUtils.mkdir_p(@output_dir)
    Dir.chdir(@output_dir) do
      open(zip.filename, "wb") do |file|
        file.print(zip.body)
      end
      system("unzip", "-o", zip.filename)
    end
  end
end
