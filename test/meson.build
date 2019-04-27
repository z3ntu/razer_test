e = executable('sometest',
               ['sometest.cpp', qt5.preprocess(moc_sources : 'sometest.cpp')],
               include_directories: inc,
               dependencies : dependency('qt5', modules : ['Core', 'DBus', 'Test']))
test('some test', e)

e = executable('testJsonValidity',
               ['testJsonValidity.cpp', qt5.preprocess(moc_sources : 'testJsonValidity.cpp')],
               include_directories: inc,
               dependencies : dependency('qt5', modules : ['Core', 'Test']))
test('test json validity', e)
