
# Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
#
# This file is part of Ruby/GStreamer.
# 
# Ruby/GStreamer is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# Ruby/GStreamer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with Ruby/GStreamer; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

require 'tests/misc'

class TC_registry < Test::Unit::TestCase
    def test_registry
        i = 0
        Gst::Registry.each do |r| 
            assert_instance_of(Gst::Registry, r)
            r.test(self)
            i += 1
        end
        assert_instance_of(Array, a = Gst::Registry.registries)
        assert a.size == i
        a.each { |r| assert_instance_of(Gst::Registry, r); r.test(self) }
    end
    def test_prefered
        flags = [ 
            Gst::Registry::READABLE,
            Gst::Registry::WRITABLE,
            Gst::Registry::EXISTS,
            Gst::Registry::REMOTE,
            Gst::Registry::DELAYED_LOADING,
            0
        ]    
        flags.each do |x|
            r = Gst::Registry.get_prefered(x)
            unless r.nil?
                assert_instance_of(Gst::Registry, r)
                r.test(self)
            end
        end
    end
    def test_all_features
        types = [
	        Gst::AutoplugFactory,
	        Gst::ElementFactory,
	        Gst::IndexFactory,
	        Gst::SchedulerFactory,
	        Gst::TypeFactory
        ]
        types.each do |c|
            i = 0
            Gst::Registry.each_feature(c) do |f|
                assert_instance_of(c, f)
                f2 = Gst::Registry.find_feature(f.name, c)
                assert_instance_of(c, f2)
                assert_equal(f.name, f2.name)
                i += 1
            end
            a = Gst::Registry.get_features(c)
            assert_instance_of(Array, a)
            assert_equal(i, a.size)
            a.each { |f| assert_instance_of(c, f) }
        end
    end
    def test_paths
        Gst::Registry.each do |r|
            assert_instance_of(Gst::Registry, r)
            paths = r.paths
            r.clear_paths
            assert_equal(0, r.paths.size)
            paths.each { |p| r.add_path(p) }
            assert_equal(paths.size, r.paths.size)
        end
    end
    def test_load_plugins
        r = Gst::Registry.get_prefered(Gst::Registry::READABLE)
        assert_instance_of(Gst::Registry, r)
        Gst::Registry.each_plugin do |p|
            ret = [
                Gst::Registry::OK,
                Gst::Registry::LOAD_ERROR,
                Gst::Registry::SAVE_ERROR,
                Gst::Registry::PLUGIN_LOAD_ERROR,
                Gst::Registry::PLUGIN_SIGNATURE_ERROR
            ]
            assert r.add_plugin(p)
            p2 = r.find_plugin(p.name)
            assert_instance_of(Gst::Plugin, p2)
            assert_equal(p.name, p2.name)
            assert ret.include?(r.load_plugin(p))
            assert ret.include?(r.update_plugin(p))
            assert ret.include?(r.unload_plugin(p))
            r.remove_plugin(p)
        end
    end
    # XXX:
    # Gst::Registry.add
    # Gst::Registry.remove
    # Gst::Registry.add_plugin
    # Gst::Registry.load_all
    # Gst::Registry#load
    # Gst::Registry#save
    # Gst::Registry#rebuild
    # Gst::Registry#unload
end

