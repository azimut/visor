((c-mode . ((compile-command . "make -B")
            (dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B"
                              :request "launch"
                              :program "main"
                              :stopAtBeginningOfMainSubprogram t))))))
