
require 'rbconfig'
require 'ftools'

Split="split -l 100"

def my_split(fname, split_lno, outname, sfx)
  lno = 0
  ofs = nil
  File.foreach(fname) do |line|

    if lno == 0
      ofs = File.open(outname + sfx, "w")
    end

    ofs.puts line
    lno += 1

    if lno == split_lno
      ofs.close
      lno = 0
      sfx.succ!
    end

  end
  begin
    ofs.close
  rescue
  end
end

if ARGV[0].empty? or ARGV[0] == ".h"
  puts("set input legal header file name.")
  exit(1)
end
if ARGV[1].empty?
  puts("set public function name.")
  exit(1)
end

func = ARGV[1]

if not FileTest.readable?(ARGV[0])
  puts("cannot found " + ARGV[0])
  exit(1)
end

name = File.basename(ARGV[0], ".h")

if not FileTest.directory?(name)
  Dir.mkdir(name) || exit(1)
  Dir.chdir(name); 
  #if /mswin32/ =~ RUBY_PLATFORM
  #  my_split("../#{name}.h", 100, "_#{name}.", "aa")
  #else
  #  `#{Split} ../#{name}.h _#{name}.`
  #end
  my_split("../#{name}.h", 100, "_#{name}.", "aa")
  Dir.chdir("..")
else
  File.safe_unlink("#{name}/Makefile")
end

File.copy("Makefile", "#{name}/Makefile")
Dir.chdir(name)
makefile = File.open("Makefile", "a")
makefile.puts
makefile.puts("# -----------------------------------------")

if /mswin32/ =~ RUBY_PLATFORM
  makefile.print("../#{name}.lib:")
  makefile.print("#{name}.obj ")
else
  makefile.print("../lib#{name}.a: ")
  makefile.print("#{name}.o ")
end

File.safe_unlink(*Dir.glob("#{name}.[ch]"))
cfile = File.open("#{name}.c", "a")
cfile.puts("#include \"ruby.h\"")
cfile.puts("#include <gdk/gdkkeysyms.h>")
cfile.puts("#include \"../global.h\"")
cfile.puts("#include \"#{name}.h\"")
cfile.puts("void #{func}() {")

hfile = File.open("#{name}.h", "a")
for i in Dir.glob("_#{name}.[a-z][a-z]")
  STDERR.puts(i)
  if /mswin32/ =~ RUBY_PLATFORM
    makefile.print("#{i}.obj ")
  else
    makefile.print("#{i}.o ")
  end

  j = i.tr(".", "_")

  hfile.puts("extern void Init_gtk_#{j}();")
  cfile.puts("	Init_gtk_#{j}();")

  File.safe_unlink("#{i}.c")
  cfile2 = File.open(i + ".c", "w")
  cfile2.puts("#include \"ruby.h\"")
  cfile2.puts("#include <gdk/gdkkeysyms.h>")
  cfile2.puts("#include \"../global.h\"")
  cfile2.puts("void Init_gtk_#{j}() {")
  cfile2.puts("  #include \"#{i}\"")
  cfile2.puts("}")
  cfile2.close
end

cfile.puts("}")

makefile.puts
case RUBY_PLATFORM
when /mswin32/
  makefile.puts("	lib /OUT:\$@ \$?")
else
  makefile.puts("	\$(AR) -r \$@ \$?");
  ranlib = Config::CONFIG["RANLIB"]
  makefile.puts("	-#{ranlib} $@ 2> /dev/null || true")
end

makefile.close
cfile.close
