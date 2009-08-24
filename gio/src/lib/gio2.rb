require 'glib2'
require 'gio2.so'

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

module GLib::File
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

class GLib::InputStream
  def pending=(pending)
    pending ? set_pending : clear_pending
  end
end
