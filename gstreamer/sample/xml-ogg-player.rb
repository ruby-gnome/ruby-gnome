
# A simple OGG/Vorbis player which uses GStreamer's XML serialization feature
# to save and load a Gst::Pipeline.

require 'gst'

Gst.init

XML_FILE = "/tmp/ogg_pipeline"

unless ARGV.length == 1
    puts "Usage: #{__FILE__} [ogg_file]"
    exit 1
end

def build_pipeline(file)
    src = Gst::ElementFactory.make("filesrc")
    dec = Gst::ElementFactory.make("vorbisfile")
    sink = Gst::ElementFactory.make("osssink")
    src.location = file
    src >> dec >> sink
    pipe = Gst::Pipeline.new("our_ogg_pipeline")
    pipe.add(src, dec, sink)
    pipe
end

def write_pipeline(pipe, file)
    Gst::XML.write_file(pipe, file)
end

def read_pipeline(file)
    xml = Gst::XML.new
    xml.parse_file(file)
    xml.get_element("our_ogg_pipeline")
end

def play_pipeline(pipe)
    pipe.play
    begin 
        while pipe.iterate do end
    rescue Interrupt
    ensure 
        pipe.stop 
    end
end

pipe = build_pipeline ARGV.first
write_pipeline pipe, XML_FILE
pipe = read_pipeline XML_FILE
play_pipeline pipe

