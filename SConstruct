import platform

env = Environment()
env['CC'] = 'gcc'
env['LINK'] = 'gcc'

CF = ['-std=c17', '-Wall', '-pedantic']
target = 'sof'

Help("\nSOF bootstrap, type 'scons'\n")

operating_system = f'{env["PLATFORM"]}_{platform.architecture()[0]}'

platform_specifics = {
    'win32_32bit': {
        'files': ['compiler\\windows\\win32\\win32.c'],
        'target_extension': '.exe',
        'cflags': ['-D W32']
    },
    'win32_64bit': {
        'files': ['compiler\\windows\\win64\\win64.c'],
        'target_extension': '.exe',
        'cflags': ['-D W64']
    },
    'posix_32bit': {
        'files': ['compiler/linux/linux.c'],
        'target_extension': '.run',
        'cflags': ['-D L32']
    },
    'posix_64bit': {
        'files': ['compiler/linux/linux.c'],
        'target_extension': '.run',
        'cflags': ['-D L64']
    }
}

print('[INFO] Scanning source files')
source_files = []
source_files += Glob('*.c')
source_files += Glob('*/*.c')

print('[INFO] Setting platform specifics')
try:
    print(f'[OK] Platform detected as "{operating_system}"')
    source_files += platform_specifics[operating_system]['files']
    target += platform_specifics[operating_system]['target_extension']
    CF += platform_specifics[operating_system]['cflags']
except KeyError:
    print('[ERROR] Invalid platform for build')
    exit(1)

print('[INFO] Building object files')
for file in source_files:
    full_file_name = str(file)
    print(f'[OK] Building: "{full_file_name}"')

    file_name = full_file_name.split('/')[-1].split('.')[0]
    Object(f'objects/{file_name}', file, CCFLAGS=CF)

object_files = Glob('objects/*.o')

print('[INFO] Linking object files')
Program(target, object_files)

print('[OK] Linking complete')
