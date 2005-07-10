require 'gtk2'

path = ARGV[0] || __FILE__

=begin
GLib::IOChannel.open(path) {|io|
  puts io.read
}
=end

stdout = GLib::IOChannel.new(path, "r")
stdout.add_watch(GLib::IOChannel::IN
		 ) {|io, condition|
  puts "condition = #{condition}"
  false
  true
}

context = GLib::MainContext.default
mainloop = GLib::MainLoop.new(context, true)


Thread.new{
  num = 0
  loop {
    num += 1
    str = stdout.gets
    puts "line #{num}: #{str}"
    unless str
      mainloop.quit
      break
    end
  }
}

mainloop.run


stdout.close
