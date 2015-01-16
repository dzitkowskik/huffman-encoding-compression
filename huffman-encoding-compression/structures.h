//
//  structures.h
//  huffman-encoding-compression
//
//  Created by Karol Dzitkowski on 16.01.2015.
//  Copyright (c) 2015 Karol Dzitkowski. All rights reserved.
//

#ifndef huffman_encoding_compression_structures_h
#define huffman_encoding_compression_structures_h

#define CHAR_NUM 256

struct tree_t
{
    tree_t *left;
    tree_t *right;
    char character;
    
    tree_t()
    {
        left = nullptr;
        right = nullptr;
    }
    tree_t(char _char):tree_t()
    {
        this->character = _char;
    }
    ~tree_t()
    {
        if(left != nullptr) delete left;
        if(right != nullptr) delete right;
    }
    
    bool isLeaf()
    {
        return left==nullptr && right==nullptr;
    }
};

struct elem_t
{
    tree_t *tree;
    int frequency;
    
    elem_t()
    {
        tree = nullptr;
    }
    elem_t(int _freq):elem_t()
    {
        this->frequency = _freq;
    }
    ~elem_t(){}
};

struct header_t
{
    char letters[CHAR_NUM];
};

#endif
