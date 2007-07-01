require 'glib2'
require 'gnomevfs.so'

module GnomeVFS
  class FileInfo
    alias setuid? suid?
    alias setgid? sgid?

    def <=>(other)
      unless other.is_a?(FileInfo)
        nil
      else
        mtime <=> other.mtime
      end
    end
    include Comparable
  end

  class File
    class <<self

      def foreach(uri, *args, &block)
        open(uri, *args){|f| f.each(&block) }
      end

      def read(uri, length=nil, offset=nil)
        if offset.nil? or offset==0
          open(uri, OpenMode::READ){|f|
            f.read(length)
          }
        else
          open(uri, OpenMode::READ | OpenMode::RANDOM){|f|
            f.seek(offset)
            f.read(length)
          }
        end
      end

      def readlines(uri, *args)
        open(uri){|f| f.readlines(*args) }
      end

      def readlink(uri)
        lstat(uri).symlink_to
      end

      def utime(atime, mtime, *rest)
        info = FileInfo.new
        info.set_utime(atime, mtime)
        rest.each{|item|
          File.set_file_info(item, info, FileInfo::SET_TIME)
        }
        rest.length
      end

      def exists?(uri)
        unless uri.is_a?(GnomeVFS::URI)
          uri = GnomeVFS::URI.new(uri)
        end
        uri.exists?
      end
      alias exist? exists? 
      
      def ftype(uri)      lstat(uri).ftype end
      def file?(uri)      lstat(uri).file? end
      def directory?(uri) lstat(uri).directory? end
      def symlink?(uri)   lstat(uri).symlink? end
      def chardev?(uri)   lstat(uri).chardev? end
      def blockdev?(uri)  lstat(uri).blockdev? end
      def socket?(uri)    lstat(uri).socket? end
      def pipe?(uri)      lstat(uri).pipe? end

      def size?(uri)      stat(uri).size? end
      def zero?(uri)      stat(uri).zero? end

      def readable?(uri)        stat(uri).readable? end
      def writable?(uri)        stat(uri).writable? end
      def executable?(uri)      stat(uri).executable? end
      def readable_real?(uri)   stat(uri).readable_real? end
      def writable_real?(uri)   stat(uri).writable_real? end
      def executable_real?(uri) stat(uri).executable_real? end

      def owned?(uri)    stat(uri).owned? end
      def grpowned?(uri) stat(uri).grpowned? end

      def sticky?(uri) stat(uri).sticky? end
      def setuid?(uri) stat(uri).setuid? end
      def setgid?(uri) stat(uri).setgid? end

      def size(uri) stat(uri).size end

      def atime(uri) stat(uri).atime end
      def mtime(uri) stat(uri).mtime end
      def ctime(uri) stat(uri).ctime end
    end

    def atime; stat.atime end
    def mtime; stat.mtime end
    def ctime; stat.ctime end

    def pos=(arg)
      seek(arg)
    end
  end
  LOG_DOMAIN = "libgnomevfs"
end

GLib::Log.set_log_domain(GnomeVFS::LOG_DOMAIN)
GLib::Log.set_log_domain("gnome-vfs-modules")
GLib::Log.set_log_domain("GnomeVFSMonikers")
