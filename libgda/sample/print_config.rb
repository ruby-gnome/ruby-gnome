# Print system information about libgda.

require 'libgda'

def puts_dm (dm)
    puts dm.columns.join(', ')
    dm.each_row do |row|
        puts row.values.join(', ')
    end
end

puts "Configured data sources:"
puts_dm Gda::DataSource.model
puts ""
puts "Installed providers:"
puts_dm Gda::Provider.model
