
#  This program uses the media-info library to display information
#  embedded in media files (tags in MP3/OGG files for instance).

require 'gst'

def print_caps(caps)
    caps.each_property { |name, obj| puts "      #{name}: #{obj}" } 
end

def print_info(stream)
    puts "- mime type: #{stream.mime}"
    puts "- length: #{stream.length_time} seconds"
    puts "- bitrate: #{stream.bitrate / 1000.0} kbps"
    puts "- number of tracks: #{stream.length_tracks}"
    i = 0 
    stream.tracks.each do |x|
        puts "- track #{i += 1}:"
        puts "  - metadata:"
        x.metadata and print_caps(x.metadata)
        puts "  - streaminfo:"
        x.streaminfo and print_caps(x.streaminfo)
        puts "  - format:"
        x.format and print_caps(x.format)
    end
end

Gst.init

info = Gst::MediaInfo.new

if ARGV.empty?
    puts "Usage: #{__FILE__} files..."
    exit 1
end

ARGV.each do |x|
    if stream = info.read(x)
        print_info(stream)
    else
        $stderr.puts "No media info found for file #{x}."
    end 
end
