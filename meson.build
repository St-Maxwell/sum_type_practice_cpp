project('calculator','cpp',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

inc = include_directories(
  'src',
  'third_party'
)

src = files(
  'src/tokenize.cpp',
  'src/parse.cpp',
  'src/expr.cpp',
  'src/evaluate.cpp',
)

lib = static_library('calc', src,
  include_directories : inc
)

executable('calculator', 'src/main.cpp',
  include_directories: inc,
  link_with : lib,
  dependencies : dependency('fmt')
)

test_evaluate = executable('test_evaluate', 'test/test_evaluate.cpp',
  include_directories: inc,
  link_with : lib,
  dependencies : dependency('fmt')
)
test('test_evaluate', test_evaluate)

test_expr = executable('test_expr', 'test/test_expr.cpp',
  include_directories: inc,
  link_with : lib,
  dependencies : dependency('fmt')
)
test('test_expr', test_expr)
