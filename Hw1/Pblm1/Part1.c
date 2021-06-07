#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <assert.h>

size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}


int main()
{

printf("Running Problem 1 Part A \n");
SHA512_CTX ctx;
unsigned char buffer[512];

   FILE *fp;//File pointer to get shadow_a
   int c;
   
   char str[500]; //declaring variables
   char salt1[10]="";
   char salt2[10]="";
   char base64[90];
   char base642[90];
   char root[10] = "";
   char adm[10] = "";
   fp = fopen("shadow_a","r");//opening shadow_a file

   int a =0;
   while(1) {

      c = fgetc(fp);
      if( feof(fp) ) { 
         break ;
      }
      str[a] = c;
      a++;
   }
   fclose(fp);
  int d;
  int e;
  for(d =0; d<8;d++){
  salt1[d] = str[d+8];
  salt2[d] = str[d+132];
  }
 
  for(e=0;e<86;e++){
  base64[e] = str[e+17];
  base642[e] = str[e+141];
  }

strcat(base642,"==");//concat == to decode password string
char* base64DecodeOutput;
size_t test;
Base64Decode(base642, &base64DecodeOutput, &test);//decode function base64 to ascii
//printf("Output: %s %d\n", base64DecodeOutput, test);
 
char src[] = "19000101"; //starting date
char salt[10];

strcpy(salt,salt2);

strcat(src,salt);

int len = strlen(src);
//initial SHA512 hash 
strcpy(buffer,src);
SHA512_Init(&ctx);
SHA512_Update(&ctx, buffer, len);
SHA512_Final(buffer, &ctx);

int res = strcmp(base64DecodeOutput,buffer);
//Comparing until a match is found with decoded string
while (strcmp(base64DecodeOutput,buffer) !=0){

if (src[7] > '9'){
	src[6]++;
	src[7] = '0';
}

if (src[5] > '9'){
	src[4]++;
	src[5] = '0';
}

if (src[3] > '9'){
	src[2]++;
	src[3] = '0';
	
}

if (src[2] > '9'){
	src[1]++;
	src[2] = '0';
}

if (src[1] > '9'){
	src[0]++;
	src[1] = '0';
}

if(src[6] == '3' && src[7] == '2'){
	src[5]++;
	src[6] = '0';
	src[7] = '1';
}

if(src[4] == '1' && src[5] == '3'){
	src[3]++;
	src[4] = '0';
	src[5] = '1';
	
}


int len = strlen(src);

strcpy(buffer,src);
//calculating sha512 hash for current input string
SHA512_Init(&ctx);
SHA512_Update(&ctx, buffer, len);
SHA512_Final(buffer, &ctx);

src[7]++;//increasing date by 1
if(src[0] == '2' && src[2] == '2' && src[3] == '1'){
	break;//break if nothing found till 2021
}

}
src[7]--;
int f;
for(f =0; f<8;f++){
  root[f] = src[f];//extracting admin password from string 
  }
printf("admin password : %s \n",root);

//doing the same thing for root password
strcat(base64,"==");

Base64Decode(base64, &base64DecodeOutput, &test);
//printf("Output: %s %d\n", base64DecodeOutput, test);
 
char src1[] = "19000101";

strcpy(src,src1);
strcpy(salt,salt1);

strcat(src,salt);

len = strlen(src);

strcpy(buffer,src);
SHA512_Init(&ctx);
SHA512_Update(&ctx, buffer, len);
SHA512_Final(buffer, &ctx);

res = strcmp(base64DecodeOutput,buffer);

while (strcmp(base64DecodeOutput,buffer) !=0){

if (src[7] > '9'){
	src[6]++;
	src[7] = '0';
}

if (src[5] > '9'){
	src[4]++;
	src[5] = '0';
}

if (src[3] > '9'){
	src[2]++;
	src[3] = '0';
	
}

if (src[2] > '9'){
	src[1]++;
	src[2] = '0';
}

if (src[1] > '9'){
	src[0]++;
	src[1] = '0';
}

if(src[6] == '3' && src[7] == '2'){
	src[5]++;
	src[6] = '0';
	src[7] = '1';
}

if(src[4] == '1' && src[5] == '3'){
	src[3]++;
	src[4] = '0';
	src[5] = '1';
	
}


int len = strlen(src);

strcpy(buffer,src);

SHA512_Init(&ctx);
SHA512_Update(&ctx, buffer, len);
SHA512_Final(buffer, &ctx);

src[7]++;
if(src[0] == '2' && src[2] == '2' && src[3] == '1'){
	break;
}

}
src[7]--;
int g;
for(g =0; g<8;g++){
  adm[g] = src[g];
  }
printf("root password : %s \n",adm);


return 0;
}
