#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
    if (argc != 5)
        ERROR_EXIT("Usage: ./pipex infile \"cmd1\" \"cmd2\" outfile");
    run_pipex(argv[1], argv[2], argv[3], argv[4], envp);
    return 0;
}
