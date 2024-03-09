set shell := ["bash", "-uc"]

find := if os() == "windows" {"/cygdrive/c/cygwin64/bin/find.exe"} else {"find"}
exe := "retained_gui.exe"
debug_exe := "retained_gui_debug.exe"
gf2 := "/home/joe/.local/bin/gf2"

debug:
    just fmt
    time g++ main.cpp -Wall -Wextra -Werror -pedantic -ggdb -o {{exe}} -lX11 -fsanitize=address,undefined,leak
    # just move_exe

debug_run:
	just debug
	./{{exe}}

gf:
	just debug
	{{gf2}} {{exe}}
	
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

fmt:
	clang-format -i *.cpp *.h

move_exe:
    #!/bin/sh
    mv {{exe}} {{debug_exe}}

