
require 'gst'

Gst.init

if ARGV.length != 1
    puts "Usage: #{__FILE__} mp3-file"
    exit
end

file = ARGV.first

pipeline = Gst::Pipeline.new
filesrc = Gst::ElementFactory.make("filesrc")
filesrc.location = file
pipeline.add(filesrc)

autobin = Gst::Bin.new
cache = Gst::ElementFactory.make("autoplugcache")
cache.signal_connect("cache_empty") do |cache|
    $stderr.puts("have cache empty")
    pipeline.pause
    new_element = autobin.get_by_name("new element")
    filesrc.unlink_pads(cache)
    cache.unlink_pads(new_element)
    autobin.remove(cache)
    filesrc.link_pads(new_element)
    pipeline.play
    $stderr.puts("done with cache_empty")
end

typefind = Gst::ElementFactory.make("typefind")
typefind.signal_connect("have_type") do |typefind, caps|
    $stderr.puts("GstPipeline: play have type")
    pipeline.pause
    cache.unlink_pads(typefind)
    autobin.remove(typefind)
    osssink = Gst::ElementFactory.make("osssink")
    autoplug = Gst::AutoplugFactory.make("staticrender")
    unless element = autoplug.to_renderers(caps, osssink)
        puts "Could not autoplug, no suitable codecs found..."
        exit 1
    end
    element.name = "new element"
    autobin.add(element)
    cache.reset = true
    cache.link_pads(element)
    pipeline.play
end
autobin.add(cache, typefind)

cache.link_pads(typefind)
autobin.add_ghost_pad(cache.get_pad("sink"))

pipeline.add(autobin)
filesrc.link_pads(autobin)

pipeline.play
begin
    while pipeline.iterate do end
rescue Interrupt
ensure
    # XXX stop the pipeline
    exit!   # exits all running children (needed)
end

