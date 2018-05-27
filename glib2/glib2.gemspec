summary = ''
description = ''
author = 'The Ruby-GNOME2 Project Team'
email = 'ruby-gnome2-devel-en@lists.sourceforge.net'
homepage = 'https://ruby-gnome2.osdn.jp/'
name = File.basename(File.expand_path(__dir__))
version = '3.2.6'

Gem::Specification.new do |s|
  s.name                  = name
  s.summary               = summary
  s.description           = description
  s.author                = author
  s.email                 = email
  s.homepage              = homepage
  s.licenses              = ['LGPL-2.1+']
  s.version               = version
  extensions              = FileList["ext/#{name}/extconf.rb",
                                     'dependency-check/Rakefile']
  extensions.existing!
  s.extensions            = extensions
  s.require_paths         = ['lib']
  files                   = FileList['ChangeLog', 'README',
                                     'Rakefile', 'extconf.rb',
                                     'dependency-check/Rakefile',
                                     'lib/**/*.rb',
                                     'ext/**/depend',
                                     'ext/**/*.{c,h,def,rb}',
                                     '{sample,test}/**/*']
  files.existing!
  s.files = files
end
