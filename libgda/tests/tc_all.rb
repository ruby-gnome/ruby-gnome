
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

class TC_everything < Test::Unit::TestCase
    def setup
        Gda.init(__FILE__, "0.0.0")
    end

    def test_datasource
        assert Gda::DataSource.model.is_a?(Gda::DataModel)
        assert_nil(Gda::DataSource.find("does_not_exist"))        

        arr = []
        Gda::DataSource.each do |ds|
            assert_instance_of(Gda::DataSource, ds)

            ds2 = Gda::DataSource.find(ds.name)
            assert_instance_of(Gda::DataSource, ds2)
            assert_equal(ds, ds2)
           
            assert_instance_of(String, ds.name)
            assert_instance_of(String, ds.provider)
            assert_instance_of(String, ds.cnc_string)
            assert_instance_of(String, ds.description)
            assert_instance_of(String, ds.username) unless ds.username.nil?
            assert_instance_of(String, ds.password) unless ds.password.nil?

            arr << ds
        end    

        arr2 = Gda::DataSource.datasources
        assert_instance_of(Array, arr2)
        assert_equal(arr, arr2)

        ds = Gda::DataSource.new('my_ds',
                                 'XML',
                                 'URI=/tmp/foo',
                                 'Sample DS',
                                 'someone',
                                 nil)
        ds.save
        ds2 = Gda::DataSource.find('my_ds')
        assert_equal(ds, ds2)
        ds.remove
        assert_nil(Gda::DataSource.find('my_ds'))
    end

    def test_provider
        assert Gda::Provider.model.is_a?(Gda::DataModel)
        assert_nil(Gda::Provider.get_by_name("does_not_exist"))

        arr = []
        Gda::Provider.each do |prov|
            assert_instance_of(Gda::Provider, prov)

            prov2 = Gda::Provider.get_by_name(prov.name)
            assert_instance_of(Gda::Provider, prov2)
            assert_equal(prov, prov2)

            assert_instance_of(String, prov.name)
            assert_equal(prov.name, prov.prov_id)
            assert_instance_of(String, prov.location)
            assert_instance_of(String, prov.description)

            arr << prov
        end 

        arr2 = Gda::Provider.providers
        assert_instance_of(Array, arr2)
        assert_equal(arr, arr2)
    end

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

    def test_error
        err = Gda::Error.new('description', 123, 'source', 'sqlstate')
        assert_equal('description', err.description)
        assert_equal(123, err.number)
        assert_equal('source', err.source)
        assert_equal('sqlstate', err.sqlstate)

        err = Gda::Error.new
        err.description = 'description'
        err.number = 123
        err.source = 'source'
        err.sqlstate = 'sqlstate'
        assert_equal('description', err.description)
        assert_equal(123, err.number)
        assert_equal('source', err.source)
        assert_equal('sqlstate', err.sqlstate)
    end

    def test_parameters
        list = Gda::ParameterList.new(Gda::Parameter.new('nil', nil),
                                      Gda::Parameter.new('number',  123))
        list << Gda::Parameter.new('string',  'hello world')
        list << Gda::Parameter.new('boolean', true)

        assert !list.empty?
        assert_equal(4, list.length)
        arr = list.parameters
        assert_instance_of(Array, arr)
        assert_equal(4, arr.length)

        i = 0
        list.each_parameter do |param|
            assert_instance_of(Gda::Parameter, param)
            assert_instance_of(String, param.name)
            assert_instance_of(Gda::Value, param.value)
            assert_instance_of(Gda::Parameter, list.find(param.name))
            case param.name
                when 'nil'
                    assert_equal('NULL', param.value.to_s)
                when 'string'
                    assert_equal('hello world', param.value.to_s)
                when 'number'
                    assert_equal('123.00', param.value.to_s)
                when 'boolean'
                    assert_equal('TRUE', param.value.to_s)
                else
                    assert false
            end
            i += 1
        end
        assert_equal(4, i)
       
        arr = list.names
        assert_instance_of(Array, arr)
        assert_equal(4, arr.length)
        
        i = 0
        list.each_name do |name|
            assert_instance_of(String, name)
            param = list[name]
            assert_instance_of(Gda::Parameter, param)
            i += 1
        end
        assert_equal(4, i)

        assert_nil list.find("does_not_exist")
        list.clear
        assert_nil list.find('nil')
        assert_nil list.find('string')
        assert_nil list.find('number')
        assert_nil list.find('boolean')
        assert list.empty?
        assert_equal(0, list.length)
    end
end

