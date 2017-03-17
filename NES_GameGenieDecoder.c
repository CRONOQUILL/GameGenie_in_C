//NES Game Genie Decoder . Decodes 6 or 8 GameGenie code to its equivalent Address and value.
//Written by HaseeB Mir (haseebmir.hm@gmail.com)
//Dated : 22/02/2017
//Refrence by The Mighty Mike Master : http://tuxnes.sourceforge.net/gamegenie.html
//Thanks to Mighty Mike for his amazing refrence.

//Including Standard Libraries.
#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<windows.h> //WinAPI for GUI.

//Defining Length Constants.
#define MAX_GAME_GENIE_LEN 8 //Defining Max Length of GameGenie.
#define GAME_GENIE_TABLE_LEN 16 //MAX GameGenie Table Length.

//GameGenieCode subroutines.
void setGameGenieTable();
int checkGameGenieCode(char *);
int decodeGameGenieCode(char *);
char* readGameGenieCode();
short getGameGenieLen();

//subroutines for Address,value and Compare.
char* decodeAddress();
char* decodeValue();
char* decodeCompare();

//Checking Invalid Input.
int isInvalidInput();

//Converting Input to UpperCase if needed.
_Bool isInputLower(char	*str);
char* toUpperCase(char*);

//Clears All TextFields on Error.
void clearTextFields(HWND,HWND,HWND,HWND);

//Window Management procedure.
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//For Creating GUI Windows.
HWND _createGUIWindow(LPSTR,int,int,int,int,HWND);
LRESULT _setGUIFont(HWND,int);


//GameGenie Code Table.
char GameGenieTable[GAME_GENIE_TABLE_LEN] = {
						'A','P','Z','L','G','I','T','Y',
						'E','O','X','U','K','S','V','N',
						}; //Master GameGenie Table.

short n0,n1,n2,n3,n4,n5,n6,n7; //Contains HEX of GameGenieCode.  
_Bool is_8_Char_GameGenie = false; //Checking for 8-Char GameGenie Code.
short GG_Len = 0;//Contains length of Input Code Entered.

char GameGenieCode[MAX_GAME_GENIE_LEN + 1] = {'\0'}; //Contains 6 or 8 Character GameGenieCode.	

//Main Function for Handlin GUI.
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
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","NES GameGenie Decoder",WS_VISIBLE|WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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

//Declaring WIndow Handlers.
static HWND GameGenieInput; //Input field has to be static.
HWND GG_Label,Address_Label,Value_Label,Compare_Label,info_Label; //Handler For Labels
HWND Address_hwnd,Value_hwnd,Compare_hwnd; //Handler for Text Fields.
HWND decode_Button; //Handler for Button.


    switch(Message) {
        case WM_CREATE:
								
								////////////////////////
								//Creating Labels Window.
								///////////////////////
								
								//Creating GameGenie Label.                  
        GG_Label = _createGUIWindow("static",0,10,120,20,hwnd); 
								SetWindowText(GG_Label, "GameGenieCode");                       
								_setGUIFont(GG_Label,SYSTEM_FIXED_FONT);
								
   			 
							//Creating Address Label.                  
        Address_Label = _createGUIWindow("static",0,140,65,20,hwnd); 
								SetWindowText(Address_Label, "Address");                       
								_setGUIFont(Address_Label,SYSTEM_FIXED_FONT);
   			
   			
   				//Creating Value Label.                  
        Value_Label = _createGUIWindow("static",0,100,65,20,hwnd); 
								SetWindowText(Value_Label, "Value");                       
								_setGUIFont(Value_Label,SYSTEM_FIXED_FONT);
   			
      
      	//Creating Compare Label.                  
        Compare_Label = _createGUIWindow("static",0,180,65,20,hwnd); 
								SetWindowText(Compare_Label, "Compare");                       
								_setGUIFont(Compare_Label,SYSTEM_FIXED_FONT);

						   
									//Creating Window for GameGenieInput.
									GameGenieInput = _createGUIWindow("edit",140,10,100,20,hwnd); 
									
									                     
        //Creating Decode_Button Window.                              
        decode_Button = _createGUIWindow("BUTTON",100, 55, 70, 20,hwnd);                       
								_setGUIFont(decode_Button,OEM_FIXED_FONT);
                   																																                               
								//Creating Info.
								info_Label = _createGUIWindow("static",0,227,320,16,hwnd); 
								SetWindowText(info_Label, "\tWritten by HaseeB Mir");                       
								_setGUIFont(info_Label,OEM_FIXED_FONT);
								

        break;

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
                case IDOK:
										
										//Retrieving GameGenie Input and its length.
										GetWindowText(GameGenieInput,&GameGenieCode[0],MAX_GAME_GENIE_LEN + 1);
          GG_Len = GetWindowTextLength(GameGenieInput);
		
		 //Check for Invalid Input. 
   if(isInvalidInput()){
   	
   	    							//Clear TextFields on Error.
   	    clearTextFields(GameGenieInput,Address_hwnd,Value_hwnd,Compare_hwnd);
							
							 MessageBox(NULL, "Invalid GameGenie Length", "Input Error...",MB_ICONERROR);
							return 1;
						}
		
										 
 //If Input is in lower Case convert it to Upper case.
 if(isInputLower(GameGenieCode))
  toUpperCase(GameGenieCode);
  
        
						
											///////////////////////////
										//Creating TextFiles Window.
										//////////////////////////
  										
									//Creating Window for GameGenieInput.
									Value_hwnd = _createGUIWindow("edit",100,100,50,20,hwnd); 
									
									//Creating Window for GameGenieInput.
									Address_hwnd = _createGUIWindow("edit",100,140,50,20,hwnd); 
									
									//Creating Window for GameGenieInput.
									Compare_hwnd = _createGUIWindow("edit",100,180,50,20,hwnd); 
																					 
                                						
						 //Checking for 8-Character GameGenieCode.
						is_8_Char_GameGenie = (GG_Len == MAX_GAME_GENIE_LEN) ? true : false;
											
 			 if(decodeGameGenieCode(GameGenieCode)){
	  		SetWindowText(Address_hwnd,toUpperCase(decodeAddress())); //Prints Address in Hex.  
					SetWindowText(Value_hwnd,toUpperCase(decodeValue()));  //Prints Value in Hex.
 				
     	if(is_8_Char_GameGenie) //If its 8-Character GameGenie then decode Compare value also.
  	    SetWindowText(Compare_hwnd,toUpperCase(decodeCompare()));  //Prints Value in Hex.	
	
					 }
						
						else{
							
							//Clear TextFields on Error.
   	    clearTextFields(GameGenieInput,Address_hwnd,Value_hwnd,Compare_hwnd);
							
							 MessageBox(NULL, "Invalid GameGenie Code Entered", "Input Error...",MB_ICONERROR);
							return 1;
						}
						                             																 
                
                break;
            }

        break;

        case WM_DESTROY: { PostQuitMessage(0);break;}
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
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
char* toUpperCase(char *str) 
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

return str;
}

//Get Length of GameGenie Code.
short getGameGenieLen(){
	
	return (GG_Len != 0 ? GG_Len : 0x0);
	
}

//Checking Invalid Input.
int isInvalidInput(){
	
	if(getGameGenieLen() < 6 || getGameGenieLen() > 8)
	  return 1; //Return True if its invalid Input.
 	 
 return 0;	 //Otherwise return false.
}

//Decodes Value from GameGenie Code.
char* decodeAddress(){
	
 int address = 0x8000 + 
              ((n3 & 7) << 12)
            | ((n5 & 7) << 8) | ((n4 & 8) << 8)
            | ((n2 & 7) << 4) | ((n1 & 8) << 4)
            |  (n4 & 7)       |  (n3 & 8);
             
  //Array to Contain Address in Char* format.
 static  char char_address[4]; //Contains Address in HEX (0xFFFF = 4 Chars).
 sprintf(char_address, "%x",address); //Converting Integer to char* for LPWSTR type needed for GUI Textbox.
	 	 
 return char_address; //Return Char Array.
}

//Decodes Value from 6 or 8-Char GameGenie Code.
char* decodeValue(){
	
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
 
 //Array to Contain Value in Char* format.
 static char char_value[2]; //Contains Value in Hex (FF = 2 Chars Max.).
 sprintf(char_value, "%x", value); //Converting Integer to char* for LPWSTR type needed for GUI Textbox.
	 
 return char_value; //Return Char Array.
}


//Decodes Compare Value for 8-Char GameGenie Code.
char* decodeCompare(){
	
	      int  compare =
             ((n7 & 7) << 4) | ((n6 & 8) << 4)
            | (n6 & 7)       |  (n5 & 8);
         
	 //Array to Contain Compare in Char* format.
 static char compare_value[2]; //Contains Compare in Hex (0xFF = 2 Chars Max.).
 sprintf(compare_value, "%x", compare); //Converting Integer to char* for LPWSTR type needed for GUI Textbox.
		 
 return compare_value; //Return Char Array.
}

//Decodes GameGenie chars to its equivalent Hex.
int decodeGameGenieCode(char *GameGenieCode){
				
int i = 0,j = 0,found = 0;
int GameGenieLen = strlen(GameGenieCode);

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

//Clears All the TextFields.
void clearTextFields(HWND _input,HWND _addr,HWND _val,HWND _comp){
	
	//Set All the TextFields to NULL.
 SetWindowText(_input,"\0");
 SetWindowText(_addr,"\0");
 SetWindowText(_val,"\0");
 SetWindowText(_comp,"\0");
	
}

//Creating GUI Components like Labels,TextFields and Buttons.
HWND _createGUIWindow(LPSTR GUI_Type,int x,int y,int w,int h,HWND _hwnd){
	
	return (!strcmp(GUI_Type,"BUTTON")) 
	
	//Button got a style :p
	? CreateWindow(GUI_Type,"Decode",WS_VISIBLE | WS_CHILD | WS_BORDER,
 x, y, w, h,_hwnd, (HMENU) 1, NULL, NULL) 
 
	//For Labels and TextFields.
	: CreateWindow(GUI_Type,"",WS_BORDER | WS_CHILD | WS_VISIBLE,
 x,y,w,h,_hwnd, NULL, NULL, NULL);

}

//Setting the Fonts to GUI Fields.
LRESULT _setGUIFont(HWND _hwnd,int FontType){
 return SendMessage(_hwnd, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(FontType), (LPARAM)TRUE);
} 