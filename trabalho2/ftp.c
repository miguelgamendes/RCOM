#include <stdio>

int main(int argc, char** argv) {
	
	printf("Checking arguments...");

	if(argc < 2 || argc > 2){
		printf("Usage arguments: ftp ftp://[<user>:<password>@]<host>/<url-path>\n");
		exit(1);
	}

	printf("OK\n");

	printf("Detecting url...");

	if(sscanf(argv[1], "ftp://%[^:]:%[^@]@%[^/]/%s", user, password, host, urlpath) == 4) {
		printf("Full url detected.\n");
	} else if(sscanf(argv[1], "ftp://%[^/]/%s", host urlpath) == 2) {
		printf("Short url detected.\n");
	} else {
		printf("Something in your input was wrong =(\nTry to check your url, maybe?\n");
		exit(1);
	}
}
