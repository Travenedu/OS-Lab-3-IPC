#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){
  int pipefd[2];
  int pipefd2[2];
  pipe(pipefd);
  pipe(pipefd2);

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  int status;

  pid_t pid = fork();

  if(pid == 0){
    pid_t pid_two = fork();
    
    if(pid_two == 2){
      
      dup2(pipefd2[0], 0);

      close(pipefd[1]);
      close(pipefd[0]);
      close(pipefd2[1]);
      close(pipefd2[0]);
      //sort
      execvp(*sort_args, sort_args);
    } 

    dup2(pipefd2[0], 0);
    dup2(pipefd2[1], 1);

    close(pipefd[1]);
    close(pipefd[0]);
    close(pipefd2[1]);
    close(pipefd2[0]);

    //grep
    execvp(*grep_args, grep_args);

  } else{
    
    dup2(pipefd2[1], 1);

    close(pipefd[1]);
    close(pipefd[0]);
    close(pipefd2[1]);
    close(pipefd2[0]);

    //cat
    execvp(*cat_args, cat_args);

    for (int i = 0; i < 3; i++){
      wait(&status);
    }
  }

}
