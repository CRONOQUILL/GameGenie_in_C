//NES GameGenieEncoder. It Encodes Address and value to its Equivalent GameGenie Code.
//Written by Haseeb Mir (haseebmir.hm@gmail.com)
//Dated : 26/02/2017

//Thanks to Emu Works for their GameGenie Encoder/Decoder Software under GNU GENERAL PUBLIC LICENSE.
//Software could be found here : http://games.technoplaza.net/ggencoder/qt/

//Including Standard Libraries.
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<windows.h> //WinAPI for GUI.

#define byte unsigned char //We need only one byte for Value and Compare
#define word unsigned short //We need only two bytes for address.

//Contains Encoded GameGenie Code.
static char GameGenieCode[8] = {0x0};
			 
//GameGenieTable list of GameGenie Characters.
static const char GameGenieTable[] = {
    'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y',
    'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N'
};

//Defining Constant for Converting Hex to Decimal.
	static const long hextable[] = {
   [0 ... 255] = -1, // bit aligned access into this table is considerably
   ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // faster for most modern processors,
   ['A'] = 10, 11, 12, 13, 14, 15,       // for the space conscious, reduce to
   ['a'] = 10, 11, 12, 13, 14, 15        // signed char.
};

//Table for Valid Hex characters.
static const char ValidHexTable[] = "0123456789abcdefABCDEF";

//Variables for Storing Inputs.
char* _address = NULL; 
char* _value = NULL;
char* _compare = NULL;
_Bool _compareBool = false; //Bool To check if input has Compare present or not.

//Prototype for Encoding Input to GameGenieCode.
word getAddress();
byte getValue();
char* encode_input(word,byte,byte);

//Prototype For Compare values. 
_Bool hasCompare();
byte getCompare();

//Checking for Invalid Input.
_Bool isInvalidInput();

//Convert Hex input to decimal and checking valid Hex.
int hexToDec(unsigned const char *hex);
_Bool isValidHex();
	
//Clears All TextFields on Error.
void clearTextFields(HWND,HWND,HWND,HWND);

//Reset All Inputs.
void resetInput();

//Window Management procedure.
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//For Creating GUI Windows.
HWND _createGUIWindow(LPSTR,int,int,int,int,HWND);
LRESULT _setGUIFont(HWND,int);

//Storing Length of Inputs.
short Compare_Len = 0,Address_Len = 0,Value_Len = 0;

//Main Function of WinAPI for Handlin GUI.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    //WinAPI Declarations.
    WNDCLASSEX wc; 
    HWND hwnd; 
    MSG Msg;
    memset(&wc,0,sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(24, 185, 158));
    wc.lpszClassName = "WindowClass";
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","NES GameGenie Encoder",WS_VISIBLE|WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        320,
        280,
        NULL,NULL,hInstance,NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg); 
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

//For Handling All Windows Created in GUI.
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

//Declaring Window Handlers.
static HWND GameGenieOutput; //Generated GameGenie Output.
HWND GG_Label,Address_Label,Value_Label,Compare_Label,info_Label; //Handler For Labels
static HWND Address_hwnd,Value_hwnd,Compare_hwnd; //Handler for Text Fields.
HWND encode_button; //Handler for Encode Button.

//Variable for Storing Inputs.
_address = (char*)malloc((sizeof(word) * 2) + 1); //4 bytes for 4 Hex Chars. (0xFFFF) 
_value = (char*)malloc((sizeof(byte)* 2) + 1); //2 byte for 2 Hex Chars (0xFF)
_compare = (char*)malloc((sizeof(byte)* 2) + 1); //2 byte for 2 Hex Chars (0xFF)

    switch(Message) {
        case WM_CREATE:
																
								////////////////////////
								//Creating Labels Window.
								///////////////////////
																
   			    			
   				//Creating Value Label.                  
        Value_Label = _createGUIWindow("static",0,10,65,20,hwnd); 
								SetWindowText(Value_Label, "Value");                       
								_setGUIFont(Value_Label,SYSTEM_FIXED_FONT);
   			 
							//Creating Address Label.                  
        Address_Label = _createGUIWindow("static",0,50,65,20,hwnd); 
								SetWindowText(Address_Label, "Address");                       
								_setGUIFont(Address_Label,SYSTEM_FIXED_FONT);
   			
      	//Creating Compare Label.                  
        Compare_Label = _createGUIWindow("static",0,90,65,20,hwnd); 
								SetWindowText(Compare_Label, "Compare");                       
								_setGUIFont(Compare_Label,SYSTEM_FIXED_FONT);

								//Creating GameGenie Label.                  
        GG_Label = _createGUIWindow("static",0,180,120,20,hwnd); 
								SetWindowText(GG_Label, "GameGenieCode");                       
								_setGUIFont(GG_Label,SYSTEM_FIXED_FONT);
						   
						   
											///////////////////////////
										//Creating Input Window.///
										//////////////////////////
  							
												
									//Creating Value Window for Input.
									Value_hwnd = _createGUIWindow("edit",100,10,50,20,hwnd);
									
									//Creating Address Window for Input.
									Address_hwnd = _createGUIWindow("edit",100,50,50,20,hwnd); 
									
									//Creating Compare Window for Input.
									Compare_hwnd = _createGUIWindow("edit",100,90,50,20,hwnd);
						   
						   
						   
									//Creating Window for GameGenieOutput.
									GameGenieOutput = _createGUIWindow("edit",140,180,100,20,hwnd); 
									
					         
        //Creating encode_button Window.                              
        encode_button = _createGUIWindow("BUTTON",100, 140, 70, 20,hwnd);                       
								_setGUIFont(encode_button,OEM_FIXED_FONT);
                   																																                               
								//Creating Info.
								info_Label = _createGUIWindow("static",0,227,320,16,hwnd); 
								SetWindowText(info_Label, "\tWritten by HaseeB Mir");                       
								_setGUIFont(info_Label,OEM_FIXED_FONT);
								

        break;

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
                case IDOK:
										
										
										//Retrieving Input Address and its length.
										GetWindowText(Address_hwnd,_address,(sizeof(word) * 2) + 1); 
										Address_Len = GetWindowTextLength(Address_hwnd);
										 
										//Retrieving Input Value and its length. 
          GetWindowText(Value_hwnd,_value,(sizeof(byte)* 2)  + 1);
          Value_Len = GetWindowTextLength(Value_hwnd);
          
          //Retrieving Input Compare and its length.
          GetWindowText(Compare_hwnd,_compare,(sizeof(byte)* 2) + 1);
          Compare_Len = GetWindowTextLength(Compare_hwnd);
                  
		
		 //Check for Invalid Input. 
   if(isInvalidInput()){
   	
				   //Clear TextFields on Error.
   	    clearTextFields(GameGenieOutput,Address_hwnd,Value_hwnd,Compare_hwnd);
							
							 MessageBox(NULL, "Invalid Input Entered", "Input Error...",MB_ICONERROR);
							return 1;
						}
 																					 
	    
						//If we have read atleast One Input be it Zero even.
						(Compare_Len > 0) ? (_compareBool = true) //set to true.
					 : (_compareBool = false); //Or Set to false.
										
					//Writing GameGenie Code as Output.				
	  		SetWindowText(GameGenieOutput,strrev(encode_input(getAddress(),getValue(),getCompare()) ) ); 
					
					//Clear and Reset Inputs.
					clearTextFields(NULL,Address_hwnd,Value_hwnd,Compare_hwnd);
					resetInput();
						                             																                 
                break;
            }

        break;

        case WM_DESTROY: { 
								
								//Release the memory on Exit
								free(_value);
								free(_address);
								free(_compare);
								
								PostQuitMessage(0);
								break;
								}
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

//Converting Hex input to its equivalent Decimal.
int hexToDec(unsigned const char *hex) {
   int ret = 0; 
   while (*hex && ret >= 0) {
      ret = (ret << 4) | hextable[*hex++];
   }
   return ret; 
}


//Checking if input has Compare value. 
_Bool hasCompare(){ 
	return (_compareBool != false); 
	}	

//Get Compare Value within 1 Byte Range. (0xFF).
byte getCompare(){ 	 
	return (byte)hexToDec(_compare);
	}

//Get Address within ROM Address Range (0x8000 - 0xFFFF). 
word getAddress(){  
	return (word)hexToDec(_address);
	}

//Get Value within 1 Byte Range. (0xFF).
byte getValue(){  
 return (byte)hexToDec(_value);
}

//Checking for Invalid Input.
_Bool isInvalidInput(){
	
		return
		(
				
		//Checking for Null Input.
		(Compare_Len == 0x0 && 
		Value_Len == 0x0 &&
		Address_Len == 0x0)
		
		//Check for Invalid Address Range.
		||(hexToDec(_address) < 0x8000) 
		
		//Checking Invalid Input length.
		||(Compare_Len > 2)  //Max Length Compare is 2 i.e 0xFF
  ||(Value_Len > 2) //Max Length Value is 2 i.e 0xFF
 	|| (Address_Len > 4) // MAX Length of Adress range is 4. i.e 0xFFFF
		
		//Checking for InValid Hex.
		|| !isValidHex()) 
		
		? true : false; 
 
}

//Checking if Input is in Hex Range.
_Bool isValidHex(){
	
	return 
	( 
		(!_address[strspn(_address, ValidHexTable)]) && 
		(!_value[strspn(_value, ValidHexTable)]) &&
		(!_compare[strspn(_compare, ValidHexTable)])
	) 
		? true : false;
}


//Encoding Input to GameGenieCode.        
char* encode_input(word	inputAddress,byte	 inputValue,byte	inputCompare) {
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
  
		
		  					//Creating Info.
			  
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

			 //Generates Equivalent GameGenieCode from Input.
				int index;
				for (index = 0; index <  (_compareBool ? 8 : 6); index++)
        GameGenieCode[index] = GameGenieTable[(genie >> (index * 4)) & 0xF];
    
				//Return Encoded GameGenie Code.       
    return GameGenieCode;
} 


//Creating GUI Components like Labels,TextFields and Buttons.
HWND _createGUIWindow(LPSTR GUI_Type,int x,int y,int w,int h,HWND _hwnd){
	
	return (!strcmp(GUI_Type,"BUTTON")) 
	
	//Button got a style :p
	? CreateWindow(GUI_Type,"Encode",WS_VISIBLE | WS_CHILD | WS_BORDER,
 x, y, w, h,_hwnd, (HMENU) 1, NULL, NULL) 
 
	//For Labels and TextFields.
	: CreateWindow(GUI_Type,"",WS_BORDER | WS_CHILD | WS_VISIBLE,
 x,y,w,h,_hwnd, NULL, NULL, NULL);

}

//Setting the Fonts to GUI Fields.
LRESULT _setGUIFont(HWND _hwnd,int FontType){
 return SendMessage(_hwnd, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(FontType), (LPARAM)TRUE);
}         

//Clears All the TextFields.
void clearTextFields(HWND _output,HWND _addr,HWND _val,HWND _comp){
	
	//Set All the TextFields to NULL.
 SetWindowText(_output,"\0");
 SetWindowText(_addr,"\0");
 SetWindowText(_val,"\0");
 SetWindowText(_comp,"\0");	
}

//Reset Input Back to its original states.
void resetInput(){
	 
		_compareBool = false;
			Compare_Len = 0;
			Address_Len = 0;
			Value_Len = 0;
			
			//Setting GameGenieCode Back to NULL.
			memset(GameGenieCode,'\0',sizeof(GameGenieCode));
			  
}
        