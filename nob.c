#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD "build/"
#define SRC   "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD)) return 1;

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "clang");
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD "main");
    nob_cc_inputs(&cmd, SRC "main.c");
    if (!nob_cmd_run(&cmd)) return 1;

    nob_cmd_append(&cmd, "build/main");
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
