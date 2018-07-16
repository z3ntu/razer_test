project('razer_test', 'c')

src = ['razer_test.c']

if build_machine.system() == 'linux'
    hidapi = dependency('hidapi-hidraw')
else
    hidapi = dependency('hidapi')
endif

executable('razer_test', src, dependencies : hidapi)
