#include "crypt.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

long long MAXN = 1 << 20;
long long lseed;


void rd(){ lseed  = (lseed * 9 + 7) % MAXN; }

unsigned char gxor(unsigned char xx, int ind, char nkey[]){
	return (xx ^ nkey[ind]);
}

void nk(char* fname, char* key, char nkey[]){
	unsigned char table[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char buff[512];
	char midk[512];
	int i, siz1, siz2, siz, j = 0;
	siz1 = strlen(fname);
	siz2 = strlen(key);
	for(i = 0; i < siz1 && i < siz2; i++){
		buff[j++] = fname[i];
		buff[j++] = key[i];
	}
	if(siz1 < siz2) for(; i < siz2; i++) buff[j++] = key[i];
	else for(; i < siz1; i++) buff[j++] = fname[i];
	buff[j] = '\0';
	j = 0;
	siz = strlen(buff);
	for(i = 0; i < siz * 2; i++){
		rd();
		midk[j++] = (unsigned char)buff[lseed % siz];
	}
	midk[j] = '\0';
	for(i = 0; i < 256; i++){
		rd();
		nkey[i] = table[(lseed + (int)midk[lseed % j]) % 62];
	}
	nkey[256] = '\0';
	printf("fname:%s\nkey:%s\nbuff:%s\nmidk:%s\nnkey:%s\n",fname, key, buff, midk, nkey);
	return;
}

void crypt(char* id, char* key, char* fname, char* typ){
	int kn, rc, i, dt, x;
	long long cnt = 0;
	double KB, MB, GB;
	FILE * fin;
	FILE * fout;
	char fkey[512];
	unsigned char x0, x1, x2;
	unsigned char buff[2048];
	char oname[2048];
	time_t start, end;
        
        strcpy(oname, fname);
        x = strlen(fname);	
	if(atoi(typ) == 0){ oname[x] = 's'; oname[x+1] = '\0';}
	else if(atoi(typ) == 1){ oname[x - 1] = '\0'; }
        else return;

	if(strcmp(fname, oname) == 0){
		printf("ERROR: In and Out Same");
		return;
	}

	printf("In : %s\n",fname);
	printf("Out: %s\n",oname);

	fin = fopen(fname,"rb");
	fout = fopen(oname,"wb");

	lseed = (long long)atoi(id);
	if(atoi(typ) == 0) nk(fname, key, fkey);
	else if(atoi(typ) == 1) nk(oname, key, fkey);
	else return;
	kn = strlen(fkey);
	
	if (!fin || !fout){ printf("FILE OPEN ERROR!\n"); return; }

	start = time(NULL);
	while((rc = fread(buff, sizeof(unsigned char), 512, fin)) != 0 ){  
		cnt += rc;
		for(i = 0; i < rc; i++){
			//x0 = buff[i];
			rd();
			//x1 = gxor(x0, lseed % kn, fkey);
			buff[i] ^= fkey[lseed % kn];
			//x2 = gxor(x1, lseed % kn, fkey);
			//buff[i] = x1;
			//if(x0 != x2) printf("ERR : %x %x %x\n",x0,x1,x2);
		}
		fwrite(buff, sizeof(unsigned char), rc, fout);  
	}
	end = time(NULL);
	dt = (int)difftime(end, start);
	KB = (double)cnt / 1024;
	MB = KB / 1024;
	GB = MB / 1024;
	printf("Handle %lf KB or %lf MB or %lf GB data.\n", KB, MB, GB);
	printf("Cost %d s.\n", dt);
	printf("Speed %lf MB/s\n",MB / dt);
	return;
}

int main(int argc, char** argv){
	if(argc != 5) return 0;
	crypt(argv[1], argv[2], argv[3], argv[4]);	
	return 0;
}
