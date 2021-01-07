#!/usr/bin/env ruby
#
# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

require "fileutils"

def run_test(test_dir, dependencies)
  $VERBOSE = true

  ruby_gnome_build_dir = ENV["RUBY_GNOME_BUILD_DIR"]
  ruby_gnome_build_dir ||= File.expand_path("..")
  ruby_gnome_source_dir = File.expand_path("../..", test_dir)

  dependencies.each do |dependency|
    source_dir = File.join(ruby_gnome_source_dir, dependency)
    build_dir = File.join(ruby_gnome_build_dir, dependency)
    makefile = File.join(build_dir, "Makefile")
    if File.exist?(makefile)
      system("make", "-C", build_dir, "-j", out: IO::NULL) or exit(false)
      $LOAD_PATH.unshift(File.join(build_dir, "ext", dependency))
    end
    $LOAD_PATH.unshift(File.join(source_dir, "lib"))
  end

  source_fixture_dir = File.join(test_dir, "fixture")
  if File.exist?(source_fixture_dir)
    target_name = File.basename(File.expand_path("..", test_dir))
    build_fixture_dir = File.join(ruby_gnome_build_dir,
                                  target_name,
                                  "test",
                                  "fixture")
    unless source_fixture_dir == build_fixture_dir
      FileUtils.rm_rf(build_fixture_dir)
      FileUtils.mkdir_p(File.dirname(build_fixture_dir))
      FileUtils.cp_r(source_fixture_dir, build_fixture_dir)
    end
    Dir.chdir(build_fixture_dir) do
      if File.exist?("Rakefile")
        system("rake") or exit(false)
      end
    end
  end

  require_relative "glib-test-utils"

  if block_given?
    context = {
      build_fixture_dir: build_fixture_dir,
    }
    yield(context)
  end

  exit(Test::Unit::AutoRunner.run(true, test_dir))
end

if $PROGRAM_NAME == __FILE__
  run_test(__dir__, ["glib2"])
end
