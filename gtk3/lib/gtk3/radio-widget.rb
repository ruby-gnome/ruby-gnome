module Gtk
  module RadioWidget
    :private
    def initialize_method(options={})
      group         = options[:group]
      label         = options[:label]
      member        = options[:member]
      use_underline = options[:use_underline]

      if label
        if member
          if use_underline
            initialize_new_with_mnemonic_from_widget(member, label)
          else
            initialize_new_with_label_from_widget(member, label)
          end
        else
          if use_underline
            initialize_new_with_mnemonic(group, label)
          else
            initialize_new_with_label(group, label)
          end
        end
      else
        if member
          initialize_new_from_widget(member)
        else
          initialize_raw
        end
      end
    end
  end
end
