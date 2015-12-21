module Gtk
  class EntryCompletion
    def match_func(&block)
      @match_func = lambda do |completion, key, iter|
        begin
          block.call(completion, key, iter)
        rescue Exception
          $stderr.puts("An exception is raised in " +
                         "#{self.class}\##{__method__} callback: #{block}")
          $stderr.puts("#{$!.class}: #{$!.message}")
          $stderr.puts($@)
          BusSyncReply::DROP
        end
      end
      set_match_func(&@sync_handler)
    end
    private :set_match_func
  end
end
