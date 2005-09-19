#!/usr/bin/env ruby
=begin
  l10n.rb - Ruby/Libglade2 sample script.

  - Localization.
  - Don't touch auto-generated code(l10n_gui.rb).

  Required: Ruby-GetText-Package-0.6.1 or later.

  Copyright (c) 2004,2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: l10n_main.rb,v 1.4 2005/09/19 14:05:21 mutoh Exp $
=end

#
# Auto-generated code by ruby-glade-create_template.
# You don't need touch here.
#
require 'l10n_gui'
 
class L10n < L10nGlade
  include GetText

  # Set values as your own application. 
  PROG_PATH = "l10n.glade"
  PROG_NAME = "l10n"
  LOCALE_DIR = "locale"

  def initialize
    super(PROG_PATH, nil, PROG_NAME, LOCALE_DIR)
    # bind a textdomain to this file(you need to write this in each files).
    bindtextdomain(PROG_NAME, LOCALE_DIR, nil, "UTF-8")
    # :
    # Write your own code for initialization.
    # :
  end

  def on_hello_button_clicked(widget)
    @glade["entry"].text = _("Hello World")
  end

  def on_window1_destroy_event(widget, arg0)
    Gtk.main_quit
  end

  def on_exit_button_clicked(widget)
    Gtk.main_quit
  end
end

Gtk.init 
L10n.new
Gtk.main
