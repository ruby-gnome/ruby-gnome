
# Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

require 'test/unit'
require 'libgda'

class TC_quarklist < Test::Unit::TestCase
    def test_quarklist
        list = Gda::QuarkList.new
        assert_nil list.find('something')
        list.add_from_string('foo=BAR')
	list.add_from_string('hoge=FUGA')
	assert_equal('BAR', list.find('foo'))
	assert_equal('FUGA', list.find('hoge'))
	list.clear
	assert_nil list.find('foo')
	assert_nil list.find('hoge')

	list = Gda::QuarkList.new('foo=BAR')
	assert_equal('BAR', list.find('foo'))
	list.add('hoge=FUGA', true);
	assert_nil list.find('foo')
	assert_equal('FUGA', list.find('hoge'))
    end
end

