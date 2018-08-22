src = [
    'sometest.cpp'
]

processed = qt5.preprocess(
  moc_headers : [
    'sometest.h'
  ]
)

e = executable('sometest', [src, processed], dependencies : [dependency('qt5', modules : ['Core', 'DBus', 'Test'])])
test('some test', e)
