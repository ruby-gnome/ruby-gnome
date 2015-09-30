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

    signal_connect "startup" do |application|
      puts "startup"
    end

    signal_connect "activate" do |application|
      puts "activate"
    end

    signal_connect "command-line" do |application, command_line|
      puts "cmd"
      options = {}
      OptionParser.new do |opts|
        opts.on("-r", "--run EXAMPLE", "Run an example") do |example|
          options[:name] = example
        end
        opts.on("-a", "--autoquit", "Quit after a delay") do |bool|
          options[:autoquit] = bool
        end
        opts.on("-l", "--list", "List examples") do |bool|
          options[:list] = bool
        end
      end.parse!(command_line.arguments)

      if options[:list]
        puts "list"
        # list_demos
        application.quit
      end

      if options[:name]
        puts "name"
        # lookup_for_corresponding_demo
        # load_demo
      end

      if options[:autoquit]
        puts "autoquit"
        GLib::Timeout.add(1) do 
          #implement auto_quit
        end
      end
      0
    end
  end
end

demo = Demo.new

puts ARGV
exit(demo.run([$PROGRAM_NAME] + ARGV))
