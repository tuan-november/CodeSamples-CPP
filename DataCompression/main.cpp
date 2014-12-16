//
//  main.cpp
//  DataCompression
//
//  Created by Tuan Anh Nguyen on 12/7/14.
//  Copyright (c) 2014 Tuan Anh Nguyen. All rights reserved.
//

#include "Codec.h"

void printStr(const char *);

int main(int argc, const char * argv[])
{
    char * input_str = new char[kMAX_STRING_SIZE];
    strcpy(input_str, "Silent Night... Holy Night... All is calm... All is bright...");
    printStr(input_str);
    
    CCodec CodecMgr;
    
    char * encoded_seq = new char[kMAX_ENCODED_STRING_SIZE];
    if(CodecMgr.encodeString(encoded_seq, input_str))
        printStr(encoded_seq);
    
    char * decoded_str = new char[kMAX_STRING_SIZE];
    if(CodecMgr.decodeString(decoded_str, encoded_seq))
        printStr(decoded_str);
    
    delete [] input_str;
    delete [] encoded_seq;
    delete [] decoded_str;
    
    return 0;
}

void printStr(const char * my_str)
{
    if(my_str == NULL)
        return;
    
    for(int i = 0; my_str[i] != '\0' && i < kMAX_STRING_SIZE; i++)
        printf("%c", my_str[i]);
    
    printf("\n");
}
