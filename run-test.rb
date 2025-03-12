#!/usr/bin/env ruby
#
# Copyright (C) 2008-2025  Ruby-GNOME Project Team
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

require "rbconfig"
require "pathname"
require "time"

require_relative "helper"

ruby = RbConfig.ruby

# for creating a separating line
def separator
  "-" * 80
end

def run_broadwayd(target)
  unless ENV["RUBY_GNOME_GTK4_USE_BROADWAY"] == "yes"
    return yield
  end

  case target.basename.to_s
  when "gdk4", "gtk4"
    pid = spawn("gtk4-broadwayd")
    begin
      gdk_backend, ENV["GDK_BACKEND"] = ENV["GDK_BACKEND"], "broadway"
      yield
    ensure
      ENV["GDK_BACKEND"] = gdk_backend
      Process.kill(:TERM, pid)
      Process.waitpid(pid)
    end
  else
    yield
  end
end

targets = []
includes = []

base_dir = Pathname(__dir__)
if ARGV.empty?
  candidates = Pathname.glob((base_dir + "*").to_s)
else
  candidates = ARGV.collect do |package|
    base_dir + package
  end
end
candidates.each do |dir|
  next unless dir.directory?
  source_dir = dir.expand_path
  next unless (source_dir + "test").directory?
  targets << dir
end

all_test_packages = Helper.all_test_packages

failed_target_names = []
targets.each do |target|
  next unless all_test_packages.include?(target.basename.to_s)

  puts "::group::Test #{target}"
  puts "#{Time.now.iso8601}: Running test for #{target}"
  puts separator

  run_broadwayd(target) do
    run_test = target + "test/run-test.rb"
    unless system(ruby, *includes, run_test.to_s)
      puts "Failed to run test: #{target.basename}"
      failed_target_names << target.basename.to_s
    end
  end

  puts separator
  puts "::endgroup::"
end

if failed_target_names.empty?
  exit(true)
else
  puts "Failed targets: #{failed_target_names.join(', ')}"
  exit(false)
end
