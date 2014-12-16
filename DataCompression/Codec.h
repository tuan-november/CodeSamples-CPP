//
//  Codec.h
//  DataCompression
//
//  Created by Tuan Anh Nguyen on 12/7/14.
//  Copyright (c) 2014 Tuan Anh Nguyen. All rights reserved.
//

#pragma once

#include <map>
#include <queue>

#include "CharNode.h"

#define kMAX_STRING_SIZE 1028
#define kMAX_ENCODED_STRING_SIZE (kMAX_STRING_SIZE * (kMAX_STRING_SIZE/2 + 1))


class CCodec
{
public:
    
    CCodec();
    virtual ~CCodec();
    
    bool encodeString(char * output_seq, char * input_str); //"Hello" --> "0111001"
    bool decodeString(char * output_str, char * input_seq); //"0111001" --> "Hello"
    
protected:
    
    bool isValidForDecoding(char * input_str);
    bool isValidForEncoding(char * input_str);
    
    bool reverseString(char * output_str, char * input_str);
    
    // Start at "curr_node" in the tree, find the "CharNode" whose value is "input_char"
    CharNode * locateCharNode(char input_char, CharNode * curr_node);
    
    // Encode a character to a sequence of 0s and 1s
    bool encodeChar(char * output_seq, char input_char);
    // Decode a sequence of 0s and 1s to a char, starting from "curr_node[curr_idx]"
    bool decodeChar(char & output_char, char * input_seq, CharNode * curr_node, int & curr_idx);
    
    // Compress "input_str" into a tree data structure
    bool generateCharTree(char * input_str);
    bool deleteTerminalNode(CharNode * curr_node);
    bool deleteCharTree();
    
private:
    
    CharNode * m_root; // characters tree - a compressed form of original input_str
};


