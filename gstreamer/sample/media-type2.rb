
# This is a Ruby implementation of the GStreamer media-info library,
# graciously provided by Nikolai Weilbull <lone-star@home.se>.
#
# It works exactly like media-type.rb.

require 'gst'

class MediaInfoStream
  def initialize
    @length_tracks = 0
    @length_time = 0
    @bitrate = 0
    @seekable = false
    @path = nil
    @mime = nil
    @tracks = []
  end
  attr_accessor :length_time, :length_tracks, :path, :mime, :bitrate, :tracks
end

class MediaInfoTrack
  def initialize
    @metadata = nil
    @streaminfo = nil
    @format = nil
    @length_time = 0
    @con_streams = nil
  end

  attr_accessor :metadata, :streaminfo, :format
end

class MediaInfo
  # TODO: add signals
  INFO_ALL = (0..4).to_a.length - 1
  INFO_STREAM, INFO_MIME, INFO_METADATA, INFO_STREAMINFO, INFO_FORMAT = *(0..4).to_a
  def initialize(source_element)
    @pipeline = Gst::Pipeline.new 'media-info'
    @typefind = Gst::ElementFactory.make 'typefind', 'typefind'
    @source_element = source_element
    @source = Gst::ElementFactory.make @source_element, 'source'
    if @source != nil
      @pipeline.add @source
    else
      @source_element = nil
      @source = nil
    end
    @decoder = @type = @format = @metadata = @stream = nil
    @decoders = Hash.new
    @pipeline.signal_connect 'deep_notify' do |obj, origin, pspec|
      case pspec.name
      when 'metadata'
	@metadata = origin.metadata
      when 'caps'
	@format = origin.caps if origin == @decoder_pad
      when 'streaminfo'
	@streaminfo = origin.streaminfo
      end
    end
  end

  def reset
    @format = nil
    @metadata = nil
    @stream = nil
  end

  def seek_to_track(track)
    track_format = Gst::Format.find 'logical_stream'
    return false if track_format == nil
    @pipeline.play
		# XXX: todo, need Gst::Event
    event = gst_event_new_seek(format.id | Gst::EventSeek::METHOD_SET |
			       Gst::EventSeek::FLAG_FLUSH)
    return false if event == nil
    @metadata = nil
    @streaminfo = nil
    return true
  end

  def get_decoder(mime)
    decoder = @decoders[mime]
    if decoder == nil
      factories = {
	'application/x-ogg' => 'vorbisfile',
	'audio/x-mp3' => 'mad',
	'audio/x-wav' => 'wavparse'
      }
      factory = factories[mime]
      return nil if factory == nil
      decoder = Gst::ElementFactory.make factory, 'decoder'
      @decoders[mime] = decoder if decoder != nil
    end
    return decoder
  end

  def find_type(location)
    @type = nil
    @pipeline.add @typefind
    @source.location = location
    @source >> @typefind
    @typefind.signal_connect 'have-type' do |typefind, type|
      @type = type
    end
    @pipeline.play
    while @type == nil && @pipeline.iterate do end
    @pipeline.ready
    # TODO: this is different
    @source.unlink_pads @typefind
    @pipeline.remove @typefind
  end

  def get_stream(stream)
    @pipeline.play
    while @format == nil && @pipeline.iterate do end
    @pipeline.pause
    return nil if @format == nil
    track_format = Gst::Format.find 'logical_stream'
    @decoder_pad.formats.each do |format|
      value = @decoder_pad.query(Gst::QueryType::TOTAL, format.type_id)
      case format.type_id
      when Gst::Format::TIME
	stream.length_time = value ? value : 1
      else
	stream.length_tracks = value if format == track_format && value != nil
      end
    end
    stream.length_tracks = 1 if stream.length_tracks == 0
    value = @source_pad.query(Gst::QueryType::TOTAL, Gst::Format::BYTES)
    if value
      seconds = stream.length_time.to_f / Gst::SECOND 
      bits = value * 8
      stream.bitrate = bits / seconds
    end
    return stream
  end

  def find_metadata
    @pipeline.play
    while @metadata == nil && @pipeline.iterate do end
    @pipeline.pause
    metadata = @metadata
    @metadata = nil
    return metadata
  end

  def find_streaminfo
    @pipeline.play
    while @streaminfo == nil && @pipeline.iterate do end
    @pipeline.pause
    streaminfo = @streaminfo
    @streaminfo = nil

    track_format = Gst::Format.find 'logical_stream'
    if track_format != nil
      start = @decoder_pad.query(Gst::QueryType::POSITION, track_format)
      track_num = start
      # XXX: need Gst::Pad#convert
    end
    return streaminfo
  end

  def find_format
    @pipeline.play
    while @format == nil && @pipeline.iterate do end
    @pipeline.pause
    format = @format
    @format = nil
    return format
  end

  def clear_decoder
    if @decoder != nil
      @pipeline.ready
      @source.unlink_pads @decoder
      @pipeline.remove @decoder
      @decoder = nil
    end
  end

  def set_decoder(decoder)
    @decoder = decoder
    @pipeline.add @decoder
    @source >> @decoder
    @decoder_pad = @decoder.get_pad 'src'
    @source_pad = @source.get_pad 'src'
  end

  def set_source(source)
    src = Gst::ElementFactory.make source, 'new-source'
    return false if src == nil
    if @source != nil
      @pipeline.remove @source
      @source_element = nil if @source_element != nil
    end
    src.name = 'source'
    @pipeline.add src
    @source = src
    @source_element = source
    return true
  end

  def read(location, flags)
    reset
    find_type location
    return nil if @type == nil
    stream = MediaInfoStream.new
    mime = @type.mime
    stream.mime = mime if flags & INFO_MIME
    stream.path = location
    decoder = get_decoder mime
    return nil if decoder == nil
    set_decoder decoder
    stream = get_stream stream
    stream.length_tracks.times do |i|
      track = MediaInfoTrack.new
      track.metadata = find_metadata if flags & INFO_METADATA
      track.streaminfo = find_streaminfo if flags & INFO_STREAMINFO
      track.format = find_format if flags & INFO_FORMAT
      stream.tracks << track
    end
    clear_decoder
    return stream
  end
end

def usage
  puts "Usage: #{__FILE__} FILE..."
  exit 1
end

def print_caps(caps)
  return if caps == nil
  caps.each_property do |name, value, fixed|
    puts "          #{name}: #{value}"
  end
end

def print_info(path, stream)
  if stream
    puts "#{path}:"
    puts "  mime type: #{stream.mime}"
    puts "  length: #{stream.length_time / Gst::SECOND} seconds"
    puts "  bitrate: #{stream.bitrate / 1000.0} kbps"
    puts "  number of tracks: #{stream.length_tracks}"
    puts "  tracks:"
    stream.tracks.each_with_index do |track, i|
      puts "    - track #{i + 1}:"
      puts "        metadata:"
      print_caps track.metadata
      puts "        streaminfo:"
      print_caps track.streaminfo
      puts "        format:"
      print_caps track.format
    end
  else
    puts "No media info found for file #{path}"
  end
end

usage if ARGV.empty?
Gst.init
mediainfo = MediaInfo.new 'filesrc'
ARGV.each do |path|
  print_info path, mediainfo.read(path, MediaInfo::INFO_ALL)
end

