
# Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
#
# This file is part of Ruby/Gda.
#
# Ruby/Gda is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# Ruby/Gda is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with Ruby/Gda; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

require 'test/unit'
require 'gda'

class TC_gda < Test::Unit::TestCase
    TEST_DSN = {
        'name'          => 'arrakis',
        'provider'      => 'MySQL',
        'cnc_string'    => 'HOST=192.168.1.1;DATABASE=pinux',
        'description'   => 'MySQL connection to arrakis for pinux',
        'username'      => 'pinux',
        'password'      => 'pinux@arrakis'
    }
    People = Struct.new("People", :name, :surname)
    def print_errors(conn)
        conn.errors.each do |err|
            puts "Error #{err.number.to_s}\n" \
                 + "Description: #{err.description}\n" \
                 + "Source: #{err.source}\n" \
                 + "SQL state: #{err.sqlstate}"
        end
    end
    def test_everything
        assert_nil Gda.init("Testing Ruby/Gda", "0.1")
        Gda.main do
            # Test Provider.
            assert_instance_of(Array, arr1 = Gda::Provider.providers)
            arr2 = []
            Gda::Provider.each do |provider|
                assert_instance_of(Gda::Provider, provider)
                arr2 << provider
            end
            assert_equal(arr1.size, arr2.size)
            arr1.each do |provider| 
                assert_instance_of(Gda::Provider, provider)
                assert_instance_of(String, provider.prov_id)
                assert_instance_of(String, provider.location)
                assert_instance_of(String, provider.description)
            end
            assert_nil Gda::Provider.get_by_name("does_not_exist")

            # Test DataSource.
            assert_instance_of(Array, arr1 = Gda::DataSource.datasources)
            arr2 = []
            Gda::DataSource.each do |data|
                assert_instance_of(Gda::DataSource, data)
                arr2 << data
            end
            assert_equal(arr1.size, arr2.size)
            arr1.each do |data| 
                assert_instance_of(Gda::DataSource, data)
                assert_instance_of(String, data.name)
                assert_instance_of(String, data.provider)
                assert_instance_of(String, data.cnc_string)
                assert_instance_of(String, data.description)
                
                # username & password may be nil.
                assert_instance_of(String, (data.username or ""))
                assert_instance_of(String, (data.password or ""))

                provider = Gda::Provider.get_by_name(data.provider)
                assert_instance_of(Gda::Provider, provider)

                data2 = Gda::DataSource.find(data.name)
                assert_equal(data, data2)
            end
            assert_nil Gda::DataSource.find("does_not_exist")

            # Test DataSource creation.
            data = Gda::DataSource.new(TEST_DSN["name"], 
                                       TEST_DSN["provider"],
                                       TEST_DSN["cnc_string"],
                                       TEST_DSN["description"],
                                       TEST_DSN["username"],
                                       TEST_DSN["password"])
            assert_equal(TEST_DSN["name"],        data.name)
            assert_equal(TEST_DSN["provider"],    data.provider)
            assert_equal(TEST_DSN["cnc_string"],  data.cnc_string)
            assert_equal(TEST_DSN["description"], data.description)
            assert_equal(TEST_DSN["username"],    data.username)
            assert_equal(TEST_DSN["password"],    data.password)
            assert_nil Gda::DataSource.find(data.name)
            data.save
            data2 = Gda::DataSource.find(data.name)
            assert_equal(data, data2)

            # Test Client and Connection.
            client = Gda::Client.new
            client.open_connection(data.name, data.username, data.password) do |conn|
                assert_instance_of(Gda::Connection, conn)

                assert_equal(data.name, conn.dsn)
                assert_equal(data.username, conn.username)
                assert_equal(data.password, conn.password)
                assert_instance_of(String, conn.cnc_string)
                assert_instance_of(String, (conn.server_version or ""))

                assert conn.opened?
                assert_equal(client, conn.client)
                assert_equal(0, conn.options)

                assert_instance_of(Array, arr1 = client.connections)
                arr2 = []
                client.each_connection do |conn|
                    assert_instance_of(Gda::Connection, conn)
                    arr2 << conn
                end
                [ arr1, arr2 ].each do |arr|
                    assert_equal(1, arr.size)
                    assert_equal(conn, arr.first)
                end

                #conn2 = client.find_connection(conn.dsn, conn.username, conn.password)
                #assert_instance_of(Gda::Connection, conn2)
                #assert_equal(conn, conn2)
                assert_nil client.find_connection("does", "not", "exist")

                # Test SQL commands (non query).
                peoples = [
                    People.new("Hiroshi",   "Igarashi"  ),
                    People.new("Geoff",     "Youngs"    ),
                    People.new("Joshua",    "Keith"     ),
                    People.new("Junichiro", "Kita"      ),
                    People.new("Kato",      "Kazuyoshi" ),
                    People.new("Kubo",      "Takehiro"  ),
                    People.new("Laurent",   "Sansonetti"),
                    People.new("Martin",    "Povolny"   ),
                    People.new("Masahiro",  "Sakai"     ),
                    People.new("Masao",     "Mutoh"     ),
                    People.new("Nikolai",   "Weibull"   ),
                    People.new("Nobuyoshi", "Nakada"    ),
                    People.new("Takeshi",   "Ogasawara" ),
                    People.new("Kenichi",   "Tamura"    ),
                    People.new("Vincent",   "Isambart"  )
                ]
                conn.execute_non_query(create_command("drop table if exists people"))
                conn.execute_non_query(create_command("create table people (name varchar(30), surname varchar(30))"))
                peoples.each do |people|
                    conn.execute_non_query(create_command("insert into people (name, surname) values ('#{people.name}', '#{people.surname}')"))    
                end
                
                # Test SQL queries.
                datamod = conn.execute_single_command(create_command("select name, surname from people"))
                if datamod.nil? 
                    print_errors(conn)
                    flunk
                end
                assert datamod.is_a?(Gda::DataModel)
                
                # Test row fetching (method #1).
                peoples2 = []
                datamod.n_rows.times do |i|
                    row = datamod.get_row(i)
                    assert(Gda::Row, row)
                    assert_equal(datamod, row.model)
                    assert_instance_of(String, row.unique_id) if row.unique_id
                    assert_equal(i, row.number) if row.number != -1
                    assert row.length > 0
                
                    arr = []    
                    datamod.n_columns.times do |i|
                        title = datamod.get_column_title(i)
                        assert_instance_of(String, title)
                        i2 = datamod.get_column_position(title)
                        assert_equal(i, i2)

                        val = row.get_value(i)
                        assert_instance_of(Gda::Value, val)
                        assert_instance_of(String, str = val.to_s)
                        arr << str
                    end
                    peoples2 << People.new(*arr)
                end
                assert_equal(peoples, peoples2)
                
                # Test row fetching (method #2).
                peoples3 = []
                datamod.each_column { |col_title| assert_instance_of(String, col_title) }
                datamod.each_row do |row|
                    arr = []
                    row.each_value do |val|
                        assert_instance_of(Gda::Value, val)
                        assert_instance_of(String, str = val.to_s)
                        arr << str
                    end
                    peoples3 << People.new(*arr)
                end
                assert_equal(peoples, peoples3)

                # Test errors.
                datamod = conn.execute_single_command(create_command("select something from somewhere"))
                assert_nil datamod            # should fail!
                assert_instance_of(Array, arr = conn.errors)
                arr.each do |err|
                    assert_instance_of(Gda::Error, err)
                    assert_instance_of(Fixnum, err.number)
                    assert_instance_of(String, err.source)    
                    assert_instance_of(String, err.description)    
                    assert_instance_of(String, err.sqlstate)    
                end
                assert arr.size > 0    # we should have at least 1 error

                # Gda::Connection#close should be called at 
                # the end of this block.
            end

            # XXX Test Transactions !

            # Test DataSource removing.
            data.remove
            assert_nil Gda::DataSource.find(TEST_DSN["name"])
            
            # Gda.main_quit should be called at the end of this block.
        end
    end

    #######
    private
    #######

    def create_command(sql_cmd)
        cmd = Gda::Command.new(sql_cmd, 
                               Gda::Command::TYPE_SQL, 
                               Gda::Command::OPTION_STOP_ON_ERRORS)
        assert_equal(sql_cmd, cmd.text)
        assert_equal(Gda::Command::TYPE_SQL, cmd.command_type)
        assert_equal(Gda::Command::OPTION_STOP_ON_ERRORS, cmd.options)
        assert_nil cmd.transaction
        cmd
    end
end

