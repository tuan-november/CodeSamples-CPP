//
//  CharNode.h
//  DataCompression
//
//  Created by Tuan Anh Nguyen on 12/15/14.
//  Copyright (c) 2014 Tuan Anh Nguyen. All rights reserved.
//

#pragma once

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
