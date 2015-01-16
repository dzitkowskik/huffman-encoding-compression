//
//  main.cpp
//  huffman-encoding-compression
//
//  Created by Karol Dzitkowski on 15.01.2015.
//  Copyright (c) 2015 Karol Dzitkowski. All rights reserved.
//

#include "huffman.h"

#include <iostream>
#include <list>
#include <memory>
#include <map>

template<class T>
void printVector(vector<T> vec)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        cout << *it;
    }
}

void printCodes(map<char, vector<bool>> codes)
{
    for(auto it = codes.begin(); it != codes.end(); it++)
    {
        cout << "Char: " << it->first << " Code: ";
        printVector(it->second);
        cout << endl;
    }
}

header_t createHuffmanHeader(string text)
{
    header_t result;
    for(int i=0; i<256; i++) result.letters[i] = 0;
    for(auto it = text.begin(); it != text.end(); it++)
    {
        result.letters[*it]++;
    }
    return result;
}

shared_ptr<list<elem_t*>> createTreeList(header_t header)
{
    shared_ptr<list<elem_t*>> list_ptr(new list<elem_t*>());
    for (int i=0; i < CHAR_NUM; i++) {
        int freq = (int)header.letters[i];
        elem_t* temp = new elem_t(freq);
        temp->tree = new tree_t((char)i);
        list_ptr->push_front(temp);
    }
    return list_ptr;
}

elem_t* mergeElements(elem_t* first, elem_t* second)
{
    elem_t* newElement = new elem_t(first->frequency+second->frequency);
    
    // merge trees
    tree_t* newTree = new tree_t('\0');
    newTree->left = first->tree;
    newTree->right = second->tree;
    newElement->tree = newTree;
    
    delete first;
    delete second;
    
    return newElement;
}

elem_t* createHuffmanTree(shared_ptr<list<elem_t*>> list_ptr)
{
    while (list_ptr->size() != 1)
    {
        list_ptr->sort( []( const elem_t* a, const elem_t* b ) { return (a->frequency) < (b->frequency); } );
        elem_t* first = *(list_ptr->begin());
        list_ptr->pop_front();
        elem_t* second = *(list_ptr->begin());
        list_ptr->pop_front();
        list_ptr->push_front(mergeElements(first, second));
    }
    
    return *list_ptr->begin();
}

void generateCodes(tree_t *tree, vector<bool> code, map<char, vector<bool>>* codes)
{
    if(tree->left == NULL && tree->right == NULL)
    {
        codes->insert(pair<char, vector<bool>>(tree->character, code));
    }
    if(tree->left != NULL)
    {
        code.push_back(0);
        generateCodes(tree->left, code, codes);
        code.pop_back();
    }
    if(tree->right != NULL)
    {
        code.push_back(1);
        generateCodes(tree->right, code, codes);
    }
}

vector<bool> _encode(map<char, vector<bool>> codes, string text)
{
    vector<bool> result;
    for(auto it = text.begin(); it != text.end(); it++)
    {
        vector<bool> temp = codes[*it];
        result.insert(result.end(), temp.begin(), temp.end());
    }
    return result;
}

vector<char> _decode(tree_t* tree, vector<bool> code)
{
    vector<char> result;
    tree_t* temp = tree;
    for(auto it = code.begin(); it != code.end(); it++)
    {
        if(*it)
            temp = temp->right;
        else
            temp = temp->left;
        if(temp->isLeaf())
        {
            result.push_back(temp->character);
            temp = tree;
        }
    }
    return result;
}

vector<bool> encode(string to_encode, header_t& header)
{
    // Create a header and tree
    header = createHuffmanHeader(to_encode);
    shared_ptr<list<elem_t*>> list_ptr = createTreeList(header);
    elem_t* elem = createHuffmanTree(list_ptr);
    
    // Generate huffman codes
    map<char, vector<bool>> codes;
    vector<bool> code;
    generateCodes(elem->tree, code, &codes);
    
    // Encode using huffman algorithm
    vector<bool> encoded = _encode(codes, to_encode);
    
    delete elem->tree;
    delete elem;
    
    return encoded;
}

vector<char> decode(header_t header, vector<bool> to_decode)
{
    // Create a tree
    shared_ptr<list<elem_t*>> list_ptr = createTreeList(header);
    elem_t* elem = createHuffmanTree(list_ptr);
    
    // Decode using the same tree
    std::vector<char> decoded = _decode(elem->tree, to_decode);
    
    delete elem->tree;
    delete elem;
    
    return decoded;
}

int main(int argc, const char * argv[])
{
    string to_encode = "ADA ATE APPLE";
    
    header_t header;
    vector<bool> encoded = encode(to_encode, header);
    vector<char> decoded = decode(header, encoded);
    
    cout << "Encoded text: " << endl;
    printVector(encoded);
    cout << endl;
    
    cout << "Decoded text: " << endl;
    printVector(decoded);
    cout << endl;
    
    return 0;
}
