import os

from platform import architecture
from SCons.Environment import Environment

def windows_build_env(flags) -> Environment:
    env = Environment(tools = ['mingw'], ENV = {'PATH' : os.environ['PATH']}, CCFLAGS=flags)
    tmp_dir = os.path.join(os.getcwd(), ".scons-tmp")
    if not os.path.exists(tmp_dir):
        os.makedirs(tmp_dir)
    env['ENV']['TMP'] = tmp_dir
    env['ENV']['TEMP'] = tmp_dir
    return env

def linux_build_env(flags) -> Environment:
    env = Environment(CC = 'gcc', LINK = 'gcc', CCFLAGS=flags)
    return env

env = None
target = None
platform = Environment()["PLATFORM"]

CFlags = ['-std=c17', '-Wall', '-pedantic', '-fms-extensions']

platform_specifics = {
    'win32': {
        '32bit': {
            'files': ['compiler\\windows\\win32\\win32.c'],
            'cflags': ['-D W32'],
        },
        '64bit': {
            'files': ['compiler\\windows\\win64\\win64.c'],
            'cflags': ['-D W64'],
        },
        'target': 'sof.exe',
        'dir_sep': '\\',
        'env': windows_build_env,
    },
    'posix': {
        '32bit': {
            'files': ['compiler/linux/linux.c'],
            'cflags': ['-D L32'],
        },
        '64bit': {
            'files': ['compiler/linux/linux.c'],
            'cflags': ['-D L64'],
        },
        'target': 'sof.run',
        'dir_sep': '/',
        'env': linux_build_env
    }
}
source_files = []

try:
    (system_arch, _) = architecture()
    source_files    += platform_specifics[platform][system_arch]['files']
    CFlags          += platform_specifics[platform][system_arch]['cflags']
    target           = platform_specifics[platform]['target']
    env              = platform_specifics[platform]['env'](CFlags)
except KeyError:
    print('[ERROR] Invalid platform for build')
    exit(1)

objects_to_remove = [str(f) for f in env.Glob('objects/*.o')]
for f in objects_to_remove:
    os.remove(f)

source_files += env.Glob('*.c')
source_files += env.Glob('*/*.c')
source_files += env.Glob('./pkg/*/*/*.c')

dir_sep = platform_specifics[platform]['dir_sep']

for file in source_files:
    full_file_name = str(file)
    file_name = full_file_name.split(dir_sep)[-1].split('.')[0]
    env.Object(f'./objects/{file_name}', file)

object_files = env.Glob('./objects/*')
env.Program(target, object_files)
