project('qdbuscpp2xml-plugin', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

src = ['myplugin.cpp', '../../src/led/razerled.cpp']
inc = include_directories('../../src/led/')

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : ['Core', 'DBus'])

processed = qt5.preprocess(
  moc_headers : [
    'myplugin.h'
  ]
)

shared_library('qdbuscpp2xml-plugin', [src, processed], dependencies : qt5_dep, include_directories : inc)
