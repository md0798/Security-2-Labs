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

printf("Running Problem 1 Part B \n");
FILE *fp;
 int c;
 
  FILE *fp2;
   int k;
  
char str[500];
char src[] = "AAAAAA";
char salt1[10]="";
char salt2[10]="";
char base64[90]="";
char base642[90]="";
   char alice[10] = "";
   char bob[10] = "";
char src2[10]="AAAAAA";
EVP_MD_CTX * mdctx;
unsigned char * digest;
unsigned int digest_len;
SHA512_CTX ctx;
unsigned char buffer[512];
mdctx = EVP_MD_CTX_create();

   fp2 = fopen("shadow_b","r");
   int p=0;
   while(1) {

      k = fgetc(fp2);
      if( feof(fp2) ) { 
         break ;
      }
      str[p] = k;
      p++;
   }
   fclose(fp2);
  int d;
  int e;
  for(d =0; d<8;d++){
  salt1[d] = str[d+9];
  salt2[d] = str[d+131];
  }
 
  for(e=0;e<86;e++){
  base64[e] = str[e+18];
  base642[e] = str[e+140];
  }



int a;
fp = fopen("word.txt","r");
strcat(base642,"==");
strcat(base64,"==");

char* base64DecodeOutput;
char* base64DecodeOutput2;
size_t test;
Base64Decode(base642, &base64DecodeOutput2, &test);
Base64Decode(base64, &base64DecodeOutput, &test);
strcat(src,salt2);
int len = strlen(src);

strcpy(buffer,src);
EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
EVP_DigestUpdate(mdctx, buffer, len);
digest_len = EVP_MD_size(EVP_sha3_512());

digest = (unsigned char *)OPENSSL_malloc(digest_len);
EVP_DigestFinal_ex(mdctx, digest, &digest_len);
int res = strcmp(base64DecodeOutput,digest);

int dec = 0;

while(strcmp(base64DecodeOutput2,digest) !=0 ){
char str[8]="";
for(a =0;a<7;a++){
      c = fgetc(fp);
      if( feof(fp) ) { 
         break ;
      }
      if (c == ' ' || c == '\n'){
      continue;
      }
      str[a] = c;
	
      }
      
     // printf("%s \n",str);
strcpy(src,str); 
//printf("%s \n",src);     
strcat(src,salt2);
for(int i=0; i<64;i++){
if(strcmp(base64DecodeOutput2,digest) ==0){
dec =1;
strcpy(src2,src);
break;
}
if(src[5]<'A'){
src[4] = src[4]-32;
src[5] = src[5] +64;
}
if(src[4]<'A'){
src[3] = src[3]-32;
src[4] = src[4] +64;
}
if(src[3]<'A'){
src[2] = src[2]-32;
src[3] = src[3] +64;
}
if(src[2]<'A'){
src[1] = src[1]-32;
src[2] = src[2] +64;
}
if(src[1]<'A'){
src[0] = src[0]-32;
src[1] = src[1] +64;
}
if(src[0]<'A'){
src[0] = src[0] +64;
}

if(dec==1){
break;
}
//printf("%s \n",src);

int len = strlen(src);

strcpy(buffer,src);
EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
EVP_DigestUpdate(mdctx, buffer, len);
digest_len = EVP_MD_size(EVP_sha3_512());
//printf("%s \n",src);
digest = (unsigned char *)OPENSSL_realloc(digest,digest_len);
EVP_DigestFinal_ex(mdctx, digest, &digest_len);


src[5] = src[5]-32;
}
//printf("%d",dec);
if( feof(fp) ) { 
         break ;
      }


}
src2[5] = src2[5]+32;

int g;
for(g =0; g<6;g++){
  bob[g] = src2[g];
  }

if (dec==1){
printf("Bob's Password: %s \n",bob);
}
strcpy(src,"AAAAAA");
strcat(src,salt2);
while(strcmp(base64DecodeOutput2,digest) !=0 && dec !=1){

if (src[5] > 'Z' && src[5]<'a'){
	src[5] = 'a';
}

if (src[5] > 'z'){
	src[4]++;
	src[5] = 'A';
}
if (src[4] > 'Z' && src[4]<'a'){
	src[4] = 'a';
}

if (src[4] > 'z'){
	src[3]++;
	src[4] = 'A';
}

if (src[3] > 'Z' && src[3]<'a'){
	src[3] = 'a';
}

if (src[3] > 'z'){
	src[2]++;
	src[3] = 'A';
}

if (src[2] > 'Z' && src[2]<'a'){
	src[2] = 'a';
}

if (src[2] > 'z'){
	src[1]++;
	src[2] = 'A';
}

if (src[1] > 'Z' && src[1]<'a'){
	src[1] = 'a';
}

if (src[1] > 'z'){
	src[0]++;
	src[1] = 'A';
}
if (src[0] >'z'){
break;
}
if(dec==1){
break;
}
//printf("%s \n",src);

int len = strlen(src);

strcpy(buffer,src);
EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
EVP_DigestUpdate(mdctx, buffer, len);
digest_len = EVP_MD_size(EVP_sha3_512());
digest = (unsigned char *)OPENSSL_realloc(digest,digest_len);
EVP_DigestFinal_ex(mdctx, digest, &digest_len);
src[5]++;
}
if(dec !=1){
src[5]--;
for(g =0; g<6;g++){
  bob[g] = src[g];
  }

printf("Bob's Password: %s \n",bob);
}



dec = 0;
strcpy(src,"AAAAAA");
while(strcmp(base64DecodeOutput,digest) !=0 ){
char str[8]="";
for(a =0;a<7;a++){
      c = fgetc(fp);
      if( feof(fp) ) { 
         break ;
      }
      if (c == ' ' || c == '\n'){
      continue;
      }
      str[a] = c;
	
      }
      
     // printf("%s \n",str);
strcpy(src,str); 
//printf("%s \n",src);     
strcat(src,salt1);
for(int i=0; i<64;i++){
if(strcmp(base64DecodeOutput,digest) ==0){
dec =1;
strcpy(src2,src);
break;
}
if(src[5]<'A'){
src[4] = src[4]-32;
src[5] = src[5] +64;
}
if(src[4]<'A'){
src[3] = src[3]-32;
src[4] = src[4] +64;
}
if(src[3]<'A'){
src[2] = src[2]-32;
src[3] = src[3] +64;
}
if(src[2]<'A'){
src[1] = src[1]-32;
src[2] = src[2] +64;
}
if(src[1]<'A'){
src[0] = src[0]-32;
src[1] = src[1] +64;
}
if(src[0]<'A'){
src[0] = src[0] +64;
}
//printf("%s \n",src);

int len = strlen(src);

strcpy(buffer,src);
EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
EVP_DigestUpdate(mdctx, buffer, len);
digest_len = EVP_MD_size(EVP_sha3_512());
//printf("%s \n",src);
digest = (unsigned char *)OPENSSL_realloc(digest,digest_len);
EVP_DigestFinal_ex(mdctx, digest, &digest_len);


src[5] = src[5]-32;
}
//printf("%d",dec);
if( feof(fp) ) { 
         break ;
      }


}
src2[5] = src2[5]+32;

for(g =0; g<6;g++){
  bob[g] = src2[g];
  }

if (dec==1){
printf("Alice's Password: %s \n",bob);
}
strcpy(src,"AAAAAA");
strcat(src,salt1);
while(strcmp(base64DecodeOutput,digest) !=0 && dec !=1){

if (src[5] > 'Z' && src[5]<'a'){
	src[5] = 'a';
}

if (src[5] > 'z'){
	src[4]++;
	src[5] = 'A';
}
if (src[4] > 'Z' && src[4]<'a'){
	src[4] = 'a';
}

if (src[4] > 'z'){
	src[3]++;
	src[4] = 'A';
}

if (src[3] > 'Z' && src[3]<'a'){
	src[3] = 'a';
}

if (src[3] > 'z'){
	src[2]++;
	src[3] = 'A';
}

if (src[2] > 'Z' && src[2]<'a'){
	src[2] = 'a';
}

if (src[2] > 'z'){
	src[1]++;
	src[2] = 'A';
}

if (src[1] > 'Z' && src[1]<'a'){
	src[1] = 'a';
}

if (src[1] > 'z'){
	src[0]++;
	src[1] = 'A';
}
if (src[0] >'z'){
break;
}
//printf("%s \n",src);

int len = strlen(src);

strcpy(buffer,src);
EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
EVP_DigestUpdate(mdctx, buffer, len);
digest_len = EVP_MD_size(EVP_sha3_512());
digest = (unsigned char *)OPENSSL_realloc(digest,digest_len);
EVP_DigestFinal_ex(mdctx, digest, &digest_len);
src[5]++;
}
if(dec !=1){
src[5]--;
for(g =0; g<6;g++){
  bob[g] = src[g];
  }

printf("Alice's Password: %s \n",bob);
}



return 0;
}
