project('razer_test', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++11'])

src = [
    'src/razer_test.cpp',
    'src/razerreport.cpp',
    'src/led/razerled.cpp',
    'src/led/razerclassicled.cpp',
    'src/device/razerdevice.cpp',
    'src/device/razerclassicdevice.cpp',
    'src/device/razermatrixdevice.cpp'
]

if host_machine.system() == 'linux'
    hidapi = dependency('hidapi-hidraw')
else
    hidapi = dependency('hidapi')
endif

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : ['Core', 'DBus'])

executable('razer_test', src, dependencies : [hidapi, qt5_dep])
