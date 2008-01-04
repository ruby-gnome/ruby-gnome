#!/usr/bin/evn ruby

require 'optparse'
require 'ostruct'

argv = ARGV.dup
ARGV.clear

require 'gst'

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

  options
end

class Inspector
  def initialize(print_all)
    @print_all = print_all
    @prefix = ""
  end

  def print
    print_list
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
    registry = Gst::Registry.default
    registry.plugins.sort_by {|plugin| plugin.name}.each do |plugin|
      features = registry.get_features(plugin.name)
      features.sort_by {|feature| feature.name}.each do |feature|
        yield(plugin, feature)
      end
    end
  end

  def print_factory_details_info(factory)
    rank_name = factory.rank.name
    rank_name ||= "unknown"
    rank_name = rank_name.gsub(/GST_RANK_/, '').downcase

    puts("Factory Details:",
         "  Long name:\t#{factory.long_name}",
         "  Class:\t#{factory.klass}",
         "  Description:\t#{factory.description}",
         "  Author(s):\t#{factory.author}",
         "  Rank:\t\t#{rank_name} (#{factory.rank.to_i})",
         "")
  end

  def print_plugin_info(name)
    return if name.nil?
    registry = Gst::Registry.default
    plugin = registry.find_plugin(name)
    return if plugin.nil?

    puts("Plugin Details:",
         "  Name:\t\t\t#{plugin.name}",
         "  Description:\t\t#{plugin.description}",
         "  Filename:\t\t#{plugin.filename || '(null)'}",
         "  Version:\t\t#{plugin.version}",
         "  License:\t\t#{plugin.license}",
         "  Source module:\t#{plugin.source}",
         "  Binary package:\t#{plugin.package}",
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

    caps.each do |structure|
      puts(structure.name)
      structure.each do |key, value|
        puts("  %15s: %s" % [key, value.inspect])
      end
    end
  end

  def print_pad_template_info(template)
    prefix("#{@prefix}  ") do
      puts("#{template.direction.nick.upcase}: '#{template.name}'")
      prefix("#{@prefix}  ") do
        if template.presence == Gst::Pad::REQUEST
          puts("Availability: On request")
        else
          puts("Availability: #{template.presence.nick.capitalize}")
        end
      end
    end
    puts

    static_caps = template.caps
    return if static_caps.description.nil?
    prefix("#{@prefix}    ") do
      puts("Capabilities:")
      prefix("#{@prefix}  ") do
        print_caps(static_caps.to_caps)
      end
    end
    puts
  end

  def print_pad_templates_info(element, factory)
    puts("Pad Templates:")
    templates = factory.pad_templates

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
    if !element.require_clock? and
        !element.provide_clock? and
        element.clock.nil?
      puts("Element has no clocking capabilities.")
      puts
      return
    end

    puts("Clocking Interaction:")
    puts("  element requires a clock") if element.require_clock?
    if element.provide_clock?
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
    if element.indexable?
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
        current_value = element.get_property(name)
        value = "Default: #{param.default} Current: #{current_value.inspect}"
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
    if !factory.load!
      puts("element plugin (#{factory.name}) couldn't be loaded\n")
      return
    end

    element = factory.create

    prefix(print_names ? "#{factory.name}: " : "") do
      print_factory_details_info(factory)
      print_plugin_info(factory.plugin_name)

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
      when Gst::IndexFactory
        puts(": #{feature.description}")
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

  def print_list
    plugins = {}
    n_features = 0
    each_feature do |plugin, feature|
      plugins[plugin.name] = nil
      n_features += 1
      if @print_all
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

  def show_all
  end
end

options = parse(argv)
Inspector.new(options.print_all).print

__END__
$prefix = 0
def prefix; $prefix += 1; yield; $prefix -= 1; end

def puts(o)
   print " " * ($prefix * 4) 
   super o 
end

def print_prop(name, obj)
    s = (name) ? name.concat(":").ljust(15) : ""
    if obj.is_a?(Array)
        puts s + "List:"
        prefix { obj.each { |obj| print_prop(nil, obj) } } 
    else
        puts s + obj.class.to_s + ": " + obj.to_s
    end
end

def print_format(f)
    puts "(#{f.type_id}) :".ljust(10) + f.nick + " (#{f.description})" 
end

def get_consts(klass)
    res = {}
    klass.constants.each do |x|
        res[eval(klass.name + "::" + x)] = x
    end
    res
end

def print_event_mask(m)
    event_name = get_consts(Gst::Event)[m.type_id].downcase
    flags = case m.type_id
    when Gst::Event::SEEK
        get_consts(Gst::EventSeek)
    when Gst::Event::SEEK_SEGMENT
        get_consts(Gst::EventSegmentSeek)
    when Gst::Event::SIZE
        get_consts(Gst::EventSize)
    else
        {}
    end
    flags.delete_if do |flag, name| 
        Gst::Event.constants.include?(flag)
    end
    s = event_name
    m.type_flags.each do |flag|
        flag_name = flags.has_key?(flag) ? flags[flag].downcase : flag.to_s 
		s += " | " + flag_name
    end
    puts s
end

def print_query(q)
    puts "(#{q.type_id}) :".ljust(10) + q.nick + " (#{q.description})" 
end

def print_pad(p)
    dir = case p.direction
        when Gst::Pad::SRC  then "SRC"
        when Gst::Pad::SINK then "SINK"
        else "UNKNOWN"
    end
    puts dir + ": " + p.name
    puts "Pad Template: " + p.pad_template.name if p.pad_template
    prefix do
        puts "Implementation:"
#         if p.provides_formats? 
#             prefix do
#                 puts "Provides seeking/conversion/query formats:"
#                 prefix { p.each_format { |f| print_format(f) } }
#             end
#         end
#         if p.provides_event_masks?
#             prefix do
#                 puts "Provides event masks:"
#                 prefix { p.each_event_mask { |m| print_event_mask(m) } }
#             end
#         end
        if p.provides_query_types?
            prefix do
                puts "Provides query types:"
                prefix { p.each_query_type { |q| print_query(q) } }
            end
        end
    end
end

def print_element_info(f)
  raise "Could not create element" unless e = f.create
  puts "Factory Details:"
  prefix do
    f.details.each do |key, val|
      puts key.capitalize.concat(":").ljust(15) + val
    end
  end
  puts ""
  print_hierarchy(e)
  puts ""
  puts "Pad Templates:"
  prefix do
    f.each_pad_template do |name, direction, presence, caps|
      dir = case direction
            when Gst::Pad::SRC  then "SRC"
            when Gst::Pad::SINK then "SINK"
            else "UNKNOWN"
            end
      puts dir + " template: " + name
      pres = case presence
             when Gst::Pad::ALWAYS    then "always"
             when Gst::Pad::SOMETIMES then "sometimes"
             when Gst::Pad::REQUEST   then "on request"
             else "unknown"
             end
      puts "Avaibility: " + pres
      if caps
        puts "Capabilities: #{caps}"
      end
    end
  end
#   puts ""
#   puts "Element Flags:"
#   prefix do
#     flags = {
#       e.complex?          => "GST_ELEMENT_COMPLEX", 
#       e.decoupled?        => "GST_ELEMENT_DECOUPLED", 
#       e.thread_suggested? => "GST_ELEMENT_THREADSUGGESTED",
#       e.event_aware?      => "GST_ELEMENT_EVENT_AWARE" 
#     }
#     flags.each { |b, v| puts v if b }
#     puts "no flags set" unless flags.has_key?(true) 
#   end
  puts ""
  puts "Clock Interaction:"
  prefix do
    puts "element requires a clock" if e.requires_clock? 
    if e.provides_clock?
      if c = e.clock   
        puts "element provides a clock: " + c.name 
      else
        puts "element is supposed to provide a clock but returned nil"
      end
    end
    puts "none" unless (e.requires_clock? or e.provides_clock?)        
  end
  puts ""
  puts "Indexing capabilities:"
  prefix { puts((e.indexable?) ? "element can do indexing" : "none") }
  puts ""
  puts "Pads:"
  prefix do
    if e.pads.empty?
      puts "none"
    else
      e.pads.each { |pad| print_pad(pad) }
    end
  end
#   puts ""
#   puts "Element Arguments:"
#   prefix do
#     e.each_property do |key, descr, val|
#       puts key.concat(":").ljust(15) + descr
#       puts " ".ljust(15) + "#{val.class.to_s} (default: '#{val}')"
#     end
#   end
  puts ""
  puts "Signals:"
  prefix do
    if e.class.signals.empty?
      puts "none"
    else
      e.class.signals.each { |x| puts x }
    end
  end
end

def print_plugin_info(p)
    puts "Plugin details:"
    prefix do
    	x = 15
        puts "Name:".ljust(x) + p.name
        puts "Description:".ljust(x) + p.description
        puts "Filename:".ljust(x) + p.filename
        puts "Version:".ljust(x) + p.version
        puts "License:".ljust(x) + p.license
        puts "Origin URL:".ljust(x) + p.origin
    end
    puts ""
    stats = Hash.new
    prefix do 
        p.each_feature do |f| 
            puts f
            stats[f.class] ||= 0
            stats[f.class] += 1
        end
    end 
    puts ""
    prefix do
        puts stats.size.to_s + " feature(s):"
        stats.each do |f, c|
            prefix { puts c.to_s + " " + f.name }
        end
    end 
end

def print_element_list
  Gst::Registry.default.each_plugin do |p|
    puts p.name + ":" 
    prefix { p.each_feature { |f| puts f } } 
  end
end

def print_help
    puts <<EOS
Usage: 
  #{__FILE__}                   List all registred elements
  #{__FILE__} element_name      Show element details
  #{__FILE__} plugin-name[.so]  Show information about plugin
EOS
    exit 1
end

case ARGV.length
    when 0 then print_element_list
    when 1 
        n = ARGV.first.dup
        if n.sub!(/\.so$/, "").nil? and f = Gst::ElementFactory.find(n) 
            print_element_info(f)
        else
            if p = Gst::Registry.default.find_plugin(n)
                print_plugin_info(p)
            else
                $stderr.puts("No such element or plugin `#{n}'")
                exit 1
            end
        end 
    else
        print_help
end 

