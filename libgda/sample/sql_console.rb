
# Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
#
# This file is part of Ruby/Libgda.
#
# Ruby/Libgda is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# Ruby/Libgda is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with Ruby/Libgda; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

require 'libgda'
require 'readline'
include Readline

def err(s)
    $stderr.puts s
    exit 1
end

def sql_execute(conn, text)
    cmd = Gda::Command.new(text,
                           Gda::Command::TYPE_SQL,
                           Gda::Command::OPTION_STOP_ON_ERRORS)
    out = ""
    if datamod = conn.execute_single_command(cmd)
        datamod.each_column { |title| out += title + "\t" }
        out += "\n"
        datamod.each_row do |row|
            row.each_value { |val| out += val.to_s + "\t" }
            out += "\n"
        end
    else
        conn.errors.each do |err|
            out += "Error #{err.number.to_s}\n" \
                 + "Description: #{err.description}\n" \
                 + "Source: #{err.source}\n" \
                 + "SQL state: #{err.sqlstate}\n"
        end
    end
    out.strip 
end

def do_stuff(ds)
    client = Gda::Client.new
    client.open_connection(ds.name, ds.username, ds.password) do |conn|
        puts "Connected."
        while line = readline("> ", true) do
            raise Interrupt if line.downcase =~ /^(exit|quit)$/
            next if line.strip.empty?
            puts sql_execute(conn, line) 
        end
    end
end

Gda.init(__FILE__, '0.0.1')

err "Usage: #{__FILE__} [dsn]" unless ARGV.length == 1
datasource = Gda::DataSource.find(ARGV.first)

err "Could not find datasource '#{ARGV.first}'" unless datasource

begin
    Gda.main { do_stuff(datasource) } 
rescue Interrupt
    Gda.main_quit
rescue => e
    err "Exception: #{e.message}"
end

