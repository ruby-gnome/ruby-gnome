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
end

GLib::Log.set_log_domain(Pango::LOG_DOMAIN)

