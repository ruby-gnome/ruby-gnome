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

module Vte
  class Terminal
    if method_defined?(:fork_command_full)
      def fork_command(options={})
        pty_flags = options[:pty_flags] || PtyFlags::DEFAULT
        working_directory = options[:working_directory]
        argv = options[:argv] || [ENV["SHELL"] || "/bin/sh"]
        envv = options[:envv]
        default_spawn_flags =
          GLib::Spawn::CHILD_INHERITS_STDIN | GLib::Spawn::SEARCH_PATH
        spawn_flags = options[:spawn_flags] || default_spawn_flags
        succeeded, pid = fork_command_full(pty_flags,
                                           working_directory,
                                           argv,
                                           envv,
                                           spawn_flags)
        if succeeded
          pid
        else
          nil
        end
      end

      alias_method :spawn, :fork_command

      alias_method :pty_raw, :pty
      def pty
        pty_object
      end
    else
      def spawn(options)
        pty_flags = options[:pty_flags] || PtyFlags::DEFAULT
        working_directory = options[:working_directory]
        argv = options[:argv] || [ENV["SHELL"] || "/bin/sh"]
        envv = options[:envv]
        default_spawn_flags =
          GLib::Spawn::CHILD_INHERITS_STDIN | GLib::Spawn::SEARCH_PATH
        spawn_flags = options[:spawn_flags] || default_spawn_flags
        succeeded, pid = spawn_sync(pty_flags,
                                    working_directory,
                                    argv,
                                    envv,
                                    spawn_flags)
        if succeeded
          pid
        else
          nil
        end
      end
    end

    alias_method :feed_child_raw, :feed_child
    def feed_child(text)
      fedd_child_raw(text, text.bytesize)
    end
  end
end
