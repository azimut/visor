((c-mode . ((compile-command . "cd .. && make -B && ./main")
            (dape-configs . ((gdb-main
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B DEBUG=1"
                              :request "launch"
                              :program "main"
                              :stopAtBeginningOfMainSubprogram t))))))
