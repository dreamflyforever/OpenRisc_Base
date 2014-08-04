import glob
import config

env = Environment(tools = ['mingw'],
	AS = config.AS, ASFLAGS = config.AFLAGS,
	CC = config.CC, CCFLAGS = config.CFLAGS,
	AR = config.AR, ARFLAGS = '-rc',
	LINK = config.LINK, LINKFLAGS = config.LFLAGS)
env.PrependENVPath('PATH', config.EXEC_PATH)
src = Glob('/home/yangshanjjin/test/test_arch_openrisc/*.c')
src += Glob('/home/yangshanjjin/test/test_arch_openrisc/*.S')
print src

env.Program('hello',src)

