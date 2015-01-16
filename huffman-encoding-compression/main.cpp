//
//  main.cpp
//  huffman-encoding-compression
//
//  Created by Karol Dzitkowski on 15.01.2015.
//  Copyright (c) 2015 Karol Dzitkowski. All rights reserved.
//

#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <map>

#define CHAR_NUM 256

using namespace std;

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

string to_encode = "ADA ATE APPLE";


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
        //std::cout << "Leaf Found: " << tree->symbol << "\t" << code << std::endl;
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

vector<bool> encode(map<char, vector<bool>> codes, string text)
{
    vector<bool> result;
    for(auto it = text.begin(); it != text.end(); it++)
    {
        vector<bool> temp = codes[*it];
        result.insert(result.end(), temp.begin(), temp.end());
    }
    return result;
}

void printBoolVector(vector<bool> vec)
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
        printBoolVector(it->second);
        cout << endl;
    }
}

int main(int argc, const char * argv[]) {
    cout << "START!\n" << std::endl;
    
    header_t header = createHuffmanHeader(to_encode);
    shared_ptr<list<elem_t*>> list_ptr = createTreeList(header);
    elem_t* elem = createHuffmanTree(list_ptr);
    map<char, vector<bool>> codes;
    vector<bool> code;
    generateCodes(elem->tree, code, &codes);
    std::vector<bool> encoded = encode(codes, to_encode);
    cout << codes.size() << endl;
    printBoolVector(encoded);
    cout << endl;
    cout << encoded.size() << endl;
    cout << endl;
    printCodes(codes);
    
    delete elem->tree;
    delete elem;
    return 0;
}
