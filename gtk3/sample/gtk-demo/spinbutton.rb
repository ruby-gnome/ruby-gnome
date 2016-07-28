# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Spin Button

 GtkSpinButton provides convenient ways to input data
 that can be seen as a value in a range. The examples
 here show that this does not necessarily mean numeric
 values, and it can include custom formatting.
=end
require "date"

module SpinbuttonDemo
  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/spinbutton/spinbutton.ui")
    builder.connect_signals do |name|
      case name
      when "hex_spin_input"
        proc do |button|
          hex_spin_input(button)
        end
      when "hex_spin_output"
        proc do |button|
          hex_spin_output(button)
        end
      when "time_spin_input"
        proc do |button|
          time_spin_input(button)
        end
      when "time_spin_output"
        proc do |button|
          time_spin_output(button)
        end
      when "month_spin_input"
        proc do |button|
          month_spin_input(button)
        end
      when "month_spin_output"
        proc do |button|
          month_spin_output(button)
        end
      else
      end
    end
    window = builder["window"]
    window.screen = main_window.screen
    window.title = "Spin Buttons"
    window.resizable = false

    value_to_label = proc do |value|
      value.to_s
    end

    adj = builder["basic_adjustment"]
    basic_label = builder["basic_label"]
    adj.bind_property("value", basic_label, "label", :sync_create,
                      :transform_to => value_to_label)

    hex_adj = builder["hex_adjustment"]
    hex_label = builder["hex_label"]
    hex_adj.bind_property("value", hex_label, "label", :sync_create,
                      :transform_to => value_to_label)

    adj = builder["time_adjustment"]
    time_label = builder["time_label"]
    adj.bind_property("value", time_label, "label", :sync_create,
                      :transform_to => value_to_label)

    adj = builder["month_adjustment"]
    month_label = builder["month_label"]
    adj.bind_property("value", month_label, "label", :sync_create,
                      :transform_to => value_to_label)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.hex_spin_input(button)
    puts button.class
    puts "hex_spin_input"
    puts "intput --#{button.text} #{button.value} #{button.adjustment.value}--"
    value = button.text.to_i(16)
    value = 0 if value < 1e-5
    button.value = value
  end

  def self.hex_spin_output(button)
    puts "hex_spin_output"
    value = button.value
    # if value.abs < 1e-5
      # button.text = "0x00"
      # puts "0x00"
    # else
    puts "output --#{button.text} #{button.value} #{button.adjustment.value}--"
      button.text = sprintf("0x%.2X", value)
      puts sprintf("0x%.2X", value)
    # end
  end

  def self.time_spin_input(button)
    # puts "time_spin_input"
    str = button.text.split(":")
    found = false
    if str.length == 2
      hours = str[0].to_i
      minutes = str[1].to_i
      if (0..24).include?(hours) &&
         (0..60).include?(minutes)
        return hours * 60 + minutes
      end
    end
    return 0
  end

  def self.time_spin_output(button)
    # puts "time_spin_output"
    hours = button.adjustment.value / 60.0
    minutes = (hours - hours.floor) * 60.0
    button.text = sprintf("%02.0f:%02.0f", hours.floor, (minutes + 0.5).floor)
  end

  def self.month_spin_input(button)
    # puts "month_spin_input"
    # puts button.text
    Date::MONTHNAMES.index(button.text) || 1
  end

  def self.month_spin_output(button)
    # puts "month_spin_output"
    value = button.adjustment.value || 1
    # puts Date::MONTHNAMES[value]
    Date::MONTHNAMES[value]
  end
end
