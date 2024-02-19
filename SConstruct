env = Environment()

Help("\nSOF bootstrap, type 'scons'\n")

CF = ['-std=c17', '-Wall', '-pedantic']

print('[INFO] Scanning source files')

target = 'sof'

source_files = []
source_files += Glob('*.c')
source_files += Glob('*/*.c')

print('[INFO] Checking platform..')

platform = env['PLATFORM']
if platform == 'win64':
    source_files += ['compiler/windows/win64/win64.c']
    target += '.exe'
    CF += ['-D W64']
elif platform == 'posix':
    source_files += ['compiler/linux/linux.c']
    target += '.run'
    CF += ['-D L64']
else:
    print('[ERROR] Invalid platform for build')
    exit(1)

print(f'[INFO] Platform detected as "{platform}"')

print('[INFO] Building object files')
for file in source_files:
    full_file_name = str(file)
    print(f'[INFO] Building: "{full_file_name}"')

    file_name = full_file_name.split('/')[-1].split('.')[0]
    Object(f'objects/{file_name}', file, CCFLAGS=CF)

object_files = Glob('objects/*.o')

print('[INFO] Linking object files')
if platform == 'win64':
    Program(target, object_files)
else:
    Program(target, object_files)