//
//  CharNode.cpp
//  DataCompression
//
//  Created by Tuan Anh Nguyen on 12/15/14.
//  Copyright (c) 2014 Tuan Anh Nguyen. All rights reserved.
//

#include "CharNode.h"

#ifndef NULL
#define NULL (0)
#endif

#pragma mark - Struct CharNode
#pragma mark -

CharNode::CharNode()
{
    character   = '\0';
    frequency   = 0;
    node_code   = 'X';
    
    parent      = NULL;
    left_child  = NULL;
    right_child = NULL;
}

CharNode CharNode::operator=(const CharNode & rhs)
{
    if(this == &rhs)
        return *this;
    
    character = rhs.character;
    frequency = rhs.frequency;
    node_code = rhs.node_code;
    
    parent = rhs.parent;
    left_child = rhs.left_child;
    right_child = rhs.right_child;
    
    return *this;
}

#pragma mark - CharNode Comparator
#pragma mark -

CharNodePtrComparator::CharNodePtrComparator(char optr)
{
    op = optr;
}

bool CharNodePtrComparator::operator()(const CharNode * lhs, const CharNode * rhs) const
{
    if(op == '<')
        return lhs->frequency > rhs->frequency; // nodes with the smallest frequency enqueued at the front
    
    return false;
}
