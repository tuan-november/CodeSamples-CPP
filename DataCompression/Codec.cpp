//
//  Codec.cpp
//  DataCompression
//
//  Created by Tuan Anh Nguyen on 12/7/14.
//  Copyright (c) 2014 Tuan Anh Nguyen. All rights reserved.
//

#include "Codec.h"

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

#pragma mark - Class Codec
#pragma mark -

CCodec::CCodec()
{
    m_root = NULL;
}

CCodec::~CCodec()
{
    if(m_root != NULL)
        deleteCharTree();
}


#pragma mark - Codec - Public Interface
#pragma mark -

// Huffman's Encoding Algorithm
bool CCodec::encodeString(char * output_seq, char * input_str)
{
    if(!isValidForEncoding(input_str))
        return false;
    
    generateCharTree(input_str);
    
    char char_seq[20];
    for(int i = 0; input_str[i] != '\0'; i++)
    {
        memset(char_seq, '\0', 20);
        if(encodeChar(char_seq, input_str[i]))
           strcat(output_seq, char_seq);
        
    }
    
    return true;
}

bool CCodec::decodeString(char * output_str, char * input_seq)
{
    if(!isValidForDecoding(input_seq))
        return false;
    
    CharNode * tree_traveller = m_root;
    int input_seq_idx = 0;
    int output_str_idx = 0;
    char decoded_char = '-';
    
    while(input_seq[input_seq_idx] != '\0')
    {
        if(decodeChar(decoded_char, input_seq, tree_traveller, input_seq_idx))
            output_str[output_str_idx++] = decoded_char;
    }
    
    return true;
}

bool CCodec::reverseString(char * output_str, char * input_str)
{
    if(input_str == NULL || sizeof(output_str) < sizeof(input_str))
        return false;
    
    // Incase output_str == input_str
    char * temp = new char[sizeof(input_str)];
    memset(temp, '\0', sizeof(input_str));
    for(int i = 0, j = (int)strlen(input_str) - 1; j >= 0; i++, j--)
        temp[i] = input_str[j];
    temp[strlen(temp)] = '\0';

    strcpy(output_str, temp);
    delete [] temp;
    
    return true;
}

// Start at "curr_node" in the tree, find the "CharNode" whose value is "input_char"
CharNode * CCodec::locateCharNode(char input_char, CharNode * curr_node)
{
    if(curr_node == NULL)
        return NULL;
    
    CharNode * node_found = NULL;
    
    if(curr_node->character != input_char)
    {
        if(curr_node->left_child != NULL)
            node_found = locateCharNode(input_char, curr_node->left_child);
        if(curr_node->right_child != NULL && node_found == NULL)
            node_found = locateCharNode(input_char, curr_node->right_child);
    }
    
    if(curr_node->character == input_char)
        node_found = curr_node;
    
    return node_found;
}

// Encode a character to a sequence of 0s and 1s
bool CCodec::encodeChar(char * output_seq, char input_char)
{
    CharNode * node_found = locateCharNode(input_char, m_root);
    
    if(node_found == NULL)
        return false;
    else
    {
        for(int i = 0; node_found != m_root; node_found = node_found->parent, i++)
            output_seq[i] = node_found->node_code;
        reverseString(output_seq, output_seq);
//        printf("\n--> char found: %c - %s", input_char, output_seq);
    }
    
    return true;
}

// Decode a sequence of 0s and 1s to a char, starting from "curr_node[curr_idx]"
bool CCodec::decodeChar(char & output_char, char * input_seq, CharNode * curr_node, int & curr_idx)
{
    if(input_seq == NULL)
        return false;
    
    bool char_decoded = false;
    if(curr_node->left_child == NULL && curr_node->right_child == NULL)
    {
        output_char = curr_node->character;
        char_decoded = true;;
    }
    else
    {
        if(input_seq[curr_idx] == '0' && curr_node->left_child != NULL)
            char_decoded = decodeChar(output_char, input_seq, curr_node->left_child, ++curr_idx);
        if(input_seq[curr_idx] == '1' && curr_node->right_child != NULL && !char_decoded)
            char_decoded = decodeChar(output_char, input_seq, curr_node->right_child, ++curr_idx);
    }
    
    return char_decoded;
}



#pragma mark - Codec - Private Interface
#pragma mark -

bool CCodec::isValidForDecoding(char * input_str)
{
    if(input_str == NULL)
        return false;
    
    for(int i = 0; input_str[i] != '\0' && i < kMAX_ENCODED_STRING_SIZE; i++)
    {
        if(input_str[i] != '1' && input_str[i] != '0')
            return false;
    }
    
    return true;
}

bool CCodec::isValidForEncoding(char * input_str)
{
    return (input_str == NULL) ? false : true;
}


bool CCodec::generateCharTree(char * input_str)
{
    if(input_str == NULL)
        return false;
    
    if(m_root != NULL)
        deleteCharTree();
    
    // Initialize frequency of each character
    std::map<char, int> char_hash;
    for(int i = 0; input_str[i] != '\0' && i < kMAX_STRING_SIZE; i++)
    {
        if(char_hash.find(input_str[i]) != char_hash.end())
            char_hash[input_str[i]]++;
        else
            char_hash[input_str[i]] = 1;
    }
    
    // Enqueue the characters with the smallest-frequency chars at the front
    CharNodePtrComparator CharNodeCmpr('<');
    std::priority_queue<CharNode *, std::deque<CharNode *>, CharNodePtrComparator> char_queue(CharNodeCmpr);
    for(std::map<char, int>::iterator itr = char_hash.begin(); itr != char_hash.end(); itr++)
    {
        CharNode * curr_char_node = new CharNode();
        curr_char_node->character = itr->first;
        curr_char_node->frequency = itr->second;
        
        char_queue.push(curr_char_node);
    }
    
    // Generate Character Tree
    while(char_queue.size() > 1)
    {
        CharNode * left_child;
        left_child = char_queue.top();
        left_child->node_code = '0';
        char_queue.pop();
        
        CharNode * right_child;
        right_child = char_queue.top();
        right_child->node_code = '1';
        char_queue.pop();
        
        CharNode * parent = new CharNode();
        parent->left_child = left_child;
        parent->right_child = right_child;
        parent->frequency = left_child->frequency + right_child->frequency;
        left_child->parent = parent;
        right_child->parent = parent;
        
        char_queue.push(parent);
    }
    
    m_root = char_queue.top();
    return true;
}

bool CCodec::deleteTerminalNode(CharNode * curr_node)
{
    if(curr_node == NULL)
        return false;
    
    if(curr_node->left_child == NULL && curr_node->right_child == NULL)
    {
        if(curr_node == m_root)
            delete curr_node;
        else
        {
            CharNode * parent = curr_node->parent;
            if(curr_node->node_code == '0')
                parent->left_child = NULL;
            else if(curr_node->node_code == '1')
                parent->right_child = NULL;
            else
                return false;
            
            delete curr_node;
        }
        return true;
    }
    
    if(curr_node->left_child != NULL)
        deleteTerminalNode(curr_node->left_child);
    if(curr_node->right_child != NULL)
        deleteTerminalNode(curr_node->right_child);
    
    return true;
}

bool CCodec::deleteCharTree()
{
    if(m_root == NULL)
        return false;
    
    CharNode * tree_traveller = m_root;
    deleteTerminalNode(tree_traveller);
    
    return true;
}





