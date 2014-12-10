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

#define kMAX_STRING_SIZE 1028
#define kMAX_ENCODED_STRING_SIZE (kMAX_STRING_SIZE * (kMAX_STRING_SIZE/2 + 1))

struct CharNode
{
    CharNode();
    
    char   character;
    short  frequency;
    char   node_code; //  0: left_child node;
                      //  1: right_child node;
                      //  X: root node;
    
    CharNode operator=(const CharNode & rhs);
    
    CharNode * parent;
    CharNode * left_child;
    CharNode * right_child;
};

struct CharNodePtrComparator
{
    CharNodePtrComparator(char optr);
    bool operator()(const CharNode * lhs, const CharNode * rhs) const;
    char op;
};

//----------------------------------------------------------------

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
    CharNode * locateCharNode(char input_char, CharNode * curr_node);
    
    bool encodeChar(char * output_seq, char input_char);
    bool decodeChar(char & output_char, CharNode * curr_node, char * input_seq, int & curr_idx);
    
    bool generateCharTree(char * input_str);
    bool deleteTerminalNode(CharNode * curr_node);
    bool deleteCharTree();
    
private:
    
    CharNode * m_root; // characters tree - a compressed form of original input_str
};


