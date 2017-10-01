#include<stdio.h>
#include<string.h>
#include<stdlib.h>

unsigned char table[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
int main(int argc,char ** argv){
	int i;
	int seed;
	int newseed;
	int key_num;
	unsigned char key[1024];
	unsigned char newkey[1024];
	if(argc != 3) return 0;
	seed = atoi(argv[1])%65536;
	srand(seed);
	strncpy(key,argv[2],1024);
	key_num = strlen(key);
	printf("seed:%d\nkey:%s\n",seed,key);
	for(i=0;i<256;i++){
		newseed = (((((newseed * rand())%RAND_MAX)*seed)%RAND_MAX) + key[rand()%key_num] * key_num )%RAND_MAX;
		newkey[i] = table[((unsigned char)(newseed+key[newseed%key_num])%(62))];
	}
	newkey[256] = '\0';
	printf("%d %s\n",newseed,newkey);

		

}
