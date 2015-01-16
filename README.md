# huffman-encoding-compression
Simple huffman encoding compression written in C++

# Interface
```cpp
vector<bool> encode(string to_encode, header_t& header);
vector<char> decode(header_t header, vector<bool> to_decode);
```
  
# Usage
```cpp
#include "huffman.h"

header_t header;
vector<bool> encoded = encode(to_encode, header);
vector<char> decoded = decode(header, encoded);
```
