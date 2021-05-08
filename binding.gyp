{
    "targets": [
        {
            "target_name": "module",
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ],
            'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                'CLANG_CXX_LIBRARY': 'libc++',
                'MACOSX_DEPLOYMENT_TARGET': '10.7',
            },
            'msvs_settings': {
                'VCCLCompilerTool': { 'ExceptionHandling': 1 },
            },
            'conditions': [
                ['OS=="win"', { 'defines': [ '_HAS_EXCEPTIONS=1' ] }]
            ],
            'include_dirs': [
                '.',
                "<!(node -p \"require('node-addon-api').include_dir\")",
                "<!@(node -p \"require('napi-thread-safe-callback').include\")",
                "D:/Program Files/opencv/build/include",
                
            ],
            'link_settings': {
                'libraries': [
                    '-lD:/Program Files/opencv/build/x64/vc15/lib/opencv_world452'
                ],
            },
            "sources": [ "./src/module.cpp"]
        }
    ],

}