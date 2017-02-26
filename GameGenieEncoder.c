//GameGenieEncoder. It Encodes Adress and value to its Equivalent GameGenie Code.
//Written by Haseeb Mir (haseebmir.hm@gmail.com)
//Dated : 26/02/2017

//Thanks to Emu Works for their GameGenie Encoder/Decoder Software under GNU GENERAL PUBLIC LICENSE.
//Software could be found here : http://games.technoplaza.net/ggencoder/qt/

//Including Standard Libraries.
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define byte unsigned char //We need only one byte for Value and Compare
#define word unsigned short //We need only two bytes for address.

//GameGenieTable list of GameGenie Characters.
static const char GameGenieTable[] = {
    'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y',
    'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N'
};

//Variable for Storing Inputs.
word address = 0x0; //2 bytes for range (8000-FFFF)  
byte value = 0x0; //1 byte for range (00-FF)
byte compare = 0x0; //1 byte for range (00-FF)
_Bool _compareBool = false; //Bool To check if input has Compare present or not.

//Prototype for Encoding Input to GameGenieCode.
word getAddress();
byte getValue();
char* encode_input(word,byte,byte);

//Prototype For Compare values. 
_Bool hasCompare();
byte getCompare();

//For Clearing Input buffer when there is Buffer OverFlow.
void clearInputBuffer();
 
int main(){
 
 //Reading Input.
	printf("Enter Adress : (Range : 8000-FFFF).\n");
	scanf("%x",&address);

 //Clears Input buffer for Next Input. Only True In case of Buffer OverFlow.
  clearInputBuffer();
 
	printf("\nEnter value : (Range : 00-FF).\n");
	scanf("%x",&value);

 //Reading Optional Compare input for 8-char GameGenie Code.
	printf("\nEnter Compare value : (Range : 00-FF)  (Optional) if you want to Skip Write \\n\n");
 
 //Clears Input buffer for Next Input. Only True In case of Buffer OverFlow.
  clearInputBuffer();
  
	//If we have read atleast One Input be it Zero even.
	if(scanf("%x",&compare) > 0) 
	  _compareBool = true; //set Bool to true.
	 
 if(compare == '\n') //If user Skipped Compare.
 	_compareBool = false; //Then Set _compareBool to False.
   
 //Prints Equivalent GameGenie Code.
	printf("\nGame Genie Code is : %s\n",strrev(encode_input(getAddress(),getValue(),getCompare())));

return 0; 
}

//Clears Input buffer for Next Input.
void clearInputBuffer(){
	 //Same as what fflush(stdin) does, but atleast it doesnt have undefined behaiour. :p
 char ch; 
	
	while((ch = getchar()) != '\n' && ch != EOF);//Dont override the input buffer with previos overflow value . 
 //Simply Wait till EOF or '\n' occurs.

}

//Checking if input has Compare value. 
_Bool hasCompare(){ 
	return (_compareBool != false); 
	}	

//Get Compare Value within 1 Byte Range. (0xFF).
byte getCompare(){ 
 compare =  (compare & 0xFF); //Masking Invalid Compare value Range to its Range.
 return compare;
	}

//Get Adress within ROM Adress Range (0x8000 - 0xFFFF). 
word getAddress(){ 
 address = (address & 0xFFFF); //Masking Invalid Address to its Range.
 return address; 
	}

//Get Value within 1 Byte Range. (0xFF).
byte getValue(){ 
 value =  (value & 0xFF); //Masking Invalid value to its Range.
 return value; 
}


//Encoding Input to GameGenieCode.        
char* encode_input(word	 inputAddress,byte inputValue,byte inputCompare) {
    int genie;
    int temp = 0;
    
    byte value = inputValue;
    word address = inputAddress;
    
    // position 1678
    genie = ((value & 0x80) >> 4) | (value & 0x7);
    
    // position H234
    temp = ((address & 0x80) >> 4) | ((value & 0x70) >> 4);
    genie <<= 4;
    genie |= temp;
    
    // position -IJK
    temp = (address & 0x70) >> 4;
    
    if (hasCompare()) {
        temp |= 0x8;
    }
    
    genie <<= 4;
    genie |= temp;
    
    // position LABC
    temp = (address & 0x8) | ((address & 0x7000) >> 12);
    genie <<= 4;
    genie |= temp;
    
    // position DMNO
    temp = ((address & 0x800) >> 8) | (address & 0x7);
    genie <<= 4;
    genie |= temp;
    
    if (hasCompare()) {
        byte compare = inputCompare;
        
        // position eEFG
        temp = (compare & 0x8) | ((address & 0x700) >> 8);
        genie <<= 4;
        genie |= temp;
        
        // position afgh
        temp = ((compare & 0x80) >> 4) | (compare & 0x7);
        genie <<= 4;
        genie |= temp;
        
        // position 5bcd
        temp = (value & 0x8) | ((compare & 0x70) >> 4);
        genie <<= 4;
        genie |= temp;
    } else {
        // position 5EFG
        temp = (value & 0x8) | ((address & 0x700) >> 8);
        genie <<= 4;
        genie |= temp;
    }
    
    
					//Contains Encoded GameGenie Code.
					static char GameGenieCode[8] = {0x0}; //We are returning this so it has to be static.
			 
			 //Generates Equivalent GameGenieCode from Input.
				int index;
				for (index = 0; index <  (hasCompare() ? 8 : 6); index++)
        GameGenieCode[index] = GameGenieTable[(genie >> (index * 4)) & 0xF];
    
				//Return GameGenieCode Encoded from Input.       
    return GameGenieCode;
}        
        