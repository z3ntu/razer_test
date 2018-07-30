project('razer_test', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++11'])

conf_data = configuration_data()
conf_data.set('version', meson.project_version())

configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

src = [
    'src/razer_test.cpp',
    'src/razerreport.cpp',
    'src/led/razerled.cpp',
    'src/led/razerclassicled.cpp',
    'src/device/razerdevice.cpp',
    'src/device/razerclassicdevice.cpp',
    'src/device/razermatrixdevice.cpp',
    'src/dbus/razerdeviceadaptor.cpp',
    'src/dbus/devicemanageradaptor.cpp',
    'src/manager/devicemanager.cpp'
]

if host_machine.system() == 'linux'
    hidapi = dependency('hidapi-hidraw')
else
    hidapi = dependency('hidapi')
endif

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : ['Core', 'DBus'])


processed = qt5.preprocess(
  moc_headers : [
    'src/device/razerdevice.h',
    'src/dbus/razerdeviceadaptor.h',
    'src/dbus/devicemanageradaptor.h',
    'src/manager/devicemanager.h'
  ]
)

executable('razer_test', [src, processed], dependencies : [hidapi, qt5_dep])
