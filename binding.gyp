{
    "targets": [
        {
            "target_name": "calculator",
            "cflags_cc!": [
                "-fno-exceptions",
                "-fstandalone-debug"
            ],
            "cflags_cc":[
                 "-std=c++17",
                "-fno-exceptions",
                "-fstandalone-debug"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "<(module_root_dir)/include"
            ],            
            "sources": [
                "src/addon.cpp",
                "src/lexer.cpp",
                "src/parser.cpp",
                "src/reporter.cpp",
            ],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "conditions": [
                ['OS=="mac"', {
                    "xcode_settings": {
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                        "OTHER_CFLAGS": [
                            "-ferror-limit=0",
                            "-std=c++2a",
                            "-frtti",
                        ],
                        "RuntimeTypeInfo": "true"
                    }
                }],
            ]
        }
    ]
}