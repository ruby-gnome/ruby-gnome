require 'mkmf'


module PKGConfig
  @@cmd = with_config('pkg-config', 'pkg-config')
  if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
    @@cmd += ' --msvc-syntax'
  end

  def exists?(pkg)
    system("#{@@cmd} --exists #{pkg}")
  end
  module_function :exists?

  def libs(pkg)
    `#{@@cmd} --libs #{pkg}`.chomp
  end
  module_function :libs

  def cflags(pkg)
    `#{@@cmd} --cflags #{pkg}`.chomp
  end
  module_function :cflags

  def variable(pkg, var)
    `#{@@cmd} --variable=#{var} #{pkg}`.chomp
  end
  module_function :variable

  def have_package(pkg)
    STDOUT.print("checking for #{pkg}... ")
    STDOUT.flush
    if exists? pkg
      STDOUT.print "yes\n"
      $libs   += ' ' + libs(pkg)
      $CFLAGS += ' ' + cflags(pkg)
      true
    else
      STDOUT.print "no\n"
      false
    end
  end
  module_function :have_package
end


unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end


STDOUT.print("checking for GCC... ")
STDOUT.flush
if macro_defined?("__GNUC__", "")
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  is_gcc = true
else
  STDOUT.print "no\n"
  is_gcc = false
end


STDOUT.print("checking for G_OS_WIN32... ")
STDOUT.flush
if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
  STDOUT.print "yes\n"
  $CFLAGS += ' -fnative-struct' if is_gcc
else
  STDOUT.print "no\n"
end
