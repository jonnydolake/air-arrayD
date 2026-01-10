#define NOB_IMPLEMENTATION
#include "include/nob.h"

#define BUILD "build/"
#define SRC   "src/"

int strcmp(const char *s1, const char *s2);

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD)) return 1;

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "clang");
    nob_cc_flags(&cmd);
    nob_cmd_append(&cmd, "-I./include");
    nob_cc_output(&cmd, BUILD "main");
    nob_cc_inputs(&cmd, SRC "airlib.c");
    if (!nob_cmd_run(&cmd)) return 1;

    if (strcmp(argv[argc-1], "run") == 0) {
        nob_cmd_append(&cmd, "build/main");
        if (!nob_cmd_run(&cmd)) return 1;
    }

    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (int)*s1 - (int)*s2;
}
