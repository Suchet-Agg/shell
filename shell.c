//Suchet Aggarwal
//2018105

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h> 
#include<fcntl.h> 
#include<sys/wait.h>

int read_command(char *input){
//parses the string and tokenises it into smaller commands and then uses fork and exec system calls to execute the original command
	if(strcmp(input,"")==0){
		exit(0);
	}

	int cnt = 0;
	int i = 0;
    char* token; 
    char* rest = malloc(4);
	if(rest != NULL){
	  strcpy(rest, input);
	}

    char *rest1 = malloc(4);
	if(rest1 != NULL){
	  strcpy(rest1, input);
	}

    while ((token = strtok_r(rest, "|", &rest)))
    { 
	    cnt++;
	}
	char *cmd[cnt];
	char *args[cnt][20];
	char *tok;

    while ((tok = strtok_r(rest1, "|", &rest1)))
    {	
       	cmd[i] = tok;
       	i++;
	}

	for(i = 0;i<cnt;i++){
		rest = cmd[i];
		char *temp;
		int k = 0;
		int j = 0;
	    while ((token = strtok_r(rest, " ", &rest)))
	    { 
		    	args[i][k] = token;
	    		k++;
		}
		args[i][k] = NULL;
	}

	for(int i=0;i<cnt;i++){
		cmd[i]= args[i][0];
	}

	int fdi = 0;
	i=0;
	while(args[0][i]!=NULL){
		if(strcmp(args[0][i],"<")==0){
			fdi = open(args[0][i+1],O_RDWR);
			args[0][i] = NULL;
			break;
		}
		i++;
	}

	int fdf = 1;
	int stde = 2;
	i=0;
	char *temp1;
	int flag = 0;
	int ind = -1;
	while(args[cnt-1][i]!=NULL && args[cnt-1][i+1]!=NULL){
		if(strcmp(args[cnt-1][i],"1")==0 || strcmp(args[cnt-1][i],"1>")==0){
			temp1 = args[cnt-1][i+1];
			fdf = open(temp1,O_RDWR| O_CREAT | O_TRUNC,0666);
			args[cnt-1][i] = NULL;
			break;
		}		
		else if(strcmp(args[cnt-1][i],"2>")==0){
			stde = open(args[cnt-1][i+1],O_RDWR| O_CREAT | O_TRUNC,0666);
			args[cnt-1][i] = NULL;
			break;
		}
		else if(strcmp(args[cnt-1][i],"2>&1")==0){
			ind = i;
			flag = 1;
		}
		else if(strcmp(args[cnt-1][i],">>")==0){
			temp1 = args[cnt-1][i+1];
			fdf = open(temp1,O_RDWR|O_APPEND,0666);
			args[cnt-1][i] = NULL;
			break;
		}
		i++;
	}

	if(ind!=-1){
		args[cnt-1][ind]= NULL;
	}

	if(cnt>1){
		int fd[cnt][2];
		pid_t pid[cnt];
		pid_t wpid;
		int status = 0;
		for(int i = 0;i<cnt-1;i++){
			pipe(fd[i]);
		}

		for(int i=0;i<cnt;i++){
			if((pid[i] = fork())==0){
				if(i==0){
					if(fdi!=fileno(stdin)){
						dup2(fdi, fileno(stdin));
						fflush(stdin); close(fdi);
					}

					close(fd[i][0]);
					close(1);
					dup(fd[i][1]);
					close(fd[i][1]);

					for(int i =0;i<cnt-1;i++){
						close(fd[i][0]);
						close(fd[i][1]);
					}

					execvp(cmd[i],args[i]);
					printf("Command '%s' not found\n",cmd[i]);
					exit(0);
				}
				else if(i==cnt-1){
					if(fdf!=fileno(stdout)){
						dup2(fdf, fileno(stdout));
						fflush(stdout); close(fdf);
					}

					if(stde!=fileno(stderr)){
						dup2(stde, fileno(stderr));
						fflush(stderr); close(stde);
					}

					if(flag){
						dup2(1,2);
					}

					close(fd[i-1][1]);
					close(0);
					dup(fd[i-1][0]);
					close(fd[i-1][0]);

					for(int i =0;i<cnt-1;i++){
						close(fd[i][0]);
						close(fd[i][1]);
					}
					execvp(cmd[i],args[i]);
					printf("Command '%s' not found\n",cmd[i]);
					exit(0);
				}
				else{
					close(fd[i-1][1]);
					close(0);
					dup(fd[i-1][0]);
					close(fd[i-1][0]);

					close(fd[i][0]);
					close(1);
					dup(fd[i][1]);
					close(fd[i][1]);

					for(int i =0;i<cnt-1;i++){
						close(fd[i][0]);
						close(fd[i][1]);
					}

					execvp(cmd[i],args[i]);
					printf("Command '%s' not found\n",cmd[i]);
					exit(0);
				}
			}
		}

		for(int i =0;i<cnt-1;i++){
			close(fd[i][0]);
			close(fd[i][1]);
		}

		while ((wpid = wait(&status)) > 0);

	}
	else{
		
		if(strcmp(cmd[0],"cd")==0)
		{
			chdir(args[0][1]);
			exit(0);
		}

		int pid = fork();
		if(pid>0){
			wait(NULL);
		}
		else{
			if(fdf!=fileno(stdout)){
				dup2(fdf, fileno(stdout));
				fflush(stdout); close(fdf);
			}
			if(fdi!=fileno(stdin)){
				dup2(fdi, fileno(stdin));
				fflush(stdin); close(fdi);
			}
			if(stde!=fileno(stderr)){
				dup2(stde, fileno(stderr));
				fflush(stderr); close(stde);
			}
			if(flag){
				dup2(1,2);
			}
			execvp(cmd[0],args[0]);
			printf("Command '%s' not found\n",cmd[0]);
			exit(0);
		}
	}
	exit(0);
}

int main(){
	int i = 1;
	while (i) {
		char *input = malloc(400);
		write(1,"$ ",2);
		gets(input);

		if(strcmp(input,"exit")==0){
			break;
		}

		int p = fork();
		if(p==0){
			i = read_command(input);
			exit(0);
		}
		else{
			wait(NULL);
		}
	}
}