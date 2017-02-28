# GameGenie_in_C
GameGenie Encoder Decoder and Master List Generator  in C.

GameGenie Ported to C from Various sources , use of efficent algorithms and reducing time complexity thus making it more efficient.

GameGenie Encoder :
It Generates 6 or 8 character GameGenie based on whether input has compare value or not.
it encodes GameGenieCode based on Address and Value based in HEX Values.

GameGenie Decoder :
It decodes 6-character GameGenie to Address and data.
Maybe in future i'll add 8-character GameGenie Decoder (PS : Added 8-Character GameGenie Support).

GameGenieMasterListGenerator :
It Generates All the Possbile Combinations of GameGenie Codes and saves it to their respective files.
it Uses efficient algorithm to achieve maximum efficency in generating Large Chunk of Codes.
Input cannot be duplicates remember that it has to be different 2-16 Chars combination. (Added 8-Character GameGenie Support).

GameGenie_6_CharsList.txt :
This is Output of 6-Character GameGenieCodes generated from GameGenieMasterListGenerator 
it contains GameGenieCodes with their equivalent Address and Data Values.

GameGenie_8_CharsList.txt :
This is Output of 8-Character GameGenieCodes generated from GameGenieMasterListGenerator 
it contains GameGenieCodes with their equivalent Address/Data Values and Compare Values.

GameGenie_failedCodesList.txt
This is Output of failed GameGenieCodes generated from GameGenieMasterListGenerator 
it can contains Failed GameGenieCodes that were not generated due to some Error or it will usually contain Junk data from CodeGenieBuffer.


Feel Free to Distribute/Use it in your own project.
Thanks & Regards.
Haseeb Mir (haseebmir.hm@gmail.com)
