e = executable('sometest',
               ['sometest.cpp', qt6.preprocess(moc_sources : 'sometest.cpp')],
               include_directories: inc,
               dependencies : dependency('qt6', modules : ['Core', 'DBus', 'Test']))
test('some test', e)

e = executable('testJsonValidity',
               ['testJsonValidity.cpp', qt6.preprocess(moc_sources : 'testJsonValidity.cpp')],
               include_directories: inc,
               dependencies : dependency('qt6', modules : ['Core', 'Test']))
test('test json validity', e)
