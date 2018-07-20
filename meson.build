project('razer_test', 'cpp')

src = ['razer_test.cpp', 'razerdevice.cpp', 'razerreport.cpp', 'razerled.cpp']

if host_machine.system() == 'linux'
    hidapi = dependency('hidapi-hidraw')
else
    hidapi = dependency('hidapi')
endif

executable('razer_test', src, dependencies : hidapi)
