#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
//In other words, if you run ./YOUR_COMPILER /path/to/program.c,
//it should produce an executable at /path/to/program and terminate with an exit code of 0.
int main (int argc, char* argv[]){
	
	//----------------
	//-- Preprocess --
	//----------------
	//trunk the string to only have the namefile
	const char *input = argv[1];          

	char base[256];
	snprintf(base, sizeof base, "%s", input);
	char *dot = strrchr(base, '.');
	if (dot) *dot = '\0';                

	char preproc_file[260];
	snprintf(preproc_file, sizeof preproc_file, "%s.i", base);

	pid_t id = fork();
	if (id == -1) { perror("fork"); exit(1); }

	if (id == 0) {
		execlp("gcc", "gcc", "-E", "-P", input, "-o", preproc_file, (char *)NULL);
		perror("execlp gcc -E -P");
		_exit(1);
	}

	int status;
	waitpid(id, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Preprocessing failed\n");
		exit(1);
	}

	//-------------
	//-- Compile --
	//-------------
	id = fork();
	if (id == -1) { perror("fork"); exit(1); }

	if (id == 0) {
		execlp("gcc", "gcc", "-S",preproc_file, "-o","assembly-file.s", (char *)NULL);
		perror("execlp gcc -S");
		_exit(1);
	}
	
	waitpid(id, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Compilation failed\n");
		exit(1);
	}
	remove(preproc_file);

	//-----------------------
	//-- assemble and link --
	//-----------------------
	id = fork();
	if (id == -1) { perror("fork"); exit(1); }

	if (id == 0) {
		execlp("gcc", "gcc","assembly-file.s","-o",base,(char *)NULL);
		perror("execlp gcc -S");
		_exit(1);
	}
	
	waitpid(id, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Assembly and link failed\n");
		exit(1);
	}
	remove("assembly-file.s");


	return 0;
}
