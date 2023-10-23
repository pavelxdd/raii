project('raii', 'c', license: 'UNLICENSE')

project_source_root = meson.current_source_dir()
project_build_root = meson.current_build_dir()

install_headers('src/raii.h')

astyle = find_program('astyle', required: false)
if astyle.found()
    custom_target('astyle',
        output: 'astyle',
        command: [
            astyle,
            '--options=@0@/.astylerc'.format(project_source_root),
            '-Q', '-n', '-r',
            project_source_root / 'src/*.h',
        ],
        console: true,
    )
endif
