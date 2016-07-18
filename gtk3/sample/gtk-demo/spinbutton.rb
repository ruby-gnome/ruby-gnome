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
      when "time_spin_input"
      when "time_spin_output"
      when "month_spin_input"
      when "month_spin_output"
      else
      end
    end
    window = builder["window"]
    window.screen = main_window.screen
    window.title = "Spin Buttons"
    window.resizable = false

    # adj = GTK_ADJUSTMENT (gtk_builder_get_object (builder, "basic_adjustment"));
    # label = GTK_WIDGET (gtk_builder_get_object (builder, "basic_label"));
    # g_object_bind_property_full (adj, "value",
    #                              label, "label",
    #                              G_BINDING_SYNC_CREATE,
    #                              value_to_label,
    #                              NULL,
    #                              NULL, NULL);
    adj = builder["basic_adjustment"]
    basic_label = builder["basic_label"]

    adj.signal_connect "value-changed" do |widget|
      basic_label.text = widget.value.to_s
    end

    adj = builder["hex_adjustment"]
    hex_label = builder["hex_label"]

    # adj.signal_connect "value-changed" do |widget|
      # hex_label.text = widget.value.to_s
    # end
    adj.bind_property("value", hex_label, "label", :sync_create)
    adj = builder["time_adjustment"]
    time_label = builder["time_label"]

    adj.signal_connect "value-changed" do |widget|
      time_label.text = widget.value.to_s
    end

    adj = builder["month_adjustment"]
    month_label = builder["month_label"]

    adj.signal_connect "value-changed" do |widget|
      month_label.text = widget.value.to_s
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.hex_spin_input(button)
    puts "hex_spin_input"
  end

  def self.hex_spin_output(button)
    puts "hex_spin_output"
  end

  def self.time_spin_input(button)
    puts "time_spin_input"
  end

  def self.time_spin_output(button)
    puts "time_spin_output"
  end

  def self.month_spin_input(button)
    puts "month_spin_input"
  end

  def self.month_spin_output(button)
    puts "month_spin_output"
  end
end
