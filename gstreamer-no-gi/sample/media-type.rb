
#  This program uses the media-info library to display information
#  embedded in media files (tags in MP3/OGG files for instance).

require 'gst'

def print_hash(hash)
    hash.each { |key, val| puts "      #{key}: #{val}" } 
end

def print_info(stream)
    puts "- mime type: #{stream.mime}"
    puts "- length: #{stream.length_time / Gst::SECOND} seconds"
    puts "- bitrate: #{stream.bitrate / 1000.0} kbps"
    puts "- number of tracks: #{stream.length_tracks}"
    i = 0 
    stream.tracks.each do |x|
        puts "- track #{i += 1}:"
        puts "  - metadata:"
        if hash = x.metadata
            print_hash(hash)
        end
        puts "  - streaminfo:"
        if hash = x.streaminfo 
            print_hash(hash)
        end
        puts "  - format:"
        if caps = x.format
            caps.length.times { |i| print_hash(caps.get_structure(i)) }
        end
    end
end

Gst.init

if ARGV.empty?
    $stderr.puts "Usage: #{__FILE__} files..."
    exit 1
end

begin
    info = Gst::MediaInfo.new
    info.source = "filesrc" 

    ARGV.each do |x|
        if stream = info.read(x)
            print_info(stream)
        else
            $stderr.puts "No media info found for file #{x}."
        end 
    end
rescue => e
    $stderr.puts "Media error: #{e}."
    exit 1
end
