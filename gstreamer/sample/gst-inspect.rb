
#  gst-inspect.rb aims to mimic the native gst-inspect program,
#  written in C language.

#  The purpose of this program is to display various information
#  about GStreamer plugins, which are installed in the system.

#  Currently, the following points are still missing from the output:
#    - GstElement custom functions 
#    -   ''       event masks
#    -   ''       dynamic parameters 
#    -   ''       signals/actions [1]
#
#  [1]: will be fixed when Gst::Object will inherits from GLib::Object.

require 'gst'

$prefix = 0
def prefix; $prefix += 1; yield; $prefix -= 1; end

def puts(s)
    print " " * $prefix * 4 
    super s
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

def print_caps(c)
    puts c.name + " (" + ((c.fixed?) ? "fixed" : "NOT fixed") + ")"
    t = Gst::Type.find_by_id(c.type_id)
    puts "MIME type: " + (t ? t.mime : "unknown/unknown")
    prefix { c.each_property { |name, obj| print_prop(name, obj) } }
end

def print_format(f)
    puts "(#{f.id}) :".ljust(10) + f.nick + " (#{f.description})" 
end

def print_query(q)
    puts "(#{q.id}) :".ljust(10) + q.nick + " (#{q.description})" 
end

def print_pad(p)
    dir = case p.direction
        when Gst::Pad::DIRECTION_SRC  then "SRC"
        when Gst::Pad::DIRECTION_SINK then "SINK"
        else "UNKNOWN"
    end
    puts dir + ": " + p.name
    puts "Pad Template: " + p.pad_template.name if p.pad_template
    prefix do
        puts "Implementation:"
        if p.provides_formats? 
            prefix do
                puts "Provides seeking/conversion/query formats:"
                prefix { p.each_format { |f| print_format(f) } }
            end
        end
#        if p.provides_event_masks?
#            prefix do
#                puts "Provides event masks:"
#                prefix { p.each_event_mask { |e| print_event_mask(e) } }
#            end
#        end
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
        f.each_pad_template do |p|
            dir = case p.direction
                when Gst::Pad::DIRECTION_SRC  then "SRC"
                when Gst::Pad::DIRECTION_SINK then "SINK"
                else "UNKNOWN"
            end
            puts dir + " template: " + p.name
            pres = case p.presence
                when Gst::Pad::PRESENCE_ALWAYS    then "always"
                when Gst::Pad::PRESENCE_SOMETIMES then "sometimes"
                when Gst::Pad::PRESENCE_REQUEST   then "on request"
                else "unknown"
            end
            puts "Avaibility: " + pres
            if p.has_caps?
                puts "Capabilities:"
                prefix { p.each_caps { |c| print_caps(c) } }
            end
        end
    end
    puts ""
    puts "Element Flags:"
    prefix do
        flags = {
            e.complex?          => "GST_ELEMENT_COMPLEX", 
            e.decoupled?        => "GST_ELEMENT_DECOUPLED", 
            e.thread_suggested? => "GST_ELEMENT_THREADSUGGESTED",
            e.event_aware?      => "GST_ELEMENT_EVENT_AWARE" 
        }
        flags.each { |b, v| puts v if b }
        puts "no flags set" unless flags.has_key?(true) 
    end
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
    prefix { puts ((e.indexable?) ? "element can do indexing" : "none") }
    puts ""
    puts "Pads:"
    prefix do
        if pads = e.pads
            pads.each { |pad| print_pad(pad) }
        else
            puts "none"
        end   
    end    
    puts ""
    puts "Element Arguments:"
    prefix do 
        e.each_property do |key, descr, val|
            puts key.concat(":").ljust(15) + descr
            puts " ".ljust(15) + "#{val.class.to_s} (default: '#{val}')"
        end
    end
end

def print_plugin_info(p)
    puts "Plugin details:"
    prefix do
        puts "Name:".ljust(15) + p.name
        puts "Long Name:".ljust(15) + (p.longname or "n/a")
        puts "File Name:".ljust(15) + p.filename
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
    Gst::Registry.each_plugin do |p|
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

Gst.init

case ARGV.length
    when 0 then print_element_list
    when 1 
        n = ARGV.first.dup
        if n.sub!(/\.so$/, "").nil? and f = Gst::ElementFactory.find(n) 
            print_element_info(f)
        else
            if p = Gst::Registry.find_plugin(n)
                print_plugin_info(p)
            else
                $stderr.puts("No such element or plugin `#{n}'")
                exit 1
            end
        end 
    else
        print_help
end 

