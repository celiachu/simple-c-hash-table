#include <stdlib.h>
#include <stdio.h>
#include <string.h>

    // 哈希条目结构体
    typedef struct hash_node
{
    char *key;              // 指向键的指针
    void *value;            // 指向值的指针
    struct hash_node *next; // 指向链表中下一个节点的指针
} hash_node;

// 哈希表结构体
typedef struct hash_table
{
    hash_node **buckets; // 指向哈希节点的指针数组的指针
    int size;            // 桶的数量
    int count;           //  哈希表中总的元素数量
} hash_table;

// 初始化哈希表函数
hash_table *create_hash_table(int num_buckets)
{
    hash_table *table = malloc(sizeof(hash_table)); // 分配内存给哈希表
    if (table == NULL)
    {
        return NULL;
    }

    table->size = num_buckets;
    table->count = 0;
    table->buckets = malloc(sizeof(hash_node *) * num_buckets); // 分配内存给桶指针数组
    if (table->buckets == NULL)
    {
        free(table);
        return NULL;
    }

    for (int i = 0; i < num_buckets; i++)
    {
        table->buckets[i] = NULL;
    }
    return table;
}

// 清理哈希表
void free_hash_table(hash_table *table)
{
    if (table == NULL)
    {
        return;
    }

    for (int i = 0; i < table->size; i++)
    {
        hash_node *node = table->buckets[i];
        while (node != NULL)
        {
            hash_node *next = node->next;
            free(node->key); // 键是动态分配的
            free(node);      // 释放节点
            node = next;
        }
    }
    free(table->buckets); // 释放桶数组
    free(table);          // 释放哈希表结构
}

// 简单的哈希函
unsigned int hash(const char *key, int num_buckets)
{
    unsigned long int value = 0;
    while (*key != '\0')
    {
        value = value * 37 + *key;
        key++;
    }
    return value % num_buckets;
}

// 插入
void hash_table_insert(hash_table *table, const char *key, void *value)
{
    // 使用哈希函数获取键对应的桶索引
    int index = hash(key, table->size);
    hash_node *node = table->buckets[index];

    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            node->value = value; // 键已存在，更新值
            return;
        }
        node = node->next;
    }
    // 如果没有找到相同的键，创建一个新的哈希节点
    hash_node *new_node = malloc(sizeof(hash_node));
    if (new_node == NULL)
    {
        perror("Failed to allocate memory for new hash node");
        return;
    }
    // 为新节点的键复制字符串，确保新节点拥有独立的键副本
    new_node->key = strdup(key);
    if (new_node->key == NULL)
    {                   // 检查strdup是否成功
        free(new_node); // 如果键分配失败，释放已分配的节点内存
        perror("Failed to duplicate key string");
        return;
    }
    new_node->value = value;                // 设置新节点的值
                                            // 将新节点插入到链表的头部
    new_node->next = table->buckets[index]; // 新节点的 next 指针指向当前桶的第一个节点。
    // 如果桶是空的（即 table->buckets[index] 为 NULL），那么 new_node->next 将被设置为 NULL，表明新节点是链表中的唯一节点。
    table->buckets[index] = new_node;
    table->count++;
}

void hash_table_delete(hash_table *table, const char *key)
{
    int index = hash(key, table->size);
    // 获取对应索引位置的链表头节点
    hash_node *node = table->buckets[index];
    // 定义前驱节点，用于记录当前节点的前一个节点，初始为NULL
    hash_node *prev = NULL;

    // 遍历链表查找键
    while (node != NULL)
    {
        // 如果节点的键与要删除的键匹配
        if (strcmp(node->key, key) == 0)
        {
            // 如果当前节点是链表的头节点
            if (prev == NULL)
            {
                // 直接将头节点指向当前节点的下一个节点，从而删除当前节点
                table->buckets[index] = node->next;
            }
            else
            {
                // 如果当前节点不是头节点，则将前一个节点的next指向当前节点的下一个节点，从而删除当前节点
                prev->next = node->next;
            }
            // 释放当前节点的键和节点本身的内存
            free(node->key);
            free(node);
            // 更新哈希表中节点的计数
            table->count--;
            // 删除完成后返回
            return;
        }
        // 如果当前节点不匹配，则继续遍历，更新前驱节点和当前节点
        prev = node;
        node = node->next;
    }
    // 如果整个链表遍历完都没有找到对应的键，输出提示信息
    printf("Key not found\n");
}

void *hash_table_search(hash_table *table, const char *key)
{
    int index = hash(key, table->size);
    hash_node *node = table->buckets[index];
    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}
int main()
{
    // 创建哈希表
    hash_table *table = create_hash_table(10);
    if (table == NULL)
    {
        printf("Failed to create hash table.\n");
        return 1;
    }

    // 插入键值对
    hash_table_insert(table, "name", "Alice");
    hash_table_insert(table, "age", "30");
    hash_table_insert(table, "city", "New York");

    // 查找并打印一些键
    const char *name = hash_table_search(table, "name");
    const char *age = hash_table_search(table, "age");
    if (name != NULL)
    {
        printf("Found name: %s\n", name);
    }
    if (age != NULL)
    {
        printf("Found age: %s\n", age);
    }

    // 尝试查找不存在的键
    const char *salary = hash_table_search(table, "salary");
    if (salary == NULL)
    {
        printf("Salary not found.\n");
    }

    // 删除一个键
    printf("Deleting 'city'\n");
    hash_table_delete(table, "city");

    // 验证删除操作
    const char *city = hash_table_search(table, "city");
    if (city == NULL)
    {
        printf("City not found after deletion.\n");
    }

    // 清理哈希表
    free_hash_table(table);
    printf("Hash table freed.\n");

    return 0;
}
