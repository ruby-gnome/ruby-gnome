
require 'libgda'

def list_datasources
    Gda::DataSource.each do |ds|
        puts "NAME: '#{ds.name}', PROVIDER: '#{ds.provider}', CNC: '#{ds.cnc_string}', " \
             "USER: '#{ds.username}', PASSWORD: '#{ds.password}'."
    end
end

def list_providers
    Gda::Provider.each { |provider| puts "ID: '#{provider.prov_id}'." }
end

def save_ds
    ds = Gda::DataSource.new("arrakis", 
                             "MySQL", 
                             "DATABASE=pinux;HOST=192.168.1.1",
                             "MySQL connection to arrakis for user pinux", 
                             "pinux", "pinux@arrakis")
    ds.save
end

def remove_ds
    Gda::DataSource.find("arrakis").remove
end

def show_errors(conn)
    # Obtains errors list and loop for getting error information.
    conn.errors.each do |error|
        puts "Error no: #{error.number.to_s}"
        puts "Description: #{error.description}"
        puts "Source: #{error.source}"
        puts "SQL state: #{error.sqlstate}"
    end
end

def show_table(dm)
    dm.each_column { |title| puts title }
    puts ""
    dm.each_row { |row| row.each_value { |val| puts val.to_s } }
end

def create_command(text)
    Gda::Command.new(text, 
                     Gda::Command::TYPE_SQL, 
                     Gda::Command::OPTION_STOP_ON_ERRORS)
end

def execute_sql_non_query(conn, text)
    command = create_command(text)
    conn.execute_non_query(command)
end

def execute_sql_command(conn, text)
    command = create_command(text)
    array = conn.execute_command(command)
    array.each { |datamodel| show_table(datamodel) }
end

def execute_some_queries(conn)
    execute_sql_non_query(conn, "DELETE FROM customers")
    execute_sql_non_query(conn, "INSERT INTO customers (ref, name, surname, phone) " \
                                "VALUES (1, 'Linus', 'Torvalds', '264-85529-23');" \
                                "INSERT INTO customers (ref, name, surname, phone) " \
                                "VALUES (2, 'Alan', 'Cox', '05-564791235416');")
    execute_sql_non_query(conn, "DELETE FROM accounts;" \
                                "INSERT INTO accounts (ref_customer, balance) " \
                                "VALUES (1, 1000);" \
                                "INSERT INTO accounts (ref_customer, balance) " \
                                "VALUES (2, 5000);")
    execute_sql_command(conn, "SELECT * FROM customers;" \
                              "SELECT * FROM accounts") 
end

def process_accounts(conn)
    # Creates first transaction.
    transaction_one = Gda::Transaction.new("accounts1")
    # Changes the isolation level.
    transaction_one.isolation_level = Gda::Transaction::ISOLATION_SERIALIZABLE
    # Links it to connection.
    conn.begin_transaction(transaction_one)

    command = Gda::Command.new("UPDATE accounts SET balance=balance+50 WHERE ref_customer=1",
                               Gda::Command::TYPE_SQL,
                               Gda::Command::OPTION_STOP_ON_ERRORS)
    # Links command to transaction.
    command.transaction = transaction_one
    conn.execute_non_query(command)

    command = Gda::Command.new("UPDATE accounts SET balance=balance-50 WHERE ref_customer=2",
                               Gda::Command::TYPE_SQL,
                               Gda::Command::OPTION_STOP_ON_ERRORS)
    command.transaction = transaction_one
    conn.execute_non_query(command)

    # Makes commit on transaction.
    conn.commit_transaction(transaction_one)

    transaction_two = Gda::Transaction.new("accounts2")
    transaction_two.isolation_level = Gda::Transaction::ISOLATION_SERIALIZABLE
    conn.begin_transaction(transaction_two)

    command = Gda::Command.new("UPDATE accounts SET balance=balance+400 WHERE ref_customer=1",
                               Gda::Command::TYPE_SQL,
                               Gda::Command::OPTION_STOP_ON_ERRORS)
    command.transaction = transaction_two
    conn.execute_non_query(command)

    command = Gda::Command.new("UPDATE accounts SET balance=balance-400 WHERE ref_customer=2",
                               Gda::Command::TYPE_SQL,
                               Gda::Command::OPTION_STOP_ON_ERRORS)
    command.transaction = transaction_two
    conn.execute_non_query(command)

    # Makes rollback on second transaction.
    conn.rollback_transaction(transaction_two)

    execute_sql_command(conn, "SELECT * FROM accounts")
end

Gda.init("TestGDA", "0.1")
save_ds
Gda.main do
    list_providers
    list_datasources
    client = Gda::Client.new
    client.open_connection("arrakis", nil, nil) do |conn|
        conn.signal_connect('error') { show_errors(conn) }
        execute_some_queries(conn)
        process_accounts(conn)   
    end
    remove_ds
end

