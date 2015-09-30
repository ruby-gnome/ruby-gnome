#!/usr/bin/env ruby
#
# Copyright (C) 2006-2015  Ruby-GNOME2 Project Team
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

require "gtk3"
require "optparse"

class Demo < Gtk::Application
  def initialize
    super("org.gtk.Demo", [:non_unique, :handles_command_line])

    @options = {}

    signal_connect "startup" do |application|
      puts "startup"
    end

    signal_connect "activate" do |application|
      puts "activate"
    end

    signal_connect "command-line" do |application, command_line|
      puts "cmd"

      begin
        parse_command_line(command_line.arguments)
      rescue SystemExit => error
        error.status
      rescue OptionParser::InvalidOption => error
        puts error.message
        1
      rescue => error
        puts "#{error.class}: #{error.message}"
        puts error.backtrace
        1
      else
        run_application
      end
    end
  end

  private
  def parse_command_line(arguments)
    parser = OptionParser.new
    parser.on("-r", "--run EXAMPLE", "Run an example") do |example|
      @options[:name] = example
    end
    parser.on("-a", "--autoquit", "Quit after a delay") do
      @options[:autoquit] = true
    end
    parser.on("-l", "--list", "List examples") do
      @options[:list] = true
    end
    parser.parse(arguments)
  end

  def run_application
    if @options[:list]
      puts "list"
      # list_demos
      quit
    end

    if @options[:name]
      puts "name"
      # lookup_for_corresponding_demo
      # load_demo
    end

    if @options[:autoquit]
      puts "autoquit"
      GLib::Timeout.add(1) do
        #implement auto_quit
      end
    end

    0
  end
end

demo = Demo.new

puts ARGV
exit(demo.run([$PROGRAM_NAME] + ARGV))
