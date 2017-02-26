//GameGenieMasterListGenetator Generates List of All possible GameGenie Codes.
//Written by Haseeb Mir (haseebmir.hm@gmail.com)
//Dated 23/02/2017.

//GameGenie Refrence by The Mighty Mike Master : http://tuxnes.sourceforge.net/gamegenie.html
//Thanks to Mighty Mike for his amazing refrence.

//Thanks to Jsdemonsim for his efficent Alphabet_generation Algorithm .
//code could be found here https://github.com/Jsdemonsim/Stackoverflow/blob/master/alphabet/alphabet.c

//Including Standard Libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//Defining Constants in form of Macros.
#define NO_OF_CHARS 256 //For Ducplicate Inputs. 
#define GAME_GENIE_LEN 6 //MAX Length of GameGenie Code, Don't edit this.
#define GAME_GENIE_TABLE_LEN 16 //MAX GameGenie Table Length.

//Generates All Possible Combinations using this input.
char GameGenieInput[] = "APZLG"; //Change here for different input.

//Default input is set to 5-Chars combination but input could be between 2 to 16 Chars combination long taken from GameGenieTable.

//"APZLGITYEOXUKSVN" this is 16-Chars string for MAX Input.

/*
Output of 16-char MAX input .

File Created : GameGenieMasterList.txt
16777216 GameGenieCodes Written in File
Time Taken = 126.064000 seconds
File Size : 895 MB (MegaBytes)
*/


//GameGenie Code Table.
char GameGenieTable[GAME_GENIE_TABLE_LEN] =
{
    'A','P','Z','L','G','I','T','Y',
    'E','O','X','U','K','S','V','N',
}; //Master GameGenie Table.

short n0,n1,n2,n3,n4,n5; //Contains HEX equivalent of GameGenie Character from GameGenieTable.


//Jsdemonsim's written Alphabet Generation Algorithm .
static void generate_combinations(int);

//GameGenie Data & Address. Routines.
void generateGameGenieList(char*);
int checkGameGenieCode(char *);
int decodeGameGenieCode(char *);
int decodeGameGenieAddress();
int decodeGameGenieData();

//For Checking duplicate  & Wrong Inputs.
int check_input(char *);
_Bool check_duplicates(char *);
char *remove_duplicates(char *);
void caseToUpper(char *);

//Global variables.
int TotalCodes = 0; //Count the Total number of Codes.
FILE *fp; //FilePointer to MasterList File.

int main()
{
    //Creating Timer to count time elapsed while generating MasterList .
    clock_t start;
    double time_used;
    start = clock();

    char *New_Input;//Holds the New_Input after removing duplicates from GameGenieInput.

    char GameGenieCodesFile[] = "GameGenieMasterList.txt";

    //Creating File to Hold All GameGenie Codes.
    fp = fopen(GameGenieCodesFile, "w");
    if (fp == NULL)
    {
        perror("Error : "); //Perror Automatically detects the type of error occured and prints that error.
        exit(1); //Exit with Error.
    }

    //Convert Input to UpperCase if it isn't already.
    caseToUpper(GameGenieInput);

    //Checking  for Wrong Input, Length In-Range and for Duplicates.
    if(check_input(GameGenieInput))
    {
        if(strlen(GameGenieInput) > 1 && strlen(GameGenieInput) < GAME_GENIE_TABLE_LEN)
        {

            //Check for Duplicate Inputs before generating all combinations.
            if(check_duplicates(GameGenieInput))
            {

                //Copies the new input.
                New_Input = remove_duplicates(GameGenieInput);

                //Checking special Condition
                if(strlen(New_Input) == 1)
                {
                    printf("After removing duplicates\nGameGenieInput Length was not Sufficent to generate Combinations.\n");
                    exit(EXIT_FAILURE);
                }

                else
                    printf("Duplicates were removed from Input.\nNew Input is : %s\n",New_Input);
            }

            //Generates All the Possbile Combination of GameGenieList with MAX_Length_GameGenie.
            generate_combinations(GAME_GENIE_LEN);
        }

        else
        {
            printf("GameGenieInput Length out of Range.\n");
            exit(EXIT_FAILURE);
        }

    }
    else
    {
        printf("Wrong GameGenieInput entered.\n");
        exit(EXIT_FAILURE);
    }


    time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    printf("\nFile Created : %s\n%d GameGenieCodes Written in File.\n\nTime Taken = %f seconds\n",GameGenieCodesFile,TotalCodes,time_used);

//Close the file handle.
    fclose(fp);

    return EXIT_SUCCESS;
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

    // This for loop generates all 1 letter patterns, then 2 letters, etc,
    // up to the given maxlen.
    for (len=1; len<=maxlen; len++)
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
            generateGameGenieList(buffer);

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
        generateGameGenieList(buffer);

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
                generateGameGenieList(buffer);

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
void generateGameGenieList(char *CodeGenieBuffer)
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


    //Filter 6-digits Game Genie codes only.
    if( (strlen(CodeGenieBuffer) / GG_CodeCount ) == GAME_GENIE_LEN + 1)
    {
        // CodeBuffer Length / No. of Codes in Buffer is equal to 6-Digit GameGenieCode :)

        char GameGenieCode[6] = {0x0};//Contains 6-digit GameGenie Code from large generated code Buffer.
        int index = 0,BufferIndex = 0,CodeGenieIndex = 0,CodeGenieCount = 0,CodeGenieLen = 0;

        //Code Buffer has some -ve Values junk strings at the end. Don't copy them.
        while(CodeGenieBuffer[index] > 0)
        {

            //Count the Seperate GameGenie Codes from Large Code Chunks.
            if(CodeGenieBuffer[index] == '\n')
            {
                CodeGenieCount++;
                index++; //Skip one character if '\n' occurs in copying from code Buffer.
            }

            //Reset the CodeGenie Index counter.
            if(CodeGenieIndex >= 6) CodeGenieIndex = 0;

            //If we have reached the First/Second and so on GameGenieCode in Buffer then copy that specific code.
            if(CodeGenieCount == BufferIndex)
            {
                GameGenieCode[CodeGenieIndex] = CodeGenieBuffer[index];
                CodeGenieLen++;
            }

            //Checking length of CodeGenie.
            if(CodeGenieLen == (6 * (BufferIndex+1) ) )
            {
                //After every 6 chars there is '\n' from which we are keeping track of individual GameGenieCode.

                if(decodeGameGenieCode(GameGenieCode))
                {
                    //Writes all the GameGenieCodes to File.
                    fprintf(fp,"GameGenieCode = %s\n",GameGenieCode); //Prints current GameGenieCode.
                    fprintf(fp, "Address = 0x%x\n",decodeGameGenieAddress()); //Prints Address in Hex.
                    fprintf(fp, "Data = %x\n",decodeGameGenieData()); //Prints Data in Hex.
                    fprintf(fp,"\n");

                    //Counter for Total Number of Codes.
                    TotalCodes++;

                    /*Use this when you need to print on console.

                    printf("\nAddress = 0x%x\n",decodeGameGenieAddress());
                    printf("Data = %x\n",decodeGameGenieData());
                    printf("GameGenieCode = %s\n",GameGenieCode);
                    */

                }

                //Checking for any Junk Data in CodeBuffer.
                else
                    printf("\n%s is not a valid GameGenie Code\n",GameGenieCode);

                //Incrimenting index.
                BufferIndex++;
            }

            //Incrimenting indexes.
            index++;
            CodeGenieIndex++;
        }
    }

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

//decodes GameGenieData.
int decodeGameGenieData()
{

    int data =
        ((n1 & 7) << 4) | ((n0 & 8) << 4)
        | (n0 & 7)       |  (n5 & 8);

    return data;
}

//decode GameGenieCode.
int decodeGameGenieCode(char *GameGenieCode)
{

    int i = 0,j = 0,found = 0;

    for(i = 0; i < GAME_GENIE_LEN ; i++)
    {
        for(j = 0; j < GAME_GENIE_TABLE_LEN; j++)
            if(GameGenieCode[i] == GameGenieTable[j])
            {
                found ++;

                //Twirling Ternary operators to convert Game Genie to its equivalent Hex.
                (found == 1) ? n0 = j : (found == 2) ? n1 = j :
                                    (found == 3) ? n2 = j : (found == 4) ? n3 = j :
                                                        (found == 5) ? n4 = j : (found == 6) ? n5 = j :j;
            }
    }

//Checking for Invalid Genie Codes.
    return (found == 6) ? 1 : 0;
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

//Converts the Input to Capital case.
void caseToUpper(char *str)
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