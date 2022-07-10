def Settings(**kwargs):
    return {
            'flags': [
                '-x', 'c++',
                '-Wall', '-Wextra', '-Werror',
                '-I', '.',
                '-I', './bazel-packingsolver/external/json/single_include/',
                '-I', './bazel-packingsolver/external/googletest/googletest/include/',
                '-I', './bazel-packingsolver/external/boost/',
                '-I', './bazel-packingsolver/external/optimizationtools/',
                # '-I', './../optimizationtools/',
                '-I', './bazel-packingsolver/external/treesearchsolver/',
                # '-I', './../treesearchsolver/',
                '-I', './bazel-packingsolver/external/columngenerationsolver/',
                # '-I', './../columngenerationsolver/',
                ],
            }
