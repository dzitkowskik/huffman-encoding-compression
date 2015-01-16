//
//  huffman.h
//  huffman-encoding-compression
//
//  Created by Karol Dzitkowski on 16.01.2015.
//  Copyright (c) 2015 Karol Dzitkowski. All rights reserved.
//

#ifndef huffman_encoding_compression_huffman_h
#define huffman_encoding_compression_huffman_h

#include "structures.h"
#include <vector>

using namespace std;

vector<bool> encode(string to_encode, header_t& header);
vector<char> decode(header_t header, vector<bool> to_decode);

#endif
