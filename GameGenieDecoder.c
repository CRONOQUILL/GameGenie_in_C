//Game Genie Decoder 
//Written by HaseeB Mir (haseebmir.hm@gmail.com)
//Dated : 22/02/2017
//Refrence by The Mighty Mike Master : http://tuxnes.sourceforge.net/gamegenie.html
//Thanks to Mighty Mike for his amazing refrence.

#include<stdio.h>

//Setting prototypes.
void setGameGenieTable();
int checkGameGenieCode(char *);
int decodeGameGenieCode(char *);
int decodeAdress();
int decodeData();

//GameGenie Code Table.
char GameGenieTable[16] = {
						'A','P','Z','L','G','I','T','Y',
						'E','O','X','U','K','S','V','N',
						}; //Master GameGenie Table.

short n0,n1,n2,n3,n4,n5; //Contains HEX of GameGenieCode.  

int main(void){
	
char GameGenieCode[6] = {0x0};	

printf("Enter Game Genie Code (Max 6 char.)\n");
scanf("%[^\n]s",GameGenieCode);

 if(decodeGameGenieCode(GameGenieCode)){
	 printf("Adress = 0x%x\n",decodeAdress()); //Prints Adress in Hex.
	 printf("Data = %x\n",decodeData()); //Prints Data in Hex.
 }

else
printf("Not a Valid Game Genie Code\n\n");

return 0;
}


int decodeAdress(){
	
 int address = 0x8000 + 
              ((n3 & 7) << 12)
            | ((n5 & 7) << 8) | ((n4 & 8) << 8)
            | ((n2 & 7) << 4) | ((n1 & 8) << 4)
            |  (n4 & 7)       |  (n3 & 8);
            
 return address;

}

int decodeData(){
	
 int data =
             ((n1 & 7) << 4) | ((n0 & 8) << 4)
            | (n0 & 7)       |  (n5 & 8);

 return data;
}

int decodeGameGenieCode(char *GameGenieCode){

int i = 0,j = 0,found = 0;

 for(i = 0; i < 6 ; i++){
 	 for(j = 0; j < 16; j++)
	 if(GameGenieCode[i] == GameGenieTable[j]){
	 found ++;
	 
	 //Twirling Ternary operators to convert Game Genie to its equivalent Hex.
		(found == 1) ? n0 = j : (found == 2) ? n1 = j : 
		(found == 3) ? n2 = j : (found == 4) ? n3 = j : 
		(found == 5) ? n4 = j : (found == 6) ? n5 = j : j;  	 
	 }
 }
 
 //Checking for Invalid Genie Codes. 
 return (found == 6) ? 1 : 0;
} 