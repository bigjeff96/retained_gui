set shell := ["bash", "-uc"]

find := if os() == "windows" {"/cygdrive/c/cygwin64/bin/find.exe"} else {"find"}
exe := "retained_gui.exe"
debug_exe := "retained_gui_debug.exe"

debug:
    time g++ main.cpp -Wall -Wextra -Werror -ggdb -o {{exe}} -fsanitize=address	
    # just move_exe

debug_run:
	just debug
	./{{exe}}
	
#ignore
debug_watch:
    #!/bin/sh
    just debug
    clear
    echo -e '\rok'
    just move_exe

release:
    g++ main.cpp -Wall -Wextra -Werror -O3 -o {{exe}}
	
#Line count of project
loc:
    tokei -t C++ -o json . | jq '.["C++"].code + .["C++"].comments'
install:
    just release
    mv {{exe}} /cygdrive/c/Projects/bin/{{exe}}

# fmt:
#     #!/bin/sh
#     for i in $({{ find }} . -name "*.odin" -type f); do
#         {{odinfmt}} -w "$i"
#     done             

move_exe:
    #!/bin/sh
    mv {{exe}} {{debug_exe}}

