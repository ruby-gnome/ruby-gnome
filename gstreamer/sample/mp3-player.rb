
require 'gst'

def usage
    puts "Usage: #{__FILE__} mp3-file"
    exit
end

def main
    usage if ARGV.length != 1 
    file = ARGV[0]
    
    # Create a pipeline to hold the elements
    pipeline = Gst::Pipeline.new("my_pipeline")
    
    # Create a disk reader
    filesrc = Gst::ElementFactory.make("filesrc", "my_disk_source")
    filesrc.location = file 

    # Now it's time to get the MP3 decoder
    decoder = Gst::ElementFactory.make("mad", "my_decoder")

    # And an audio sink
    audiosink = Gst::ElementFactory.make("osssink", "my_audio_player")

    # Add objects to the main pipeline
    pipeline.add(filesrc, decoder, audiosink)

    # Link disk source => decoder => audio player 
    filesrc >> decoder >> audiosink

    # Start playing
    puts "Playing #{file}..."
    pipeline.play

    begin
        # Wait until the file is played
        while pipeline.iterate do end
    rescue Interrupt
    ensure
        # Stop the pipeline
        pipeline.stop
    end
end

Gst.init
main

