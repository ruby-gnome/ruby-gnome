
#  gst-inspect.rb aims to mimic the native gst-inspect program,
#  written in C language.

#  The purpose of this program is to display various information
#  about GStreamer plugins, which are installed in the system.

#  Currently, the following points are still missing from the output:
#    - GstElement custom functions 
#    -   ''       dynamic parameters 

require 'gst'

$prefix = 0
def prefix; $prefix += 1; yield; $prefix -= 1; end

def puts(o)
   print " " * ($prefix * 4) 
   super o 
end

def print_hierarchy(e)
    a = Array.new
    c = e.class
    while c
        a.push(c.name)
        c = c.superclass
    end
    old_prefix = $prefix
    a.reverse.each { |n| puts n; $prefix += 1 }
    $prefix = old_prefix
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

