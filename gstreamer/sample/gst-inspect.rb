#!/usr/bin/evn ruby
#
# This sample code is a port of gstreamer/tools/gst-inspect.c. It is
# licensed under the terms of the GNU Library General Public License,
# version 2 or (at your option) later.
#
# The followin is the original code's copyright notation:
#   Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
#                 2000 Wim Taymans <wtay@chello.be>
#                 2004 Thomas Vander Stichele <thomas@apestaart.org>
#
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

require "optparse"
require "ostruct"

require "gst"

def parse(argv)
  options = OpenStruct.new
  options.print_all = false
  options.print_auto_install_info = false

  opts = OptionParser.new do |opts|
    opts.banner += " [ELEMENT-NAME|PLUGIN-NAME]"

    opts.version = Gst.version

    opts.separator("")
    opts.on("-a", "--print-all", "Print all elements") do
      options.print_all = true
    end

    opts.on("--print-plugin-auto-install-info",
            "Print a machine-parsable list of features",
            "the specified plugin provides.",
            "Useful in connection with external",
            "automatic plugin installation mechanisms") do
      options.print_auto_install_info = true
    end
  end
  opts.parse!(argv)

  [options, argv.first]
end

class Inspector
  def initialize
    @prefix = ""
  end

  def print_list(print_all)
    plugins = {}
    n_features = 0
    each_feature do |plugin, feature|
      plugins[plugin.name] = nil
      n_features += 1
      if print_all
        if feature.is_a?(Gst::ElementFactory)
          print_element_factory(feature, true)
        end
      else
        print_feature(plugin, feature)
      end
    end
    puts
    puts("Total count: #{plugins.size} plugins, #{n_features} features")
  end

  def print_element(name)
    factory = Gst::ElementFactory.find(name)
    print_element_factory(factory, false)
  end

  private
  def puts(*args)
    if args.empty?
      super(@prefix)
    else
      super(*(args.collect {|arg| "#{@prefix}#{arg}"}))
    end
  end

  def prefix(new_prefix)
    prefix, @prefix = @prefix, new_prefix
    yield
  ensure
    @prefix = prefix
  end

  def each_feature
    registry = Gst::Registry.get
    registry.plugins.sort_by {|plugin| plugin.name}.each do |plugin|
      features = registry.get_features(plugin.name)
      features.sort_by {|feature| feature.name}.each do |feature|
        yield(plugin, feature)
      end
    end
  end

  def print_factory_details_info(factory)
    puts("Factory Details:",
         "  Rank:\t\t#{factory.rank.nick} (#{factory.rank.to_i})",
         "  Long name:\t#{factory.long_name}",
         "  Class:\t#{factory.klass}",
         "  Description:\t#{factory.description}",
         "  Author(s):\t#{factory.author}",
         "")
  end

  def print_plugin_info(name)
    return if name.nil?
    registry = Gst::Registry.get
    plugin = registry.find_plugin(name)
    return if plugin.nil?

    puts("Plugin Details:",
         "  Name:\t\t\t#{plugin.name}",
         "  Description:\t\t#{plugin.description}",
         "  Filename:\t\t#{plugin.filename || '(null)'}",
         "  Version:\t\t#{plugin.version}",
         "  License:\t\t#{plugin.license}",
         "  Source module:\t#{plugin.source}")
    release_date = plugin.release_date_string
    if release_date
      puts("  Source release date:\t#{release_date}")
    end
    puts("  Binary package:\t#{plugin.package}",
         "  Origin URL:\t\t#{plugin.origin}",
         "")
  end

  def print_hierarchy(element)
    ancestors = []
    type = element.gtype
    while type
      ancestors << type
      type = type.parent
    end
    ancestors.reverse.each_with_index do |klass, i|
      if i.zero?
        mark = ""
      else
        mark = " " + ("      " * (i - 1)) + "+----"
      end
      puts("#{mark}#{klass.name}")
    end
    puts
  end

  def print_interfaces(element)
    interfaces = element.gtype.interfaces
    return if interfaces.empty?

    puts("Implemented Interfaces:")
    interfaces.each do |interface|
      puts("  #{interface.name}")
    end
    puts
  end

  def print_caps(caps)
    if caps.any?
      puts("ANY")
      return
    end
    if caps.empty?
      puts("EMPTY")
      return
    end

    caps.structures.each do |structure|
      puts(structure.name)
      structure.fields.each do |name, value|
        puts("  %15s: %s" % [name, value.serialize])
      end
    end
  end

  def print_pad_template_info(template)
    prefix("#{@prefix}  ") do
      puts("#{template.direction.nick.upcase} template: '#{template.name_template}'")
      prefix("#{@prefix}  ") do
        if template.presence == Gst::PadPresence::REQUEST
          puts("Availability: On request")
        else
          puts("Availability: #{template.presence.nick.capitalize}")
        end
      end
    end

    caps = template.caps
    return if caps.nil?
    prefix("#{@prefix}    ") do
      puts("Capabilities:")
      prefix("#{@prefix}  ") do
        print_caps(caps)
      end
    end
    puts
  end

  def print_pad_templates_info(element, factory)
    puts("Pad Templates:")
    templates = factory.static_pad_templates

    if templates.empty?
      puts("  none")
      return
    end

    templates.each do |template|
      print_pad_template_info(template)
    end
  end

  def print_element_flag_info(element)
    puts("Element Flags")
    puts("  no flags set")
    puts

    return unless element.is_a?(Gst::Bin)
    puts("Bin Flags:")
    puts("  no flags set")
    puts
  end

  def print_implementation_info(element)
    puts("Element Implementation:")
    puts("  not supported")
    puts
  end

  def print_clocking_info(element)
    if !element.flags.require_clock? and
        !element.flags.provide_clock? and
        element.clock.nil?
      puts("Element has no clocking capabilities.")
      puts
      return
    end

    puts("Clocking Interaction:")
    puts("  element requires a clock") if element.flags.require_clock?
    if element.flags.provide_clock?
      clock = element.clock
      if clock.nil?
        puts("  element is supported tot provide a clock but returned nil")
      else
        puts("  element provides a clock: #{clock.name}")
      end
    end
    puts
  end

  def print_index_info(element)
    if element.flags.indexable?
      puts("Indexing capabilities:")
      puts("  element can do indexing")
    else
      puts("Element has no indexing capabilities.")
    end
    puts
  end

  def print_pad_info(element)
    puts("Pads:")
    pads = element.pads
    if pads.empty?
      puts("  none")
      return
    end

    pads.each do |pad|
      prefix("#{@prefix}  ") do
        puts("#{pad.direction.nick.upcase}: '#{pad.name}'")
        prefix("#{@prefix}  ") do
          template = pad.template
          puts("Pad Template: '#{template.name}'") if template

          caps = pad.caps
          break if caps.nil?
          puts("Capabilities:")
          prefix("#{@prefix}  ") do
            print_caps(caps)
          end
        end
      end
    end
    puts
  end

  def print_element_properties_info(element)
    puts("Element Properties:")
    properties = element.class.properties
    if properties.empty?
      puts(" none")
      return
    end

    properties.each do |name|
      param = element.class.property(name)
      puts("  %-20s: %s" % [param.name, param.blurb])

      flags = []
      flags << "readable" if param.readable?
      flags << "writable" if param.writable?
      # flags << "controllable" if param.controllable?

      prefix("#{@prefix}#{' ' * 23} ") do
        puts("flags: #{flags.join(', ')}")
        type_name = param.value_type.name
        if param.readable?
          current_value = element.get_property(name)
          value = "Default: #{param.default.inspect} "
          value << "Current: #{current_value.inspect}"
        end
        write_only = param.readable? ? "" : " Write only"
        puts("#{type_name}. #{value}#{write_only}")
      end
    end
    puts
  end

  def print_signal_info(element, signal)
    args = ["object(#{element.class})"]
    signal.param_types.each_with_index do |name, i|
      args << "arg#{i}(#{name})"
    end
    args = args.join(", ")
    signature = "{|#{args}| ...} # => #{signal.return_type}"
    puts("  #{signal.name.inspect}: #{signature}")
  end

  def print_signals_info(element)
    signals = []
    actions = []
    element.class.signals.each do |name|
      signal = element.class.signal(name)
      next if signal.owner >= Gst::Element
      if signal.action?
        actions << signal
      else
        signals << signal
      end
    end

    [[signals, "Signals"],
     [actions, "Actions"]].each do |target_signals, description|
      unless target_signals.empty?
        puts("Element #{description}:")
        target_signals.each do |signal|
          print_signal_info(element, signal)
        end
      end
    end
    puts
  end

  def print_children_info(element)
    return unless element.is_a?(Gst::Bin)
    children = element.children
    return if children.empty?

    puts("Children:")
    children.each do |child|
      puts("  #{child.name}")
    end
  end

  def print_element_factory(factory, print_names)
    factory = factory.load
    if factory.nil?
      puts("element plugin (#{factory.name}) couldn't be loaded\n")
      return
    end

    element = factory.create

    prefix(print_names ? "#{factory.name}: " : "") do
      print_factory_details_info(factory)
      print_plugin_info(factory.plugin.name)

      print_hierarchy(element)
      print_interfaces(element)

      print_pad_templates_info(element, factory)
      print_element_flag_info(element)
      print_implementation_info(element)
      print_clocking_info(element)
      print_index_info(element)
      print_pad_info(element)
      print_element_properties_info(element)
      print_signals_info(element)
      print_children_info(element)
    end
  end

  def print_feature(plugin, feature)
    prefix("#{plugin.name}: #{feature.name}") do
      case feature
      when Gst::ElementFactory
        puts(": #{feature.long_name}")
      when Gst::TypeFindFactory
        if feature.extensions.empty?
          message = "no extensions"
        else
          message = feature.extensions.join(", ")
        end
        puts(": #{message}")
      else
        puts(" (#{feature.gtype})")
      end
    end
  end
end

options, element_name = parse(ARGV)
inspector = Inspector.new
if element_name
  inspector.print_element(element_name)
else
  inspector.print_list(options.print_all)
end
