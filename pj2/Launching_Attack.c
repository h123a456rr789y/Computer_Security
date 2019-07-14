#include <stdio.h>
#include <stdlib.h>
int main(){
	while(1){
		system("ping -c 1 -s 1500 127.0.0.1 > /dev/null 2>&1");
	}

	return 0;
}
