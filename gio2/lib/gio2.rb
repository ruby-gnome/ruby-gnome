require 'glib2'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, micro, = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gio2.so"
rescue LoadError
  require "gio2.so"
end

class GLib::DataInputStream
  def each
    while line = read_line
      yield line
    end
  end

  alias each_line each

  def each_async(io_priority = GLib::PRIORITY_DEFAULT, cancellable = nil, &block)
    each_async_loop io_priority, cancellable, block
    self
  end

  alias each_line_async each_async

private

  def each_async_loop(io_priority, cancellable, block)
    # TODO: Should we really pass in the object?  I mean, how often is that
    # useful?  We can always get it with result.source_object as well.
    read_line_async io_priority, cancellable do |result|
      if line = read_line_finish(result)
        block.call line
        each_async_loop io_priority, cancellable, block
      end
    end
  end

  # TODO: Add #each_byte?
end

if GLib.const_defined? :DesktopAppInfo
  class GLib::DesktopAppInfo
    class << self
      def desktop_env=(desktop_env)
        set_desktop_env desktop_env
        desktop_env
      end
    end
  end
end

module GLib::File
  def eql?(other)
    self === other and self == other
  end

  alias each enumerate_children

  def each_async(num_files, attributes = nil, flags = nil,
                 io_priority = GLib::PRIORITY_DEFAULT, cancellable = nil,
                 &block)
    enumerate_children_async attributes, flags, io_priority, cancellable do |result|
      enumerate_children_finish(result).each_async num_files, io_priority, cancellable, &block
    end
    self
  end
end

class GLib::FileEnumerator
  def each(cancellable = nil)
    while file = next_file(cancellable)
      yield file
    end
    close
    self
  end

  def each_async(num_files, io_priority = GLib::PRIORITY_DEFAULT, cancellable = nil, &block)
    each_async_loop num_files, io_priority, cancellable, block
    self
  end

private

  def each_async_loop(num_files, io_priority, cancellable, block)
    next_files_async num_files, io_priority, cancellable do |result|
      files = next_files_finish(result)
      files.each do |file|
        block.call file
      end
      if files.length > 0
        each_async_loop num_files, io_priority, cancellable, block
      else
        close
      end
    end
  end
end

class GLib::FileInfo
  def directory?
    file_type == GLib::File::Type::DIRECTORY
  end
end

module GLib::Icon
  def eql?(other)
    self === other and self == other
  end
end

class GLib::InputStream
  def pending=(pending)
    pending ? set_pending : clear_pending
  end
end

class GLib::Resolver
  class << self
    def default=(default)
      set_default default
      default
    end
  end
end

module GLib::SocketConnectable
  include Enumerable

  def each(cancellable = nil)
    enumerator = enumerate
    while address = enumerator.next(cancellable)
      yield address
    end
  end
end
