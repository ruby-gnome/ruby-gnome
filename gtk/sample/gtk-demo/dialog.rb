# $Id: dialog.rb,v 1.2 2003/05/04 03:49:57 mutoh Exp $
=begin
= Dialog and Message Boxes

Dialog widgets are used to pop up a transient window for user feedback.
=end
require 'common'

module Demo
  class Dialog < Demo::BasicWindow
    def initialize
      @count = 1
      
      super('Dialogs')
      set_border_width(8)
      
      frame = Gtk::Frame.new('Dialogs')
      add(frame)
      
      vbox = Gtk::VBox.new(false, 8)
      vbox.set_border_width(8)
      frame.add(vbox)
      
      # Standard message dialog
      hbox = Gtk::HBox.new(false, 0)
      vbox.pack_start(hbox, false, false, 0)
      button = Gtk::Button.new('_Message Dialog', true)
      button.signal_connect('clicked') do
	message_dialog_clicked
      end
      hbox.pack_start(button, false, false, 0)

      vbox.pack_start(Gtk::HSeparator.new, false, false, 0)

      # Interactive dialog
      hbox = Gtk::HBox.new(false, 8)
      vbox.pack_start(hbox, false, false, 0)
      vbox2 = Gtk::VBox.new(false, 0)

      button = Gtk::Button.new('_Interactive Dialog')
      button.signal_connect('clicked') do
	interactive_dialog_clicked
      end
      hbox.pack_start(vbox2, false, false, 0)
      vbox2.pack_start(button, false, false, 0)

      table = Gtk::Table.new(2, 2, false)
      table.set_row_spacings(4)
      table.set_column_spacings(4)
      hbox.pack_start(table, false, false, 0)

      label = Gtk::Label.new('_Entry 1', true)
      table.attach_defaults(label, 0, 1, 0, 1)

      @entry1 = Gtk::Entry.new
      table.attach_defaults(@entry1, 1, 2, 0, 1)
      label.set_mnemonic_widget(@entry1)

      label = Gtk::Label.new('E_ntry 2', true)

      table.attach_defaults(label, 0, 1, 1, 2)

      @entry2 = Gtk::Entry.new
      table.attach_defaults(@entry2, 1, 2, 1, 2)
      label.set_mnemonic_widget(@entry2)
    end

    def message_dialog_clicked
      dialog = Gtk::MessageDialog.new(self,
				      Gtk::Dialog::MODAL |
				      Gtk::Dialog::DESTROY_WITH_PARENT,
				      Gtk::MessageDialog::INFO,
				      Gtk::MessageDialog::BUTTONS_OK,
				      <<EOS)
This message box has been popped up the following
number of times:

#{@count}
EOS
      dialog.run
      dialog.destroy
      @count += 1
    end

    def interactive_dialog_clicked
      dialog = Gtk::Dialog.new('Interactive Dialog',
			       self,
			       Gtk::Dialog::MODAL |
			       Gtk::Dialog::DESTROY_WITH_PARENT,
			       [
				 Gtk::Stock::OK,
				 Gtk::Dialog::RESPONSE_OK,
				 "_Non-stock Button",
				 Gtk::Dialog::RESPONSE_CANCEL,
			       ])

      hbox = Gtk::HBox.new(false, 0)
      hbox.set_border_width(8)
      dialog.vbox.pack_start(hbox, false, false, 0)

      stock = Gtk::Image.new(Gtk::Stock::DIALOG_QUESTION, Gtk::IconSize::DIALOG)
      hbox.pack_start(stock, false, false, 0)

      table = Gtk::Table.new(2, 2, false)
      table.set_row_spacings(4)
      table.set_column_spacings(4)
      hbox.pack_start(table, true, true, 0)
      label = Gtk::Label.new('_Entry 1', true)
      table.attach_defaults(label,
			    0, 1, 0, 1)
      local_entry1 = Gtk::Entry.new
      local_entry1.text = @entry1.text
      table.attach_defaults(local_entry1, 1, 2, 0, 1)
      label.set_mnemonic_widget(local_entry1)

      label = Gtk::Label.new('E_ntry 2', true)
      table.attach_defaults(label,
			    0, 1, 1, 2)

      local_entry2 = Gtk::Entry.new
      local_entry2.text = @entry2.text
      table.attach_defaults(local_entry2, 1, 2, 1, 2)
      label.set_mnemonic_widget(local_entry2)

      hbox.show_all
      response = dialog.run
      
      if response == Gtk::Dialog::RESPONSE_OK 
	@entry1.text = local_entry1.text
	@entry2.text = local_entry2.text
      end
      dialog.destroy
    end
  end
end

