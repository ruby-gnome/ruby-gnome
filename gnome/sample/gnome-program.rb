# -*- indent-tabs-mode: nil -*-
# $Id: gnome-program.rb,v 1.2 2003/02/02 12:47:55 tkubo Exp $
#
# gnome-program.rb - sample script of Gnome::Program and Gnome::ModuleInfo
#
# Copyright (c) 2002-2003 Ruby-GNOME2 Project Team
# Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
require 'gnome2'

options = [
	["label", 'l', Gnome::POPT_ARG_STRING, "Hello World", "Set button label", "LABEL"],
	["width", 'w', Gnome::POPT_ARG_INT, 300, "Set window width", "WIDTH"],
	["height", 'h', Gnome::POPT_ARG_INT,200, "Set window height", "HEIGHT"],
	["border", 'b', Gnome::POPT_ARG_NONE, nil, "Remove window border"]
  ]
properties = {
  'popt-table' => options,
}

pgm = Gnome::Program.new('test-gnome-program', '0.1', Gnome::ModuleInfo::LIBGNOMEUI, $0, $*, properties)

puts '-- list module info --'
def collect_modules(minfo)
  ary = [minfo]
  minfo.requirements.each do |minfo, required_version|
    ary.concat(collect_modules(minfo))
  end
  ary
end
modules = collect_modules(Gnome::ModuleInfo::LIBGNOMEUI).uniq.sort
puts modules

puts '-- list options of module info --'
modules.each do |minfo|
  puts minfo.name
  minfo.options.each do |opt|
    print '  '
    arginfo = ''
    case opt[2] & Gnome::POPT_ARG_MASK
    when Gnome::POPT_ARG_NONE
      arginfo = 'POPT_ARG_NONE'
    when Gnome::POPT_ARG_STRING
      arginfo = 'POPT_ARG_STRING'
    when Gnome::POPT_ARG_INT
      arginfo = 'POPT_ARG_INT'
    when Gnome::POPT_ARG_LONG
      arginfo = 'POPT_ARG_LONG'
    when Gnome::POPT_ARG_INCLUDE_TABLE
      arginfo = 'POPT_ARG_INCLUDE_TABLE'
    when Gnome::POPT_ARG_CALLBACK
      arginfo = 'POPT_ARG_CALLBACK'
    when Gnome::POPT_ARG_INTL_DOMAIN
      arginfo = 'POPT_ARG_INTL_DOMAIN'
    when Gnome::POPT_ARG_FLOAT
      arginfo = 'POPT_ARG_FLOAT'
    when Gnome::POPT_ARG_DOUBLE
      arginfo = 'POPT_ARG_DOUBLE'
    end
    arginfo += '|POPT_ARGFLAG_DOC_HIDDEN' if opt[2] & Gnome::POPT_ARGFLAG_DOC_HIDDEN != 0
    opt[2] = arginfo
    p opt
  end
end

puts '--- args ---'
p pgm.args
puts '--- options ---'
p pgm.options
puts '--- print usage ---'
pgm.print_usage()
puts <<EOS
You can't reach here, because Gnome::Program#print_usage and print_help
terminates the program automatically.
EOS
