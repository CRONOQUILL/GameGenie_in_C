//NES GameGenieMasterListGenetator Generates List of All possible GameGenie Codes.
//Written by Haseeb Mir (haseebmir.hm@gmail.com)
//Dated 03/03/2017.

//GameGenie Refrence by The Mighty Mike Master : http://tuxnes.sourceforge.net/gamegenie.html
//Thanks to Mighty Mike for his amazing refrence.

//Thanks to Jsdemonsim for his efficent Alphabet_generation Algorithm .
//code could be found here https://github.com/Jsdemonsim/Stackoverflow/blob/master/alphabet/alphabet.c

//Including Standard Libraries.
#include <stdio.h> //For Standatd I/O Operations.
#include <stdlib.h> //For Exit() Function.
#include <string.h> //For strrev() and strlen() functions.
#include <stdbool.h> //For _Bool Boolean.
#include <ctype.h> //For isUpper(). isLower()
#include <time.h> //For Clock .
#include <math.h> //For pow().
#include <windows.h> //WinAPI for GUI.

//Defining Constants in form of Macros.
#define NO_OF_CHARS 256 //For Ducplicate Inputs. 
#define GAME_GENIE_LEN 8 //MAX Length of GameGenie Code, Don't edit this.
#define GAME_GENIE_TABLE_LEN 16 //MAX GameGenie Table Length.
#define _6_CharBtn_ID 1 //Defining Button ID's
#define _8_CharBtn_ID 2 //Defining Button ID's
#define LPWSTR_arr_size 0x16 //LPWSTR Array size constant.

//Generates All Possible Combinations using this input.
char GameGenieInput[GAME_GENIE_TABLE_LEN + 1] = {'\0'}; //Initializing the Input to NULL.

//Input could be between 2 to 16 Character combination long taken from GameGenieTable.
//"APZLGITYEOXUKSVN" this is example of 16-Chars string for MAX Input.

/*
Output of 16-char MAX input . could be very large File around greater than 30GB or so.
To open big text file use 'EmEditor' it can handle very large text files easily.
*/

//GameGenie Code Table.
const char GameGenieTable[GAME_GENIE_TABLE_LEN] =
{
    'A','P','Z','L','G','I','T','Y',
    'E','O','X','U','K','S','V','N',
}; //Master GameGenie Table.

short n0,n1,n2,n3,n4,n5,n6,n7; //Contains HEX of GameGenieCode, n0 = 1st Char ... n7 = 8th GameGenie Character.
_Bool is_6_Char_GameGenie = false,is_8_Char_GameGenie = false,is_6CharFile = false,is_8CharFile = false; //Checking for 6 or 8-Character GameGenie Code.
long long _count_6Char = 0,_count_8Char = 0; //Counters for 6 or 8 Char GameGenieCodes.

//Jsdemonsim's written Alphabet Generation Algorithm .
static void generate_combinations(int);

//GameGenieCode subroutines.
void generateGameGenieList(char*,int);
int checkGameGenieCode(char *);
int getGameGenieLen();

//GameGenie value & Address. Routines.
int decodeGameGenieCode(char *);
int decodeGameGenieAddress();
int decodeGameGenieValue();
int decodeGameGenieCompare();

//For Checking duplicate  & Wrong Inputs.
int check_input(char *);
_Bool check_duplicates(char *);
char *remove_duplicates(char *);

//Execute SubRoutines in File and Code Generation.
int exec_CodeSubRoutine(HWND,HWND,HWND,HWND);

//Converting Input to UpperCase if needed.
_Bool isInputLower(char	*str);
void toUpperCase(char *);

//Convert input to LPWSTR format.
char* long_To_LPWSTR(long long);
char* double_To_LPWSTR(double);

//Get Length of GameGenie Code.
int getGameGenieLen();

//Printing Title to Files.
void printFileTitle(FILE**);

void resetValues();

//Clears All TextFields on Error.
void clearTextFields(HWND,HWND,HWND,HWND);


//Global variables.
FILE *_fp_6Char_GG,*_fp_8Char_GG,*_fp_failedCodes; //FilePointer to GameGenieCodes and FailedCodes File.
short Input_Len = 0;
short minlen = 0x0;

long long _6_CHAR_MAX_CODES;
long long _6_CHAR_failedCodes;

long long _8_CHAR_MAX_CODES;
long long _8_CHAR_failedCodes;


//Clock relatee Variables.
clock_t start;
double time_used;


//Window Management procedure.
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//For Creating GUI Windows.
HWND _createGUIWindow(LPSTR,int,int,int,int,HWND);
LRESULT _setGUIFont(HWND,int);

//Main Function for Handlin GUI.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

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
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","NES GameGenie Generator",WS_VISIBLE|WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          320,
                          280,
                          NULL,NULL,hInstance,NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

//For Handling All Windows Created in GUI.
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

//Declaring WIndow Handlers.
    static HWND GameGenieInput_hwnd; //Input field has to be static.
    HWND Input_Label,Time_Label,CodeFail_Label,info_Label; //Handler For Labels
    static HWND _N_Char_Label; //Static Handler for N_Char_Label.
    static HWND Time_hwnd,_N_Char_hwnd,CodeFail_hwnd; //Handler for Text Fields.
    HWND _6Char_Btn,_8Char_Btn; //Handler for Button.


    switch(Message)
    {
    case WM_CREATE:


        //Creating Input Label.
        Input_Label = _createGUIWindow("static",70,0,140,20,hwnd);
        SetWindowText(Input_Label, "GameGenie Input");
        _setGUIFont(Input_Label,SYSTEM_FIXED_FONT);


        //Creating Window for GameGenieInput.
        GameGenieInput_hwnd = _createGUIWindow("edit",70,25,145,20,hwnd);
        //SetWindowText(GameGenieInput,"Ex: APZX");

        ////////////////////////
        //Creating Labels Window.
        ///////////////////////


        //Creating _6_or_8_Char Label.
        _N_Char_Label = _createGUIWindow("static",0,100,100,20,hwnd);
        _setGUIFont(_N_Char_Label,SYSTEM_FIXED_FONT);
        SetWindowText(_N_Char_Label, "N-Char Codes");

        //Creating Time Label.
        Time_Label = _createGUIWindow("static",0,140,100,20,hwnd);
        SetWindowText(Time_Label, "Time Taken");
        _setGUIFont(Time_Label,SYSTEM_FIXED_FONT);

        CodeFail_Label = _createGUIWindow("static",0,180,100,20,hwnd);
        SetWindowText(CodeFail_Label, "Codes Failed");
        _setGUIFont(CodeFail_Label,SYSTEM_FIXED_FONT);



        ///////////////////////////
        //Creating Output Window//
        //////////////////////////


        //Creating 6 or 8-Char Window for Output.
        _N_Char_hwnd = _createGUIWindow("static",120,100,160,20,hwnd);

        //Creating Time Window for Output.
        Time_hwnd = _createGUIWindow("static",120,140,160,20,hwnd);

        //Creating CodeFail Window for Output.
        CodeFail_hwnd = _createGUIWindow("static",120,180,160,20,hwnd);


        //Creating 6Char_Btn Window.
        _6Char_Btn = CreateWindowEx( 0, "BUTTON", "6-Char GameGenie", WS_CHILD | WS_VISIBLE,10, 60, 135, 20,
                                     hwnd, ( HMENU )_6_CharBtn_ID, GetModuleHandle( NULL ), NULL );
        _setGUIFont(_6Char_Btn,OEM_FIXED_FONT);

        //Creating 8Char_Btn Window.
        _8Char_Btn = CreateWindowEx( 0, "BUTTON", "8-Char GameGenie", WS_CHILD | WS_VISIBLE, 160, 60, 135, 20,
                                     hwnd, ( HMENU )_8_CharBtn_ID , GetModuleHandle( NULL ), NULL );
        _setGUIFont(_8Char_Btn,OEM_FIXED_FONT);

        //Creating Info.
        info_Label = _createGUIWindow("static",0,227,320,16,hwnd);
        SetWindowText(info_Label, "\tWritten by HaseeB Mir");
        _setGUIFont(info_Label,OEM_FIXED_FONT);


        break;

    case WM_COMMAND:

        //Checking which button was pressed.

        //If 6-Char Button was pressed.
        if( LOWORD(wParam) == _6_CharBtn_ID)
        {

            //Setting 6CharFile Bool to true.
            is_6CharFile = true;
            is_6_Char_GameGenie = true;
            
            is_8CharFile = false;
			is_8_Char_GameGenie = false;

            //Retrieving GameGenie Input and its length.
            GetWindowText(GameGenieInput_hwnd,&GameGenieInput[0],GAME_GENIE_TABLE_LEN + 1);
            Input_Len = GetWindowTextLength(GameGenieInput_hwnd);

            //Setting Label for 6-Char Codes.
            SetWindowText(_N_Char_Label, "6-Char Codes");

            //Creating Three Files to Hold 6 or 8 GameGenie Codes and failedCodes for any failed Code not generated .
            char *GameGenie_6Char = "GameGenie_6_CharsList.txt"; //For 6-Character GameGenie.
            char *GameGenie_failedCodes = "GameGenie_failedCodesList.txt"; //For Failed Codes or Junk Data.

            //Always Open File in Write Mode.
            _fp_6Char_GG = fopen(GameGenie_6Char, "w");
            _fp_failedCodes = fopen(GameGenie_failedCodes, "w");

            //Checking for failed Memory allocation.
            if (_fp_6Char_GG == NULL || _fp_failedCodes == NULL)
            {
                MessageBox(NULL, "File Creation Error.", "File Error...",MB_ICONERROR);

                //Clear Output Fields.
                clearTextFields(GameGenieInput_hwnd,_N_Char_hwnd,Time_hwnd,CodeFail_hwnd);

                return 1;
            }

            //Prints Title to all Files before Genrating Codes.
            printFileTitle(&_fp_6Char_GG);
																										
													//Clear Output Fields.
        					clearTextFields(GameGenieInput_hwnd,Time_hwnd,_N_Char_hwnd,CodeFail_hwnd);
        
            //execute CodeSubRoutine.
            if(exec_CodeSubRoutine(GameGenieInput_hwnd,_N_Char_hwnd,Time_hwnd,CodeFail_hwnd))
            {
                MessageBox(NULL, "6-Character GameGenie Codes generated Successfully", "Output Info...",MB_OK | MB_ICONINFORMATION);

				//Reset All values back.
                resetValues();

            }

            break;
        }

        //If 8-Char Button was pressed.
        if (LOWORD(wParam) == _8_CharBtn_ID )
        {

            //Setting 8-CharFile Bool to true.
            is_8CharFile = true;
            is_8_Char_GameGenie = true;
            
            is_6CharFile = false;
												is_6_Char_GameGenie = false;
												
            //Retrieving GameGenie Input and its length.
            GetWindowText(GameGenieInput_hwnd,&GameGenieInput[0],GAME_GENIE_TABLE_LEN + 1);
            Input_Len = GetWindowTextLength(GameGenieInput_hwnd);
            
												//Setting Label for 8-Char Codes.
            SetWindowText(_N_Char_Label, "8-Char Codes");

            //Creating Three Files to Hold 6 or 8 GameGenie Codes and failedCodes for any failed Code not generated .
            char *GameGenie_8Char = "GameGenie_8_CharsList.txt"; //For 6-Character GameGenie.
            char *GameGenie_failedCodes = "GameGenie_failedCodesList.txt"; //For Failed Codes or Junk Data.

            //Always Open File in Write Mode.
            _fp_8Char_GG = fopen(GameGenie_8Char, "w");
            _fp_failedCodes = fopen(GameGenie_failedCodes, "w");

            //Checking for failed Memory allocation.
            if (_fp_8Char_GG == NULL || _fp_failedCodes == NULL)
            {
                MessageBox(NULL, "File Creation Error.", "Input Error...",MB_ICONERROR);

                //Clear Output Fields.
                clearTextFields(GameGenieInput_hwnd,_N_Char_hwnd,Time_hwnd,CodeFail_hwnd);
																
				//Reset All values back.
                resetValues();
																
                return 1;
            }

            //Prints Title to all Files before Genrating Codes.
            printFileTitle(&_fp_8Char_GG);

							//Clear Output Fields.
        					clearTextFields(GameGenieInput_hwnd,Time_hwnd,_N_Char_hwnd,CodeFail_hwnd);
        					
            //execute CodeSubRoutine.
            if(exec_CodeSubRoutine(GameGenieInput_hwnd,_N_Char_hwnd,Time_hwnd,CodeFail_hwnd))
            {
                MessageBox(NULL, "8-Character GameGenie Codes generated Successfully", "Output Info...",MB_OK | MB_ICONINFORMATION);

                //Reset All values back.
                resetValues();
            }
            break;
        }

        break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

//Executes Codes & File Subroutine to Generate 6 or 8 GameGenie Code and Handles File Management.
int exec_CodeSubRoutine(HWND _input_hwnd, HWND _N_Char_hwnd,HWND _time_hwnd,HWND _failedCodes_hwnd)
{


  //Creating Timer to count time elapsed while generating MasterList .
		start = clock();
  time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
            

    char *New_Input;//Holds the New_Input after removing duplicates from GameGenieInput.

    //Convert Input to UpperCase if it isn't already.
    if(isInputLower(GameGenieInput))
        toUpperCase(GameGenieInput);

    //Checking  for Wrong Input, Length In-Range and for Duplicates.
    if(check_input(GameGenieInput))
    {
        if(getGameGenieLen() > 1 && getGameGenieLen() <= GAME_GENIE_TABLE_LEN)
        {

            //Check for Duplicate Inputs before generating all combinations.
            if(check_duplicates(GameGenieInput))
            {

                //Copies the new input.
                New_Input = remove_duplicates(GameGenieInput);

                //Checking special Condition
                if(strlen(New_Input) == 1)
                {

                    MessageBox(NULL, "Input Not Sufficent to generate output.", "Input Error...",MB_ICONERROR);

                    //Clear Output Fields.
                    clearTextFields(_input_hwnd,_N_Char_hwnd,_time_hwnd,_failedCodes_hwnd);

                    return 0;
                }

                else
                    MessageBox(NULL, "Duplicates were removed from Input.", "Input Info...",MB_OK | MB_ICONEXCLAMATION);
            }

            //Prints Waiting Message for Large Inputs.
            if(getGameGenieLen() > 3)
                MessageBox(NULL, "Generating GameGenie Codes Please Wait...", "Output Info...",MB_OK);


            //Generates All the Possbile Combinations of using GameGenieTable.
            generate_combinations(GAME_GENIE_LEN);
        }

        else
        {
            MessageBox(NULL, "GameGenieInput Length out of Range.", "Input Error...",MB_OK | MB_ICONERROR);

            //Clear Output Fields.
            clearTextFields(_input_hwnd,_N_Char_hwnd,_time_hwnd,_failedCodes_hwnd);

            return 0;
        }

    }
    else
    {

        MessageBox(NULL, "Wrong GameGenieInput entered.", "Input Error...",MB_OK | MB_ICONERROR);

        //Clear Output Fields.
        clearTextFields(_input_hwnd,_N_Char_hwnd,_time_hwnd,_failedCodes_hwnd);

        return 0;
    }


    //If 6-Char File was selected.
    if (is_6CharFile)
    {

//Holds the Total Possbile codes depends on 'N' length of Input.
 long long _6_CHAR_MAX_CODES  = ((long long)pow(getGameGenieLen(),6));
 long long _6_CHAR_failedCodes = (_6_CHAR_MAX_CODES - _count_6Char);
            
        //Summary.
        SetWindowText(_N_Char_hwnd,long_To_LPWSTR(_count_6Char));
        SetWindowText(_time_hwnd,double_To_LPWSTR(time_used));
        SetWindowText(_failedCodes_hwnd,long_To_LPWSTR((long long)fabs(_6_CHAR_failedCodes)));

        //Print File Title at the end also.
        printFileTitle(&_fp_6Char_GG);
        printFileTitle(&_fp_failedCodes);

        fclose(_fp_6Char_GG);
        fclose(_fp_failedCodes);

        return 1;
    }


    //If 8-Char File was selected.
     if (is_8CharFile)
    {

//Holds the Total Possbile codes depends on 'N' length of Input.
 _8_CHAR_MAX_CODES =  ((long long)pow(getGameGenieLen(),8));
 _8_CHAR_failedCodes = (_8_CHAR_MAX_CODES - _count_8Char);
													
        //Summary
        SetWindowText(_N_Char_hwnd,long_To_LPWSTR(_count_8Char));
        SetWindowText(_time_hwnd,double_To_LPWSTR(time_used));
        SetWindowText(_failedCodes_hwnd,long_To_LPWSTR((long long)fabs(_8_CHAR_failedCodes)));

        //Print File Title at the end also.
        printFileTitle(&_fp_8Char_GG);
        printFileTitle(&_fp_failedCodes);

        //Close the File handles.
        fclose(_fp_8Char_GG);
        fclose(_fp_failedCodes);

        return 1;
    }

}

//Convert long long input to LPWSTR format needed for on GUI.
char* long_To_LPWSTR(long long input)
{

    static char LPWSTR_arr[LPWSTR_arr_size]; //Contains Input in LPWSTR format.

    sprintf(LPWSTR_arr, "%llu",input);

    return LPWSTR_arr;

}

//Convert Double Input to LPWSTR format needed for on GUI.
char* double_To_LPWSTR(double	input)
{

    static char LPWSTR_arr[LPWSTR_arr_size]; //Contains Input in LPWSTR format.

    sprintf(LPWSTR_arr, "%lf",input);

    return LPWSTR_arr;

}

//Jsdemonsim's written Alphabet Generation Algorithm .

//Generates All possible combinations of given input.
static void generate_combinations(int maxlen)
{
    int   alphaLen = strlen(GameGenieInput);
    int   len    = 0;
    char  *buffer  = (char*)malloc((maxlen + 1) * alphaLen * alphaLen);
    int  *letters  = (int*)malloc(maxlen * sizeof(int));

    if (buffer == NULL || letters == NULL)
    {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }

    //Setting Combination Length depend on N-Char GameGenie.
    minlen  = (is_6_Char_GameGenie) ? 6 : 8;

    // This for loop generates 6 and 8 Character Combinations for GameGenie Codes.
    for (len=minlen; len<=maxlen; len+=2)
    {

        // The stride is one larger than len because each line has a '\n'.
        int i;
        int stride = len+1;
        int bufLen = stride * alphaLen * alphaLen;

        if (len == 1)
        {
            // Special case.  The main algorithm hardcodes the last two
            // letters, so this case needs to be handled separately.
            int j = 0;
            bufLen = (len + 1) * alphaLen;
            for (i=0; i<alphaLen; i++)
            {
                buffer[j++] = GameGenieInput[i];
                buffer[j++] = '\n';
            }

            //Generates GameGenieCodes with Current Buffer.
            generateGameGenieList(buffer,bufLen);

            continue;
        }

        // Initialize buffer to contain all first letters.
        memset(buffer, GameGenieInput[0], bufLen);

        // Now write all the last 2 letters and newlines, which
        // will after this not change during the main algorithm.
        {
            // Let0 is the 2nd to last letter.  Let1 is the last letter.
            int let0 = 0;
            int let1 = 0;
            for (i=len-2; i<bufLen; i+=stride)
            {
                buffer[i]   = GameGenieInput[let0];
                buffer[i+1] = GameGenieInput[let1++];
                buffer[i+2] = '\n';
                if (let1 == alphaLen)
                {
                    let1 = 0;
                    let0++;
                    if (let0 == alphaLen)
                        let0 = 0;
                }
            }
        }

        //Generates GameGenieCodes with Current Buffer.
        generateGameGenieList(buffer,bufLen);

        // Special case for length 2, we're already done.
        if (len == 2)
            continue;

        // Set all the letters to 0.
        for (i=0; i<len; i++)
            letters[i] = 0;

        // Now on each iteration, increment the the third to last letter.
        i = len-3;
        do
        {
            char c;
            int  j;

            // Increment this letter.
            letters[i]++;

            // Handle wraparound.
            if (letters[i] >= alphaLen)
                letters[i] = 0;

            // Set this letter in the proper places in the buffer.
            c = GameGenieInput[letters[i]];
            for (j=i; j<bufLen; j+=stride)
                buffer[j] = c;

            if (letters[i] != 0)
            {
                // No wraparound, so we finally finished incrementing.
                // Write out this set.  Reset i back to third to last letter.

                //Generates GameGenieCodes with Current Buffer.
                generateGameGenieList(buffer,bufLen);

                i = len - 3;
                continue;
            }

            // The letter wrapped around ("carried").  Set up to increment
            // the next letter on the left.
            i--;
            // If we carried past last letter, we're done with this
            // whole length.
            if (i < 0)
                break;
        }
        while(1);
    }

    // Clean up.
    free(letters);
    free(buffer);
}

//Generates GameGenie MasterList.
void generateGameGenieList(char *CodeGenieBuffer,int buffLen)
{

    int GG_CodeCount = 0,i = 0;

    //Calculating Number of GameGenie Codes present in current CodeGenie Buffer.
    while(CodeGenieBuffer[i])
    {

        //If there is '\n' means we have iterated over that GameGenie.
        if(CodeGenieBuffer[i] == '\n')
            GG_CodeCount++; //Count for every GameGenie Code Present.

        i++;
    }


    //Contains Total number of Codes in Buffer and their length.
    int index = 0,BufferIndex = 0,CodeGenieIndex = 0,CodeGenieCount = 0,CodeGenieLen = 0;

    //Contains 6 or 8-character GameGenie Code from large generated code Buffer.
    char GameGenieCode[ (is_6_Char_GameGenie) ? (sizeof (char) * 6) : (sizeof (char) * 8)];
    memset(GameGenieCode,'\0',sizeof(GameGenieCode)); //Setting NULL as initial value.

    //Checking 8-Char GameGenie for Every Buffer.
    if((buffLen / GG_CodeCount) == GAME_GENIE_LEN	+ 1){
        is_8_Char_GameGenie = true;
    				is_6_Char_GameGenie = false;
				}


    //Code Buffer has some -ve Values junk strings at the end. Don't process them.
    while(CodeGenieBuffer[index] > 0)
    {

        //Count the Seperate GameGenie Codes from Large Code Chunks.
        if(CodeGenieBuffer[index] == '\n')
        {
            CodeGenieCount++;
            index++; //Skip one character if '\n' occurs in copying from code Buffer.
        }

        //Reset the CodeGenie Index counter.
        if(CodeGenieIndex >= (is_6_Char_GameGenie ? 6 : 8))
            CodeGenieIndex = 0;


        //If we have reached the First/Second and so on GameGenieCode in Buffer then copy that specific code.
        if(CodeGenieCount == BufferIndex)
        {
            GameGenieCode[CodeGenieIndex] = CodeGenieBuffer[index];
            CodeGenieLen++;
        }

        //Insert Null dont let any junk data to be processed further.
        GameGenieCode[GAME_GENIE_LEN] = '\0';

        //Checking length of CodeGenie.
        if(CodeGenieLen == ((is_6_Char_GameGenie ? 6 : 8) * (BufferIndex+1) ))
        {
            //After every 6 or 8 chars there is '\n' from which we are keeping track of individual GameGenieCode.

            if(decodeGameGenieCode(GameGenieCode))
            {
                //If it's 6-Character GameGenie Code.
                if(is_6_Char_GameGenie)
                {
                    //Writes all the GameGenieCodes to File.
                    fprintf(_fp_6Char_GG,"GameGenieCode = %s\n",GameGenieCode); //Prints current GameGenieCode.
                    fprintf(_fp_6Char_GG, "Address = 0x%x\n",decodeGameGenieAddress()); //Prints Address in Hex.
                    fprintf(_fp_6Char_GG, "value = %x\n",decodeGameGenieValue()); //Prints value in Hex.
                    fprintf(_fp_6Char_GG,"\n");
                    _count_6Char++;
                }

                //If it's 8-Character GameGenie Code.
                if(is_8_Char_GameGenie)
                {
                    //Writes all the GameGenieCodes to File.
                    fprintf(_fp_8Char_GG,"GameGenieCode = %s\n",GameGenieCode); //Prints current GameGenieCode.
                    fprintf(_fp_8Char_GG, "Address = 0x%x\n",decodeGameGenieAddress()); //Prints Address in Hex.
                    fprintf(_fp_8Char_GG, "value = %x\n",decodeGameGenieValue()); //Prints value in Hex.
                    fprintf(_fp_8Char_GG, "compare = %x\n",decodeGameGenieCompare()); //Prints Compare Value in Hex.
                    fprintf(_fp_8Char_GG,"\n");
                    _count_8Char++;
                }

            }

            //Writing for any Failed GameGenie Code or Any Junk Data.
            else
                fprintf(_fp_failedCodes,"%s\n",GameGenieCode);

            //Incrementing Bufferindex.
            BufferIndex++;
        }

        //Incrementing index and CodeGenieIndex.
        index++;
        CodeGenieIndex++;
    }
}

//decode GameGenieCode.
int decodeGameGenieCode(char *GameGenieCode)
{

    int i = 0,j = 0,found = 0;

    for(i = 0; i < (is_6_Char_GameGenie ? 6 : 8) ; i++)
    {
        for(j = 0; j < GAME_GENIE_TABLE_LEN; j++)
            if(GameGenieCode[i] == GameGenieTable[j])
            {
                found ++;

                //Twirling Ternary operators to convert Game Genie to its equivalent Hex.
                (found == 1) ? n0 = j : (found == 2) ? n1 = j :
                (found == 3) ? n2 = j : (found == 4) ? n3 = j :
                (found == 5) ? n4 = j : (found == 6) ? n5 = j :j;


                (is_8_Char_GameGenie) ? (found == 7 ? n6 = j : found == 8 ? n7 = j : j) : j;

            }
    }

//Checking for Invalid Genie Codes.
    return (found == (is_6_Char_GameGenie ? 6 : 8)) ? 1 : 0;
}

//decodes GameGenieAddress
int decodeGameGenieAddress()
{

    int address = 0x8000 +
                  ((n3 & 7) << 12)
                  | ((n5 & 7) << 8) | ((n4 & 8) << 8)
                  | ((n2 & 7) << 4) | ((n1 & 8) << 4)
                  |  (n4 & 7)       |  (n3 & 8);

    return address;

}

//Decodes Value from 6 or 8-Char GameGenie Code.
int decodeGameGenieValue()
{

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
int decodeGameGenieCompare()
{

    int  compare =
        ((n7 & 7) << 4) | ((n6 & 8) << 4)
        | (n6 & 7)       |  (n5 & 8);
    return compare;

}


//Get Length of GameGenie Code.
int getGameGenieLen()
{
    return strlen(GameGenieInput);
}


//Check the GameGenieInput.
int check_input(char * input)
{

    int input_found = 0;
    int input_index = 0;
    int Table_index = 0;

    for(input_index = 0; input_index < strlen(input); input_index++)
    {
        for(Table_index = 0; Table_index < GAME_GENIE_TABLE_LEN; Table_index++)
        {

            if(GameGenieInput[input_index] == GameGenieTable[Table_index])
                input_found++;
        }
    }

 //Checking for Invalid Inputs.
    return (input_found == strlen(input) ) ? 1 : 0;
}


//Check for Duplicate inputs.
_Bool check_duplicates	(char str[])
{

    static _Bool Input_has_dup = false; //Boolean to check duplicate.

    int str_index = 0,Table_index = 0,dup_count[GAME_GENIE_TABLE_LEN] = {0}; //Count for Every character from GameGenieTable.


    //Checking Repeated Strings.
    while (str[str_index] != '\0')
    {

        for(Table_index = 0; Table_index < GAME_GENIE_TABLE_LEN; Table_index++)
        {

            if (str[str_index] == GameGenieTable[Table_index]) //Comparing input characters to GameGenieTable characters.
                dup_count[Table_index]++; //Count Array of Duplicates if true.
        }

        str_index++;
    }

    //Checking if duplicates exist in input or not ? Later Removing them if needed.
    for(str_index = 0; str_index < Table_index	; str_index++)
    {

        //If input has atleast 1 duplicate.
        if(dup_count[str_index] > 1)
        {
            Input_has_dup = true; //Set Bool to True.
            break; //No need to Search for more Duplicates.
        }
    }

    return Input_has_dup;
}

//Remove Duplicates from Input using Hashing.
//Time Complexity: O(n)
char *remove_duplicates(char *str)
{
    int bin_hash[NO_OF_CHARS] = {0}; /* Binary hash to see if character is
                                        already processed or not */

    int input_index = 0; /* index to  keep track of location of next
							character in input string */

    int result_index = 0; /* index to  keep track of location of
                            next character in the resultant string */
    char temp;


    /* In place removal of duplicate characters*/
    while (*(str + input_index))
    {
        temp = *(str + input_index);
        if (bin_hash[temp] == 0)
        {
            bin_hash[temp] = 1;
            *(str + result_index) = *(str + input_index);
            result_index++;
        }
        input_index++;
    }

    /* After above step string is stringiittg.
       Removing extra iittg after string*/
    *(str+result_index) = '\0';

    return str;
}

//Checking if Input is in Lower Case.
_Bool isInputLower(char	*str)
{

    int index = 0;

    for(index = 0; index < strlen(str); index++)
    {
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


//Prints Title on Respective Files.
void printFileTitle(FILE **_fp)
{
    if(_fp == &_fp_6Char_GG)
        //For 6-Char GameGenie Codes.
        fprintf(_fp_6Char_GG,"%s\n","/////////////////////////////////\n6-Character Game Genie Codes...\n////////////////////////////////");

    else if(_fp == &_fp_8Char_GG)
        //For 8-Char GameGenie Codes.
        fprintf(_fp_8Char_GG,"%s\n","//////////////////////////////////\n8-Character Game Genie Codes...\n////////////////////////////////");

    else if(_fp == &_fp_failedCodes)
        //For Failed Game Genie Codes.
        fprintf(_fp_failedCodes,"%s\n","//////////////////////////////////\nFailed Codes or Junk Data...\n////////////////////////////////");

}


//Creating GUI Components like Labels,TextFields and Buttons.
HWND _createGUIWindow(LPSTR GUI_Type,int x,int y,int w,int h,HWND _hwnd)
{

    return
								//For Labels and TextFields.
        CreateWindow(GUI_Type,"",WS_BORDER | WS_CHILD | WS_VISIBLE,
                     x,y,w,h,_hwnd, NULL, NULL, NULL);

}

//Setting the Fonts to GUI Fields.
LRESULT _setGUIFont(HWND _hwnd,int FontType)
{
    return SendMessage(_hwnd, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(FontType), (LPARAM)TRUE);
}

void resetValues()
{

    is_6_Char_GameGenie,is_8_Char_GameGenie,is_6CharFile,is_8CharFile = false;
    _count_6Char,_count_8Char,start,time_used,minlen,
				_6_CHAR_MAX_CODES,_6_CHAR_failedCodes,_8_CHAR_MAX_CODES,_8_CHAR_failedCodes = 0;
    
    //Reset the Input aswell.
    memset(GameGenieInput,'\0',sizeof(GameGenieInput));

}

//Clears All the TextFields.
void clearTextFields(HWND _input,HWND _addr,HWND _val,HWND _comp)
{
	
    //Set All the TextFields to NULL.
    SetWindowText(_input,"\0");
    SetWindowText(_addr,"\0");
    SetWindowText(_val,"\0");
    SetWindowText(_comp,"\0");

}