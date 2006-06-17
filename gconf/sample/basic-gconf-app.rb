#!/usr/bin/env ruby
#
# basic-gconf-app.rb
#
# Copyright (C) 2004 Giovanni Corriga
#
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: basic-gconf-app.rb,v 1.3 2006/06/17 14:39:54 mutoh Exp $

require 'gconf2'
require 'gtk2'

# This program demonstrates how to use Ruby/GConf2.  The key thing is that
# the main window and the prefs dialog have NO KNOWLEDGE of one
# another as far as configuration values are concerned; they don't
# even have to be in the same process. That is, the GConf::Client acts
# as the data "model" for configuration information; the main
# application is a "view" of the model; and the prefs dialog is a
# "controller."
#
# You can tell if your application has done this correctly by
# using "gconftool-2" instead of your preferences dialog to set
# preferences. For example:
#
#   gconftool-2 --type string --set /apps/basic-gconf-app/foo "My string"
#
# If that doesn't work every bit as well as setting the value
# via the prefs dialog, then you aren't doing things right. ;-)
#
#
# If you really want to be mean to your app, make it survive
# this:
#
#   gconftool-2 --break-key /apps/basic-gconf-app/foo
#
# Remember, the GConf database is just like an external file or
# the network - it may have bogus values in it. GConf admin
# tools will let people put in whatever they can think of.
#
# GConf does guarantee that string values will be valid UTF-8, for
# convenience.


# Special mention of an idiom often used in GTK+ apps that does
# not work right with GConf but may appear to at first:
#
#  i_am_changing_value = TRUE;
#  change_value (value);
#  i_am_changing_value = FALSE;
#
# This breaks for several reasons: notification of changes
# may be asynchronous, you may get notifications that are not
# caused by change_value () while change_value () is running,
# since GConf will enter the main loop, and also if you need
# this code to work you are probably going to have issues
# when someone other than yourself sets the value.
#
# A robust solution in this case is often to compare the old
# and new values to see if they've really changed, thus avoiding
# whatever loop you were trying to avoid.


class BasicGConfApp

	# The keys we're going to play with
	def initialize
		@keys = ['/apps/basic-gconf-app/foo',
			 '/apps/basic-gconf-app/bar',
			 '/apps/basic-gconf-app/baz',
			 '/apps/basic-gconf-app/blah']
	end

	#Create an entry used to edit the given config key
	def create_config_entry(key)
		hbox = Gtk::HBox.new(false, 5)
		label = Gtk::Label.new(key)
		entry = Gtk::Entry.new

		hbox.pack_start(label, false, false)
		hbox.pack_end(entry, false, false)

		value = @client[key]
		entry.text = value unless value.nil?

		# The prefs dialog knows NOTHING about the existence 
		# of the main window; it is purely a way to fool
		# with the GConf database. It never does something like change
		# the main window directly; it ONLY changes GConf keys via
		# GConf::Client. This is _important_, because people may configure
		# your app without using your preferences dialog.
		commit_entry = proc {
			text = entry.text
			if text.empty?
				@client.unset key
			else
				@client[key] = text
			end
			false
		}

		entry.signal_connect('focus_out_event', &commit_entry)
		entry.signal_connect('activate', &commit_entry)

		# Set the entry insensitive if the key it edits isn't writable.
		# Technically, we should update this sensitivity if the key gets
		# a change notify, but that's probably overkill.
		entry.sensitive = @client.key_is_writable? key

		return hbox
	end

	# Create a Gtk::Label inside a frame, that we can "configure"
	# (the label displays the value of the config key).
	def create_configurable_widget(key)
		frame = Gtk::Frame.new('Value of "%s"' % key)
		label = Gtk::Label.new
		frame.add label

		value =  @client[key]
		label.text = value unless value.nil?

		# Notification callback for our label widgets that
		# monitor the current value of a gconf key. i.e.
		# we are conceptually "configuring" the label widgets
		notify_id = @client.notify_add(key) { | cli, entry |
			# Note that value can be NULL (unset) or it can have
			# the wrong type! Need to check that to survive
			# gconftool-2 --break-key
			if entry.value.nil?
				label.text = ""
			else
				label.text = entry.value
			end
		}

		# Remove the notification callback when the widget monitoring
		# notifications is destroyed
		label.signal_connect('destroy') {
			# Note that notify_id will be 0 if there was an error,
			# so we handle that
			@client.notify_remove notify_id unless (notify_id == 0)
		}

		return frame
	end

	def create_main_window
		@main_window = Gtk::Window.new
		@main_window.title = 'basic-gconf-app Main Window'

		vbox = Gtk::VBox.new(false, 5)
		@main_window.add vbox
		vbox.border_width = 5

		@keys.each { | key |
			vbox.pack_start self.create_configurable_widget(key)
		}

		@main_window.signal_connect('destroy') {
			Gtk.main_quit
		}

		@prefs = Gtk::Button.new('_Prefs')
		vbox.pack_end @prefs
		@prefs.signal_connect('clicked') {
			self.prefs_clicked
		}

		return @main_window
	end

	def create_prefs_dialog
		@prefs_dialog = Gtk::Dialog.new('basic-gconf-app Preferences',
						@main_window, nil,
						[Gtk::Stock::CLOSE, Gtk::Dialog::RESPONSE_ACCEPT])

		@prefs_dialog.signal_connect('response') {
			@prefs_dialog.destroy
		}

		@prefs_dialog.default_response = Gtk::Dialog::RESPONSE_ACCEPT

		@prefs_dialog.resizable = false

		vbox = Gtk::VBox.new(false, 5)

		vbox.border_width = 5

		@prefs_dialog.vbox.pack_start vbox

		@keys.each { | key |
			vbox.pack_start(self.create_config_entry(key),
					false, false)
		}
		#vbox.children[0].grab_focus
	end

	def prefs_clicked
		if @prefs_dialog.nil?
			self.create_prefs_dialog
			@prefs_dialog.signal_connect('destroy') {
				@prefs_dialog = nil
			}
			@prefs_dialog.show_all
		else
			@prefs_dialog.present
		end
	end

	def start
		@client = GConf::Client.default

		# Tell the GConf::Client instance that we're interested in the given
		# directory. This means it will receive notification of changes
		# to this directory, and cache keys under this directory.
		# So _don't_ add "/" or something silly like that or you'll end
		# up with a copy of the whole GConf database. ;-)
		#
		# We pass GConf::Client::PRELOAD_NONE to avoid loading all config
		# keys on startup. If your app pretty much reads all config keys
		# on startup, then preloading the cache may make sense. We could have
		# passed no second argument since PRELOAD_NONE is the default value.
		@client.add_dir('/apps/basic-gconf-app', GConf::Client::PRELOAD_NONE)

		self.create_main_window
		@main_window.show_all
	end

end


#main loop

app = BasicGConfApp.new
app.start

Gtk.main
