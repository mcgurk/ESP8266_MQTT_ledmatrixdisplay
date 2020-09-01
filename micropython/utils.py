import uos
	
def ls():
  print(uos.listdir())
  
def cat(filename):
  file = open(filename, "r")
  while True:
    data=file.readline()
    if data=='':
      break
    print(data.rstrip())
  file.close()

def unload(mod):
  mod_name = mod.__name__
  import sys
  if mod_name in sys.modules:
    del sys.modules[mod_name]
	
def reimport(mod):
  unload(mod)
  __import__(mod.__name__)
	
def run(filename):
  exec(open(filename).read(),globals())

def mem():
  import micropython
  micropython.mem_info()

def df():
  s = uos.statvfs('//')
  return ('{0} MB'.format((s[0]*s[3])/1048576))

def cd(dir):
  uos.chdir(dir)

def md(dir):
  uos.mkdir(dir)

def rm(filename):
  uos.remove(filename)

  
