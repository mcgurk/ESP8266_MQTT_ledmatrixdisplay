def ls():
  import os
  print(os.listdir())
  
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
	
def run(mod):
  unload(mod)
  __import__(mod.__name__)

def mem():
  import micropython
  micropython.mem_info()

def df():
  import os
  s = os.statvfs('//')
  return ('{0} MB'.format((s[0]*s[3])/1048576))

def cd(dir):
  import os
  os.chdir(dir)

def md(dir):
  import os
  os.mkdir(dir)
  
