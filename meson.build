project('razer_test', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++14'])

conf_data = configuration_data()
conf_data.set('version', meson.project_version())
conf_data.set('datadir', join_paths(get_option('prefix'), get_option('datadir'), meson.project_name()))

configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

if host_machine.system() == 'linux'
    hidapi = dependency('hidapi-hidraw')
else
    hidapi = dependency('hidapi', fallback : ['hidapi', 'hidapi'])
endif

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : ['Core', 'DBus'])

if get_option('build_tests')
    subdir('test')
endif

src = [
    'src/daemon.cpp',
    'src/main.cpp',
    'src/razerreport.cpp',
    'src/customeffect/customeffectbase.cpp',
    'src/customeffect/customeffectthread.cpp',
    'src/customeffect/spectrumeffect.cpp',
    'src/customeffect/waveeffect.cpp',
    'src/dbus/devicemanageradaptor.cpp',
    'src/dbus/razerdeviceadaptor.cpp',
    'src/dbus/razerledadaptor.cpp',
    'src/device/razerdevice.cpp',
    'src/device/razerclassicdevice.cpp',
    'src/device/razerfakedevice.cpp',
    'src/device/razermatrixdevice.cpp',
    'src/led/razerclassicled.cpp',
    'src/led/razerfakeled.cpp',
    'src/led/razerled.cpp',
    'src/led/razermatrixled.cpp',
    'src/manager/devicemanager.cpp'
]

processed = qt5.preprocess(
  moc_headers : [
    'src/customeffect/customeffectbase.h',
    'src/customeffect/customeffectthread.h',
    'src/dbus/devicemanageradaptor.h',
    'src/dbus/razerdeviceadaptor.h',
    'src/dbus/razerledadaptor.h',
    'src/device/razerdevice.h',
    'src/led/razerled.h',
    'src/manager/devicemanager.h'
  ]
)

# Install public header
install_headers('src/razer_test.h')

# Install pkg-config file
pkg = import('pkgconfig')
pkg.generate(name: 'razer_test',
             description: 'Header describing the enums and structs used in the D-Bus interface of razer_test')

# Install json device files
install_subdir('data/devices', install_dir : join_paths(get_option('datadir'), meson.project_name()), strip_directory : true)

config_bindir = configuration_data()
config_bindir.set('bindir', join_paths(get_option('prefix'), get_option('bindir')))

configure_file(input : 'data/razer_test.service.in',
               output : 'razer_test.service',
               configuration : config_bindir,
               install_dir : join_paths(get_option('libdir'), 'systemd', 'system'))

configure_file(input : 'data/io.github.openrazer1.service.in',
               output : 'io.github.openrazer1.service',
               configuration : config_bindir,
               install_dir : join_paths(get_option('datadir'), 'dbus-1', 'system-services'))

install_data('data/io.github.openrazer1.conf', install_dir : join_paths(get_option('datadir'), 'dbus-1', 'system.d'))

# Build requested executables
executable('razer_test', [src, processed], dependencies : [hidapi, qt5_dep], install : true)
if get_option('build_bringup_util')
  extra_src = ['src/bringup/bringuputil.cpp']
  executable('razer_test_bringup_util', [src, extra_src, processed], dependencies : [hidapi, qt5_dep], cpp_args : '-DENABLE_BRINGUP_UTIL')
endif
