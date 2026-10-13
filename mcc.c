#include <_stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

enum kind{
	TOK_IDENTIFIER,
	TOK_CONSTANT,
	TOK_INT_KEYWORD,
	TOK_VOID_KEYWORD,
	TOK_RETURN_KEYWORD,
	TOK_OPEN_PAR,
	TOK_CLOSE_PAR,
	TOK_OPEN_BRA,
	TOK_CLOSE_BRA,
	TOK_SEMICOLON
};

const char *show_token(enum kind k){
switch (k){
	case TOK_IDENTIFIER:
		return "IDENTIFIER";break;	
	case TOK_CONSTANT:
		return "CONSTANT";break;
	case TOK_INT_KEYWORD:
		return "INT_KEYWORD";break;
	case TOK_VOID_KEYWORD: 
		return "VOID_KEYWORD";break;
	case TOK_RETURN_KEYWORD: 
		return "RETURN_KEYWORD";break;
	case TOK_OPEN_PAR: 
		return "OPEN_PAR";break;
	case TOK_CLOSE_PAR: 
		return "CLOSE_PAR";break;
	case TOK_OPEN_BRA: 
		return "OPEN_BRA";break;
	case TOK_CLOSE_BRA: 
		return "CLOSE_BRA";break;
	case TOK_SEMICOLON: 
		return "SEMICOLON";break;
	default: 
		return "NONE";
	}
}

struct token {
	enum kind kind;
	char *text;
};

char * read_file(const char *fileName){
	FILE *inputFile = fopen(fileName,"r+");
	if (inputFile == NULL){
		fprintf(stderr, "File [%s] cannot be opened: %s\n", fileName, strerror(errno));
		return NULL;
	}
	struct stat st;
	
	if (fstat(fileno(inputFile),&st) != 0){

		fprintf(stderr, "fstat() : File [%s] cannot be manipulated: %s\n", fileName, strerror(errno));
		fclose(inputFile);
		return NULL;
	}
	if(!S_ISREG(st.st_mode)){
		fprintf(stderr, "[%s] is not a file: %s\n", fileName, strerror(errno));
		fclose(inputFile);
		return NULL;
	}
	
	size_t size = st.st_size;
	char *buffer = malloc(size + 1);
	if(!buffer){
		perror("Erreur : malloc()");
		fclose(inputFile);
		return 0;
	}
	
	size_t n = fread(buffer,1,size,inputFile);
	fclose(inputFile);
	buffer[n] = '\0';
	return buffer;

}

void tokenize(const char *file){
	const char * p = file;
	while(*p != '\0'){
		if(isspace((unsigned char)*p)) {
			switch(*p){
				case '\n':printf("\\n: [%c]\n",*p);break;
				case ' ':printf("Space: [%c]\n",*p);break;
				default:printf("non identifié\n");break;
			}
		}
		else if(isdigit((unsigned char)*p)){
			printf("Digit : [");
			while(isdigit((unsigned char)*p)){
				printf("%c",*p);
				p++;
			}
			printf("]\n");
			continue;
		}
		else if(isalpha((unsigned char)*p) || *p == '_' ){
			printf("Word:[");
			while(isalpha((unsigned char)*p) || *p == '_'){
				printf("%c",*p);
				p++;
			}
			printf("]\n");
			continue;
			}
		else switch(*p){
			case'(':printf("( : [%c]\n",*p);break;

			case ')':printf(") : [%c]\n",*p);break;

			case '{':printf("{ : [%c]\n",*p);break;

			case '}':printf("} : [%c]\n",*p);break;

			case ';':printf("; : [%c]\n",*p);break;

			default:  printf("inconnu : '%c'\n", *p); break;

		} 
		p++;
	}

}



int main (int argc, char* argv[]){
	
	//----------------
	//-- 1. preprocess --
	//----------------
	//trunk the string to only have the namefile
	if (argc < 2) {
    fprintf(stderr, "Usage: %s [--lex|--parse|--codegen] <input>\n", argv[0]);
    return EXIT_FAILURE;
	}
	const char *input = NULL;
	const char *args = NULL;
	if( (strcmp(argv[1],"--lex") == 0) || (strcmp(argv[1],"--parse") == 0)  || (strcmp(argv[1],"--codegen") == 0)){
		input = argv[2];
		args = argv[1];
	}
	else 
		input = argv[1];          

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

	//----------------------------------------------------------
	//--							2. Compile				  --
	//-- lexer -> parser -> codegen -> emit (assembly-file)	  --
	//----------------------------------------------------------
	id = fork();
	if (id == -1) { perror("fork"); exit(1); }

	if (id == 0) {	
		//----------------------------------------------
		//---				2.1. Lexer				 ---
		//--- Transform a preproc.i into a token list --
		//----------------------------------------------
		//while input isn't empty:
		//if input starts with whitespace:
		//	trim whitespace from start of input
		//else:
		//	find longest match at start of input for any regex in Table 1-1
		//	if no match is found, raise an error
		//	convert matching substring into a token
		//	remove matching substring from start of input
		if( args && (strcmp(args,"--lex") == 0)){
			printf("\n--lex\n");
			printf("\npreproc %s \n", preproc_file);
			char *file = read_file(preproc_file);
			if(!file)_exit(1);

			tokenize(file);

			free(file);



		}

		//----------------------------------------------
		//---				2.2. Parser				 ---
		//--- Transform a token list into an AST	  --
		//----------------------------------------------
		if( args && (strcmp(args,"--parse") == 0)){
			printf("\n--parse\n");
		}

		//----------------------------------------------
		//---			2.3. Assembly Generation	 ---
		//--- Transform an AST to assembly (data structure not txt) --
		//----------------------------------------------
		if( args && (strcmp(args,"--codegen") == 0)){
			printf("\n--codegen\n");
		}


		//----------------------------------------------
		//---			2.4. Code emission ---
		//--- Transform an assembly to .s file --
		//----------------------------------------------
		execlp("gcc", "gcc", "-S",preproc_file, "-o","assembly-file.s", (char *)NULL);
		perror("execlp gcc -S");
		_exit(1);
	}
	
	waitpid(id, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Compilation failed\n");
		exit(1);
	}
	//remove(preproc_file);

	//--------------------------
	//-- 3. assemble and link --
	//--------------------------
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
