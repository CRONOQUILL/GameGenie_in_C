//Game Genie Decoder . Decodes 6 or 8 GameGenie code to its equivalent Address and value.
//Written by HaseeB Mir (haseebmir.hm@gmail.com)
//Dated : 22/02/2017
//Refrence by The Mighty Mike Master : http://tuxnes.sourceforge.net/gamegenie.html
//Thanks to Mighty Mike for his amazing refrence.

//Including Standard Libraries.
#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>

//Defining Length Constants.
#define MAX_GAME_GENIE_LEN 8 //Defining Max Length of GameGenie.
#define GAME_GENIE_TABLE_LEN 16 //MAX GameGenie Table Length.

//GameGenieCode subroutines.
void setGameGenieTable();
int checkGameGenieCode(char *);
int decodeGameGenieCode(char *);
char* readGameGenieCode();
int getGameGenieLen();

//subroutines for Address,value and Compare.
int decodeAddress();
int decodeValue();
int decodeCompare();

//Checking Invalid Input.
int isInvalidInput();

//Converting Input to UpperCase if needed.
_Bool isInputLower(char	*str);
void toUpperCase(char*);

//GameGenie Code Table.
char GameGenieTable[GAME_GENIE_TABLE_LEN] = {
						'A','P','Z','L','G','I','T','Y',
						'E','O','X','U','K','S','V','N',
						}; //Master GameGenie Table.

short n0,n1,n2,n3,n4,n5,n6,n7; //Contains HEX of GameGenieCode.  
_Bool is_8_Char_GameGenie = false; //Checking for 8-Char GameGenie Code.

char *GameGenieCode; //Contains 6 or 8 Character GameGenieCode.	

int main(void){
	
 printf("Enter Game Genie Code\n");
 GameGenieCode = readGameGenieCode(); //Reads 6 or 8 Character GameGenieCode..
 
 //If Input is in lower Case convert it to Upper case.
 if(isInputLower(GameGenieCode))
  toUpperCase(GameGenieCode);
  
 //Checking for 8-Character GameGenieCode.
	is_8_Char_GameGenie = (getGameGenieLen() == MAX_GAME_GENIE_LEN) ? true : false;
	 
 if(decodeGameGenieCode(GameGenieCode)){
	 printf("Address = 0x%x\n",decodeAddress()); //Prints Address in Hex.
	 printf("value = %x\n",decodeValue()); //Prints Value in Hex.
 
   if(is_8_Char_GameGenie) //If its 8-Character GameGenie then decode Compare value also.
  	 printf("Compare = %x\n",decodeCompare());//Prints Compare Value in Hex.	
	
	}

 else
 printf("Not a Valid Game Genie Code\n\n");

 free(GameGenieCode);//Free the Reserved Memory.
return 0;
}

//Reads GameGenieCode from Input limiting to only 8-char Input.
char *readGameGenieCode() {
    char *buf, *tmp_p;
        int i, c;
 
        i = 0;
        buf = (char*)malloc(sizeof(char));
        if (buf == NULL)
                return NULL;
 
        c = fgetc(stdin);
 							
 							int char_count = 1; //Counter to check if Input is greater than MAX_GAME_GENIE_LEN.
 							
 							//Read input one by one untill Newline or EOF Occurs.
 							//Dont read input after 8 char_count.
 							
        while (	char_count	> 0 && char_count	<= MAX_GAME_GENIE_LEN && c != '\n' && c != EOF) {
 							
                buf[i++] = c;
                tmp_p = buf;
                buf = (char*)realloc(buf, (i + 1) * sizeof(char));
                if (buf == NULL) {
                        free(tmp_p);
                        return NULL;
                }
                c = fgetc(stdin);
                char_count++; //Count Every Char untill value will become equal to MAX_GAME_GENIE_LEN.
        }
 
        if (i == 0) {
                free(buf);
                return NULL;
        }
 
        buf[i] = '\0';
        return buf;
}

_Bool isInputLower(char	*str){
	
	int index = 0;
	
	for(index = 0; index < strlen(str); index++){
		if(islower(str[index]))
		 return true;
	}
		
		return false;
}

//Converts the Input to Capital case.
void toUpperCase(char *str) 
{
 
    int index = 0;
    char smallCase; //Hold small cases to compare

    while(str[index])
    {
        //Check for All small characters.
        for(smallCase = 'a'; smallCase <= 'z'; smallCase++)

            if(str[index] == smallCase)
                str[index] -= 32; // 32 is difference between Capital and Small character's ASCII.

        index++;
    }

}

//Get Length of GameGenie Code.
int getGameGenieLen(){
	return strlen(GameGenieCode);
}

//Checking Invalid Input.
int isInvalidInput(){
	
	if(getGameGenieLen() < 6) //Dont check for > 8 because we has already set Input taking limit to 8 in readGameGenieCode() function.
	  return 1; //Return True if its invalid Input.
 	 
 return 0;	 //Otherwise return false.
}

//Decodes Value from GameGenie Code.
int decodeAddress(){
	
 int address = 0x8000 + 
              ((n3 & 7) << 12)
            | ((n5 & 7) << 8) | ((n4 & 8) << 8)
            | ((n2 & 7) << 4) | ((n1 & 8) << 4)
            |  (n4 & 7)       |  (n3 & 8);
            
 return address;

}

//Decodes Value from 6 or 8-Char GameGenie Code.
int decodeValue(){
	
 int value;
	
	//Checking if it's 8-Char_GameGenie Code.
	(is_8_Char_GameGenie == true) 
	
 //Value of 8-Char GameGenie.
	? (value =
             ((n1 & 7) << 4) | ((n0 & 8) << 4)
            | (n0 & 7)       |  (n7 & 8)) 
	
		//Value of 6-Char GameGenie.
	: (value =
             ((n1 & 7) << 4) | ((n0 & 8) << 4)
            | (n0 & 7)       |  (n5 & 8) );
 
 
 return value;
}


//Decodes Compare Value for 8-Char GameGenie Code.
int decodeCompare(){
	
	      int  compare =
             ((n7 & 7) << 4) | ((n6 & 8) << 4)
            | (n6 & 7)       |  (n5 & 8);
      return compare;      
	
}

//Decodes GameGenie chars to its equivalent Hex.
int decodeGameGenieCode(char *GameGenieCode){

 //Check for Invalid Input. 
   if(isInvalidInput())
				return 0; //Return False for Invalid Input.
				
int i = 0,j = 0,found = 0;
int GameGenieLen = getGameGenieLen();

 for(i = 0; i < 	GameGenieLen	; i++){
 	 for(j = 0; j < GAME_GENIE_TABLE_LEN; j++)
	 if(GameGenieCode[i] == GameGenieTable[j]){
	 found ++;
	 
	 //Twirling Ternary operators to convert Game Genie to its equivalent Hex.
		(found == 1) ? n0 = j : (found == 2) ? n1 = j : 
		(found == 3) ? n2 = j : (found == 4) ? n3 = j : 
		(found == 5) ? n4 = j : (found == 6) ? n5 = j : j;
		
		
		(is_8_Char_GameGenie) ? (found == 7 ? n6 = j : found == 8 ? n7 = j : j) : j;  	 
	 }
 }
 
 //Checking for Invalid Genie Codes. 
 return (found == GameGenieLen) ? 1 : 0;
} 
