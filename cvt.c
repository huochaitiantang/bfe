#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAXLEN 1024

unsigned char key[MAXLEN];
int key_num;
unsigned char get_crypto(unsigned char xx,int ind);
int main(int argc, char** argv){
	int i;
	FILE * fin;
	FILE * fout;
	unsigned char buf[MAXLEN]; 
        unsigned char x0,x1,x2;	
	int rc;
	int ind;
	int seed;
	if(argc != 5){
		printf("ERROR: argc should be 5 : mycvt seed key input_file output_file!\n");
		return 0;
	}
	for(i=0;i<5;i++){
		printf("arg %d:%s\n",i,argv[i]);
	}
	seed = atoi(argv[1])%65536;
	srand(seed);
	strncpy(key,argv[2],MAXLEN);
	key_num = strlen(key);
	printf("seed:%d\nkey:%s\n",seed,key);
	printf("In File: %s\n",argv[3]);
	printf("Out File: %s\n",argv[4]);
	fin = fopen(argv[3],"rb");
	fout = fopen(argv[4],"wb");
	if (!fin || !fout){
		printf("FILE OPEN ERROR!\n");
		return 0;
	}
	while((rc = fread(buf, sizeof(unsigned char), MAXLEN, fin)) != 0 ){  
		//printf("%d ",ind);
		for(i=0;i<rc;i++){
			x0 = buf[i];
			ind = rand();
			x1 = get_crypto(x0,ind);
			x2 = get_crypto(x1,ind);
			//x1 = ~x0;
			//x2 = ~x1;
			buf[i] = x1;
			if(x0 != x2){
				printf("ERR : %x %x %x\n",x0,x1,x2);
			}
			
		}
		fwrite( buf, sizeof( unsigned char ), rc, fout );  
	}  
	return 0;	
}

unsigned char get_crypto(unsigned char xx,int ind){
	unsigned char dd;
	dd = ind % key_num;
	return (xx ^ key[dd]);
}
