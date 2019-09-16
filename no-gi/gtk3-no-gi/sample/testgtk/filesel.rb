=begin header

  filesel.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: filesel.rb,v 1.6 2005/07/17 16:55:27 mutoh Exp $

  Rewritten by TAKAHASHI Hitoshi <thitoshi@ne.scphys.kyoto-u.ac.jp>

Original Copyright:
 
  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

class FileChooserSample < Gtk::FileChooserDialog
  include Sample
  extend SampleClass

  def initialize
    super(
      :title => "file chooser dialog",
      :buttons  => [
# TODO
#        [:cancel, :cancel], # Gtk::Stock::CANCEL,  Gtk::Dialog::ResponseType::CANCEL
#        [:open,   :accept], # Gtk::Stock::OPEN,    Gtk::Dialog::ResponseType::ACCEPT
        [Gtk::Stock::CANCEL,  Gtk::Dialog::ResponseType::CANCEL],
        [Gtk::Stock::OPEN,    Gtk::Dialog::ResponseType::ACCEPT],
      ]
    )

    @destroyed = false
    signal_connect("destroy") do destroy end
    signal_connect("response") do |widget, response_id|
      case response_id
      when Gtk::Dialog::ResponseType::ACCEPT # TODO
        puts filename
        destroy
      when Gtk::Dialog::ResponseType::CANCEL # TODO
        destroy
      end
    end

    set_window_position(:mouse) # Gtk::Window::Position::MOUSE
  end
end
