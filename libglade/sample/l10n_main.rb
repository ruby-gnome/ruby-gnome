#!/usr/bin/env ruby
=begin
  l10n.rb - Ruby/Libglade2 sample script.

  - Localization.
  - Don't touch auto-generated code(l10n_gui.rb).

  Required: Ruby-GetText-Package-0.6.1 or later.

  Copyright (c) 2004,2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: l10n_main.rb,v 1.2 2005/02/12 04:10:52 mutoh Exp $
=end

#
# Auto-generated code by ruby-glade-create_template.
# You don't need touch here.
#
require 'l10n_gui'
require 'gettext'
 
# Set values as your own application. 
PROG_PATH = "l10n.glade"
PROG_NAME = "l10n"
LOCALE_DIR = "locale"

class L10n < L10nGlade
  include GetText
  GetText.bindtextdomain(PROG_NAME, LOCALE_DIR)

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
L10n.new(PROG_PATH, nil, PROG_NAME, LOCALE_DIR)
Gtk.main
