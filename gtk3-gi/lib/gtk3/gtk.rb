module Gtk
  class << self
    alias_method :check_version_raw, :check_version
    def check_version?(major, minor, micro=nil)
      if check_version_raw(major, minor, micro)
        false
      else
        true
      end
    end
  end
end
