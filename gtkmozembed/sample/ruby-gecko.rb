#!/usr/bin/env ruby

=begin

  The Ruby Gecko (featuring Gtk::* and Gtk::MozEmbed)

  Copyright (C) 2005 Mirko Maischberger, All rights reserverd

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

=end

## The following is commented since the comp_path gets a nice 
## default at compile time.
##
#unless ENV['MOZILLA_FIVE_HOME'] 
#  $stderr.print "Please set MOZILLA_FIVE_HOME environment variable to\n"
#  $stderr.print "something like /usr/lib/mozilla\n"
#end

require 'gtkmozembed'

class Browser < Gtk::Window

  TITLE = "The Ruby Gecko"

  attr_reader :moz

  def initialize

    super

    self.border_width = 1
    self.title = TITLE
    self.resize(800, 570)

    # This one is needed if GtkMozEmbed can't find the comp_path
    mozhome = ENV['MOZILLA_FIVE_HOME']
    if mozhome
      Gtk::MozEmbed.set_comp_path(mozhome)
    end

    # Gtk::MozEmbed :)
    Gtk::MozEmbed.set_profile_path(ENV['HOME'] + '.mozilla', 'RubyGecko')

    # Let's create the MozEmbed widget.
    @moz = Gtk::MozEmbed.new

    @moz.chrome_mask = Gtk::MozEmbed::ALLCHROME

    # A Toolbar with some stock icons and self explaining beaviour
    bar = Gtk::Toolbar.new

    back = bar.append(Gtk::Stock::GO_BACK) {  
      @moz.go_back
    }

    forward = bar.append(Gtk::Stock::GO_FORWARD){  
      @moz.go_forward
    }

    stop = bar.append(Gtk::Stock::STOP){  
      @moz.stop_load
    }

    refresh = bar.append(Gtk::Stock::REFRESH){  
      @moz.reload(Gtk::MozEmbed::RELOADBYPASSCACHE)
    }

    home = bar.append(Gtk::Stock::HOME) { 
      @moz.location="http://ruby-gnome2.sourceforge.jp/" 
    }
    
    home = bar.append(Gtk::Stock::ADD) { 
      @moz.open_stream("http://ruby-gnome2.sourceforge.jp/", "text/html")
      @moz.append_data("<html>\n")
      @moz.append_data("<head>\n")
      @moz.append_data("<title>Self feeded test page</title>\n")
      @moz.append_data("</head>\n")
      @moz.append_data("<body>\n")
      @moz.append_data("<h1>Method: open_stream</h1>\n")
      @moz.append_data("<a target=\"_blank\" href=\"hiki.cgi?Status+of+Ruby%2FGtkMozEmbed\">")
      @moz.append_data("Status of Gtk::MozEmbed</a>")
      @moz.append_data("(link relative to base_uri)\n")
      @moz.append_data("</body>")
      @moz.append_data("</html>")
      @moz.close_stream

      # see alsa: Gtk::MozEmbed#render_data
    }
    
    # A text-entry to let the user change the location manually
    entry = Gtk::Entry.new

    # A Statusbar with link info and progress bar
    statusbox = Gtk::HBox.new

    status = Gtk::Statusbar.new
    status.has_resize_grip=false
    status_context = status.get_context_id("Link")

    progress = Gtk::ProgressBar.new
    
    statusbox.pack_start(status, true)
    statusbox.pack_start(progress, false)

    # Pack together
    box= Gtk::VBox.new
    box.pack_start(bar, false)
    box.pack_start(entry, false)
    box.pack_start(@moz)
    box.pack_start(statusbox, false)

    # Add to the main window
    self.add(box)

    # Connect some more signals

    # When we press enter while the text-entry has 
    # the focus this tells mozilla to load the inserted url
    entry.signal_connect('activate') {
      @moz.location = entry.text 
    }

    # When location changes (the user clicks on a link)
    # this updates the text-entry value
    @moz.signal_connect('location') { |widget|
      $stderr.print "location signal\n"
      entry.text = widget.location if entry.text != widget.location
    }

    # When the page title changes we update the window
    # title as well
    @moz.signal_connect('title') { |widget|
      $stderr.print "title signal\n"
      self.title = widget.title + ' - ' + TITLE
    }

    # Lots of this signals arive during the page loading
    # to let us update the progress bar. It the web server
    # tells mozilla the total page size we can update the
    # progress bar with a percentage value, otherwise max
    # will be -1 and we just pulse.
    @moz.signal_connect('progress') { |widget, cur, max|
      $stderr.print "progress signal\n"
      if max < 1 or cur > max 
        progress.pulse
      else
        progress.fraction = cur.to_f / max.to_f
      end
    }

    # This signal is raised when the user selects a link
    # on a page. We update the statusbar as a real
    # browser would do.
    @moz.signal_connect('link_message') {
      $stderr.print "link_message signal\n"
      status.push(status_context, @moz.link_message)
    }

    # This signal is generated when mozilla starts to load a page
    # We update the statusbar message and reset the progress.
    @moz.signal_connect('net_start') { 
      $stderr.print "net_start signal\n"
      status.push(status_context, "Loading " + @moz.link_message + "...")
      progress.fraction = 0.0
    }

    # This signal is generated when mozilla stops to load a page
    # We update the statusbar message and reset the progress.
    @moz.signal_connect('net_stop') {
      $stderr.print "net_stop signal\n"
      status.push(status_context, "")
      progress.fraction = 0.0
    }

    # This signal is generated when mozilla stops to load a page
    # We update the statusbar message and reset the progress.
    # This does not work at the moment...
    @moz.on_new_window { |widget, mask|
      $stderr.print "new_window\n"
      Browser.new.moz
    }
    
    # The user is bored, let's quit.
    self.signal_connect("destroy") {
      $stderr.print "closing...\n"
      Gtk.main_quit
    }

    self.show_all

  end
end

Browser.new

Gtk.main


# Hint: If you use Mozilla 1.7.x gtkmozembd, you should remove
# libnullplugin.so if you want to get rid of "Default Plugin"
# window. The gtkmozembed from Firefox 1.0 has an improved nullplugin.

# Hint: You can switch between Mozilla and Firefox implementation by
# setting the library path appropiately. The Firefox version of
# GtkMozEmbed is better integrated in Gtk and has nicer scroll-bars
# and message boxes. (use LD_LIBRARY_PATH, see extconf.rb or do some 
# googling)

# Happy coding, Mirko Maischberger
