=begin
  makemo.rb

  Copyright (c) 2004 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  Original file is post-setup.rb from ruby-gettext-package-0.6.0.

  post-setup.rb:
  Copyright (C) 2001-2004 MUTOH Masao <mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby.

  $Id: makemo.rb,v 1.1 2004/07/11 15:09:28 mutoh Exp $
=end
                                                                                
require 'fileutils'
                                                                                
msgfmt = "rmsgfmt"
                                                                                
podir = "./po/"
modir = "locale/%s/LC_MESSAGES/"
                                                                                
Dir.glob("#{podir}*/*.po") do |file|
  lang, basename = /\A([\w\.]*)\/(.*)\.po/.match(file[podir.size..-1]).to_a[1,2]
  outdir = modir % lang
  FileUtils.mkdir_p(outdir) unless File.directory?(outdir)
  system("#{msgfmt} #{file} -o #{outdir}#{basename}.mo")
end
