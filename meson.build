project('razer_test', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++14'])

conf_data = configuration_data()
conf_data.set('version', meson.project_version())

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
    'src/razer_test.cpp',
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
    'src/led/razerled.cpp',
    'src/led/razermatrixled.cpp',
    'src/manager/devicemanager.cpp'
]

processed = qt5.preprocess(
  moc_headers : [
    'src/customeffect/customeffectbase.h',
    'src/customeffect/customeffectthread.h',
    'src/customeffect/spectrumeffect.h',
    'src/dbus/devicemanageradaptor.h',
    'src/dbus/razerdeviceadaptor.h',
    'src/dbus/razerledadaptor.h',
    'src/device/razerdevice.h',
    'src/led/razerled.h',
    'src/manager/devicemanager.h'
  ]
)

if get_option('build_daemon')
  executable('razer_test', [src, processed], dependencies : [hidapi, qt5_dep])
endif
if get_option('build_demo')
  executable('razer_test_demo', [src, processed], dependencies : [hidapi, qt5_dep], cpp_args : '-DDEMO')
endif
