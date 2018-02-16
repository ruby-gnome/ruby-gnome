# Copyright (C) 2015-2017  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

module Gtk
  class ActionGroup
    alias_method :add_action_raw, :add_action
    def add_action(action, options={})
      accelerator = options[:accelerator]

      if accelerator
        add_action_with_accel(action, accelerator)
      else
        add_action_raw(action)
      end
    end

    def add_actions(actions)
      actions.each do |config|
        if config.is_a?(Array)
          name        = config[0]
          stock_id    = config[1]
          label       = config[2]
          accelerator = config[3]
          tooltip     = config[4]
          callback    = config[5]
        else
          name        = config[:name]
          stock_id    = config[:stock_id]
          label       = config[:label]
          accelerator = config[:accelerator]
          tooltip     = config[:tooltip]
          callback    = config[:callback]
        end

        action = Action.new(name,
                            :label    => translate_string(label),
                            :tooltip  => translate_string(tooltip),
                            :stock_id => stock_id)
        action.signal_connect("activate") do
          callback.call(self, action)
        end
        add_action(action, :accelerator => accelerator)
      end
    end

    def add_toggle_actions(actions)
      actions.each do |config|
        if config.is_a?(Array)
          name        = config[0]
          stock_id    = config[1]
          label       = config[2]
          accelerator = config[3]
          tooltip     = config[4]
          callback    = config[5]
          is_active   = config[6]
        else
          name        = config[:name]
          stock_id    = config[:stock_id]
          label       = config[:label]
          accelerator = config[:accelerator]
          tooltip     = config[:tooltip]
          callback    = config[:callback]
          is_active   = config[:is_active]
        end

        action = ToggleAction.new(name,
                                  :label    => translate_string(label),
                                  :tooltip  => translate_string(tooltip),
                                  :stock_id => stock_id)
        action.active = true if is_active
        action.signal_connect("activate") do
          callback.call(self, action)
        end
        add_action(action, :accelerator => accelerator)
      end
    end

    def add_radio_actions(actions, default_value=nil)
      actions.each_with_index do |config, i|
        if config.is_a?(Array)
          name        = config[0]
          stock_id    = config[1]
          label       = config[2]
          accelerator = config[3]
          tooltip     = config[4]
          value       = config[5]
        else
          name        = config[:name]
          stock_id    = config[:stock_id]
          label       = config[:label]
          accelerator = config[:accelerator]
          tooltip     = config[:tooltip]
          value       = config[:value]
        end

        action = RadioAction.new(name,
                                 value,
                                 :label    => translate_string(label),
                                 :tooltip  => translate_string(tooltip),
                                 :stock_id => stock_id)
        action.active = true if value == default_value
        if i.zero?
          action.signal_connect("changed") do |connected_action, current_action|
            yield(connected_action, current_action)
          end
        end
        add_action(action, :accelerator => accelerator)
      end
    end

    alias_method :translate_string_raw, :translate_string
    def translate_string(string)
      return nil if string.nil?

      translate_string_raw(string)
    end
  end
end
