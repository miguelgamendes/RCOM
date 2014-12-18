#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	
	char username[20], password[20], host[20], urlpath[100];

	printf("Checking arguments...");

	if(argc < 2 || argc > 2){
		printf("Usage arguments: ftp ftp://[<user>:<password>@]<host>/<url-path>\n");
		exit(1);
	}

	printf("OK\n");

	printf("Detecting url...");

	if(sscanf(argv[1], "ftp://%[^:]:%[^@]@%[^/]/%s", username, password, host, urlpath) == 4) {
		printf("Full url detected.\n");
	} else if(sscanf(argv[1], "ftp://%[^/]/%s", host urlpath) == 2) {
		printf("Short url detected.\n");
		username = "anonymous";
		password = "anonymous";
	} else {
		printf("Something in your input was wrong =(\nTry to check your url, maybe?\n");
		exit(1);
	}
}
