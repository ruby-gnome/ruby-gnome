# Copyright (C) 2013-2017  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

module GLib
  extend GLib::Deprecatable
  define_deprecated_const :AppInfo, 'Gio::AppInfo'
  define_deprecated_const :AppLaunchContext, 'Gio::AppLaunchContext'
  define_deprecated_const :AskPasswordFlags, 'Gio::AskPasswordFlags'
  define_deprecated_const :AsyncInitable, 'Gio::AsyncInitable'
  define_deprecated_const :AsyncResult, 'Gio::AsyncResult'
  define_deprecated_const :BufferedInputStream, 'Gio::BufferedInputStream'
  define_deprecated_const :BufferedOutputStream, 'Gio::BufferedOutputStream'
  define_deprecated_const :Cancellable, 'Gio::Cancellable'
  define_deprecated_const :CharsetConverter, 'Gio::CharsetConverter'
  define_deprecated_const :ContentType, 'Gio::ContentType'
  define_deprecated_const :Converter, 'Gio::Converter'
  define_deprecated_const :ConverterInputStream, 'Gio::ConverterInputStream'
  define_deprecated_const :ConverterOutputStream, 'Gio::ConverterOutputStream'
  define_deprecated_const :DataInputStream, 'Gio::DataInputStream'
  define_deprecated_const :DataOutputStream, 'Gio::DataOutputStream'
  define_deprecated_const :DataStreamByteOrder, 'Gio::DataStreamByteOrder'
  define_deprecated_const :DataStreamNewlineType, 'Gio::DataStreamNewlineType'
  define_deprecated_const :DesktopAppInfo, 'Gio::DesktopAppInfo'
  define_deprecated_const :Drive, 'Gio::Drive'
  define_deprecated_const :Emblem, 'Gio::Emblem'
  define_deprecated_const :EmblemedIcon, 'Gio::EmblemedIcon'
  define_deprecated_const :File, 'Gio::File'
  define_deprecated_const :FileAttribute, 'Gio::FileAttribute'
  define_deprecated_const :FileAttributeInfo, 'Gio::FileAttributeInfo'
  define_deprecated_const :FileAttributeMatcher, 'Gio::FileAttributeMatcher'
  define_deprecated_const :FileDescriptorBased, 'Gio::FileDescriptorBased'
  define_deprecated_const :FileEnumerator, 'Gio::FileEnumerator'
  define_deprecated_const :FileIOStream, 'Gio::FileIOStream'
  define_deprecated_const :FileIcon, 'Gio::FileIcon'
  define_deprecated_const :FileInfo, 'Gio::FileInfo'
  define_deprecated_const :FileInputStream, 'Gio::FileInputStream'
  define_deprecated_const :FileMonitor, 'Gio::FileMonitor'
  define_deprecated_const :FileOutputStream, 'Gio::FileOutputStream'
  define_deprecated_const :FilenameCompleter, 'Gio::FilenameCompleter'
  define_deprecated_const :FilesystemPreviewType, 'Gio::FilesystemPreviewType'
  define_deprecated_const :FilterInputStream, 'Gio::FilterInputStream'
  define_deprecated_const :FilterOutputStream, 'Gio::FilterOutputStream'
  define_deprecated_const :IO, 'Gio::IO'
  define_deprecated_const :IOModule, 'Gio::IOModule'
  define_deprecated_const :IOModules, 'Gio::IOModules'
  define_deprecated_const :IOScheduler, 'Gio::IOScheduler'
  define_deprecated_const :IOSchedulerJob, 'Gio::IOSchedulerJob'
  define_deprecated_const :IOStream, 'Gio::IOStream'
  define_deprecated_const :Icon, 'Gio::Icon'
  define_deprecated_const :InetAddress, 'Gio::InetAddress'
  define_deprecated_const :InetSocketAddress, 'Gio::InetSocketAddress'
  define_deprecated_const :Initable, 'Gio::Initable'
  define_deprecated_const :InputStream, 'Gio::InputStream'
  define_deprecated_const :LoadableIcon, 'Gio::LoadableIcon'
  define_deprecated_const :MemoryInputStream, 'Gio::MemoryInputStream'
  define_deprecated_const :MemoryOutputStream, 'Gio::MemoryOutputStream'
  define_deprecated_const :Mount, 'Gio::Mount'
  define_deprecated_const :MountOperation, 'Gio::MountOperation'
  define_deprecated_const :NetworkAddress, 'Gio::NetworkAddress'
  define_deprecated_const :NetworkService, 'Gio::NetworkService'
  define_deprecated_const :OutputStream, 'Gio::OutputStream'
  define_deprecated_const :PasswordSave, 'Gio::PasswordSave'
  define_deprecated_const :Resolver, 'Gio::Resolver'
  define_deprecated_const :Seekable, 'Gio::Seekable'
  define_deprecated_const :SimpleAsyncResult, 'Gio::SimpleAsyncResult'
  define_deprecated_const :Socket, 'Gio::Socket'
  define_deprecated_const :SocketAddress, 'Gio::SocketAddress'
  define_deprecated_const :SocketAddressEnumerator, 'Gio::SocketAddressEnumerator'
  define_deprecated_const :SocketClient, 'Gio::SocketClient'
  define_deprecated_const :SocketConnectable, 'Gio::SocketConnectable'
  define_deprecated_const :SocketConnection, 'Gio::SocketConnection'
  define_deprecated_const :SocketConnectionFactory, 'Gio::SocketConnectionFactory'
  define_deprecated_const :SocketControlMessage, 'Gio::SocketControlMessage'
  define_deprecated_const :SocketListener, 'Gio::SocketListener'
  define_deprecated_const :SocketService, 'Gio::SocketService'
  define_deprecated_const :SrvTarget, 'Gio::SrvTarget'
  define_deprecated_const :TcpConnection, 'Gio::TcpConnection'
  define_deprecated_const :ThemedIcon, 'Gio::ThemedIcon'
  define_deprecated_const :ThreadedSocketService, 'Gio::ThreadedSocketService'
  define_deprecated_const :UnixConnection, 'Gio::UnixConnection'
  define_deprecated_const :UnixFDList, 'Gio::UnixFDList'
  define_deprecated_const :UnixFDMessage, 'Gio::UnixFDMessage'
  define_deprecated_const :UnixInputStream, 'Gio::UnixInputStream'
  define_deprecated_const :UnixMount, 'Gio::UnixMount'
  define_deprecated_const :UnixMountMonitor, 'Gio::UnixMountMonitor'
  define_deprecated_const :UnixMountPoint, 'Gio::UnixMountPoint'
  define_deprecated_const :UnixMountPoints, 'Gio::UnixMountPoints'
  define_deprecated_const :UnixMounts, 'Gio::UnixMounts'
  define_deprecated_const :UnixOutputStream, 'Gio::UnixOutputStream'
  define_deprecated_const :UnixSocketAddress, 'Gio::UnixSocketAddress'
  define_deprecated_const :Vfs, 'Gio::Vfs'
  define_deprecated_const :Volume, 'Gio::Volume'
  define_deprecated_const :VolumeMonitor, 'Gio::VolumeMonitor'
  define_deprecated_const :ZlibCompressor, 'Gio::ZlibCompressor'
  define_deprecated_const :ZlibDecompressor, 'Gio::ZlibDecompressor'
  define_deprecated_enums 'Gio::FilesystemPreviewType', 'FILESYSTEM_PREVIEW_TYPE'
  define_deprecated_enums 'Gio::DataStreamByteOrder', 'DATA_STREAM_BYTE_ORDER'
  define_deprecated_enums 'Gio::DataStreamNewlineType', 'DATA_STREAM_NEWLINE_TYPE'
  define_deprecated_flags 'Gio::AskPasswordFlags', 'ASK_PASSWORD'
  define_deprecated_enums 'Gio::PasswordSave', 'PASSWORD_SAVE'
  define_deprecated_singleton_method :gio_has_unix?, :warn => "Use 'Gio.has_unix?'." do |_self|
    Gio.has_unix?
  end
end

module Gio
  module File
    extend GLib::Deprecatable

    define_deprecated_singleton_method(:commandline_arg,
                                       "open(arg: arg, cwd: cwd)") do |_self, arg, cwd=nil|
      open(arg: arg, cwd: cwd)
    end

    define_deprecated_singleton_method(:path,
                                       "open(path: path)") do |_self, path|
      open(path: path)
    end

    define_deprecated_singleton_method(:uri,
                                       "open(uri: uri)") do |_self, uri|
      open(uri: uri)
    end
  end
end
