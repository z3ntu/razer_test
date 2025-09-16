project('qdbuscpp2xml-plugin', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

src = ['myplugin.cpp']
inc = include_directories('../../include/')

qt6 = import('qt6')
qt6_dep = dependency('qt6', modules : ['Core', 'DBus'])

processed = qt6.preprocess(
  moc_headers : [
    '../../include/openrazer.h',
    'myplugin.h'
  ]
)

shared_library('qdbuscpp2xml-plugin', [src, processed], dependencies : qt6_dep, include_directories : inc)
