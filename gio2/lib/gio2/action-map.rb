# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

module Gio
  module ActionMap
    alias_method :add_action_raw, :add_action
    def add_action(action)
      action = convert_to_action(action) unless action.is_a?(Action)
      add_action_raw(action)
      actions[action.name] = action
    end

    alias_method :remove_action_raw, :remove_action
    def remove_action(name)
      remove_action_raw(name)
      actions.delete(name)
    end

    def add_actions(actions)
      actions.each do |action|
        add_action(action)
      end
    end

    private
    def actions
      @actions ||= {}
    end

    def convert_to_action(definition)
      name = definition[:name]
      parameter_type = definition[:parameter_type]
      state = definition[:state]
      callback = definition[:callback]

      action = SimpleAction.new(name, parameter_type, state)
      if callback
        action.signal_connect("activate") do |*args|
          callback.call(*args)
        end
      end
      action
    end
  end
end
