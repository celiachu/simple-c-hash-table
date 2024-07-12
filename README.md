# simple-c-hash-table

How to implement a hash table (in C)

## Linear Search

遍历搜索

```c
#include <stdio.h>
#include <string.h>

// 定义一个键值对结构体
typedef struct {
    char key[100];  // 键是字符串
    int value;      // 值是整数
} KeyValuePair;
// 线性搜索函数
int linearSearch(KeyValuePair items[], int size, const char* key) {
    for (int i = 0; i < size; i++) {
        if (strcmp(items[i].key, key) == 0) {
            return i;  // 返回找到的索引
        }
    }
    return -1;  // 如果没有找到，返回-1
}

int main() {
    KeyValuePair items[] = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3},
        {"date", 4},
        {"fig", 5}
    };
    int size = sizeof(items) / sizeof(items[0]);

    // 测试线性搜索
    int index = linearSearch(items, size, "cherry");
    if (index != -1) {
        printf("线性搜索找到 'cherry' 在索引: %d, 值: %d\n", index, items[index].value);
    } else {
        printf("线性搜索未找到 'cherry'\n");
    }
    return 0;
}


```

## Binary search

在 C 语言中，strcmp 函数用于比较两个字符串。如果你在使用 strcmp 函数比较两个字符串中间值 items[mid].key 和 key，这通常是在实现一种需要比较字符串的搜索算法中，如二分搜索。

```c
// 用于 bsearch标准库的辅助搜索函数
int cmp(const void* a, const void* b) {
    item* item_a = (item*)a;
    item* item_b = (item*)b;
    return strcmp(item_a->key, item_b->key);
}
// 二分搜索函数
int binarySearch(KeyValuePair items[], int low, int high, const char* key) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(items[mid].key, key);

        if (cmp == 0) {
            return mid;  // 找到键，返回索引
        } else if (cmp < 0) {
            low = mid + 1;  // 在右侧子数组中搜索
        } else {
            high = mid - 1;  // 在左侧子数组中搜索
        }
    }
    return -1;  // 如果没有找到，返回-1
}
int main() {
    KeyValuePair items[] = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3},
        {"date", 4},
        {"fig", 5}
    };
    int size = sizeof(items) / sizeof(items[0]);
    // 测试二分搜索
    index = binarySearch(items, 0, size - 1, "banana");
    if (index != -1) {
        printf("二分搜索找到 'banana' 在索引: %d, 值: %d\n", index, items[index].value);
    } else {
        printf("二分搜索未找到 'banana'\n");
    }

    return 0;
}


```

## Hash Table 哈希表（simple）

简单的哈希函数，使用了乘法和加法的组合来生成基于字符的键的哈希值，然后通过取模操作将这个哈希值映射到一个特定的桶（bucket）。

```c
unsigned int hashFunction(char *key, int bucketCount) {
    unsigned long int value = 0;
    unsigned int i = 0;
    while (key[i] != '\0') {
        value = value * 37 + key[i];
        i++;
    }
    return value % bucketCount;
}
```

FNV-1a 哈希函数，这是 Fowler–Noll–Vo 哈希函数家族中的一种。

```c
// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}
```

`gcc hash_table_with_chains.c`  
`./a.out`  

