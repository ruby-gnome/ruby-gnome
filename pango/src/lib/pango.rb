require 'glib2'
require 'pango.so'


module Pango
  LOG_DOMAIN = "Pango"
  class AttrList
    def each(text = nil)
      iter = iterator
      begin
        if text
          s, e = iter.range
          yield(iter, text[s, e - s], s, e)
        else
          yield(iter)
        end
     end while iter.next!
    end
  end

  constants.each do |name|
    const = const_get(name)
    if const.is_a?(Module)
      case name
      when /^Attr([A-Z].*)/
        next
      when "Attribute", "Coverage"
        prefix = "#{name.upcase}_"
      else
        prefix = ""
      end
      
      const.constants.each do |sub_name|
        if sub_name == sub_name.upcase and sub_name != "LOG_DOMAIN"
          if /^TYPE_(.*)/ =~ sub_name
            new_name = "#{prefix}#{$1}"
          else
            new_name = "#{prefix}#{sub_name}"
          end
          const_set(new_name, const.const_get(sub_name))
        end
      end
    end
  end
end

GLib::Log.set_log_domain(Pango::LOG_DOMAIN)
