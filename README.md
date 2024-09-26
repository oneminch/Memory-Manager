# Memory Manager

A memory manager implemented in C++ (malloc &amp; free)

**Example Output**

```bash
------------------------

Current:0x7f9cca7cc000
Size:4080

------------------------


Allocated 21 bytes at 0x7f9cca7cc000


------------------------

Current:0x7f9cca7cc015
Size:4059

------------------------


Allocated 22 bytes at 0x7f9cca7cc015


------------------------

Current:0x7f9cca7cc02b
Size:4037

------------------------


Allocated 44 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc057
Size:3993

------------------------


Allocated 54 bytes at 0x7f9cca7cc057


------------------------

Current:0x7f9cca7cc08d
Size:3939

------------------------


Allocated 104 bytes at 0x7f9cca7cc08d


------------------------

Current:0x7f9cca7cc0f5
Size:3835

------------------------


Allocated 20 bytes at 0x7f9cca7cc0f5


------------------------

Current:0x7f9cca7cc109
Size:3815

------------------------


Freed space of 44 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc02b
Size:44

Current:0x7f9cca7cc109
Size:3815

------------------------


Freed space of 54 bytes at 0x7f9cca7cc057


------------------------

Current:0x7f9cca7cc02b
Size:98

Current:0x7f9cca7cc109
Size:3815

------------------------


Freed space of 104 bytes at 0x7f9cca7cc08d


------------------------

Current:0x7f9cca7cc02b
Size:202

Current:0x7f9cca7cc109
Size:3815

------------------------


Freed space of 20 bytes at 0x7f9cca7cc0f5


------------------------

Current:0x7f9cca7cc02b
Size:4037

------------------------


Allocated 82 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc07d
Size:3955

------------------------


Freed space of 21 bytes at 0x7f9cca7cc000


------------------------

Current:0x7f9cca7cc000
Size:21

Current:0x7f9cca7cc07d
Size:3955

------------------------


Freed space of 82 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc000
Size:21

Current:0x7f9cca7cc02b
Size:4037

------------------------


Allocated 20 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc000
Size:21

Current:0x7f9cca7cc03f
Size:4017

------------------------


Freed space of 22 bytes at 0x7f9cca7cc015


------------------------

Current:0x7f9cca7cc000
Size:43

Current:0x7f9cca7cc03f
Size:4017

------------------------


Freed space of 20 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc000
Size:4080

------------------------


Allocated 21 bytes at 0x7f9cca7cc000


------------------------

Current:0x7f9cca7cc015
Size:4059

------------------------


Allocated 22 bytes at 0x7f9cca7cc015


------------------------

Current:0x7f9cca7cc02b
Size:4037

------------------------


Allocated 404 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc1bf
Size:3633

------------------------


Freed space of 404 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc02b
Size:4037

------------------------


Allocated 54 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc061
Size:3983

------------------------


Freed space of 54 bytes at 0x7f9cca7cc02b


------------------------

Current:0x7f9cca7cc02b
Size:4037

------------------------


Freed space of 21 bytes at 0x7f9cca7cc000


------------------------

Current:0x7f9cca7cc000
Size:21

Current:0x7f9cca7cc02b
Size:4037

------------------------


Freed space of 22 bytes at 0x7f9cca7cc015


------------------------

Current:0x7f9cca7cc000
Size:4080

------------------------
```
