#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* ht_key_t;
typedef int ht_type_t;
typedef int ht_value_t;
typedef char* ht_value_of;

// key => value plus pointer to next item for hash collisions
typedef struct HashTableItem HashTableItem;
struct HashTableItem {
  ht_key_t       key;
  ht_value_t     value;
  ht_type_t      type;
  ht_value_of    value_of;
  HashTableItem* next;
};

// Contains an array of pointers to HashTableItems
typedef struct HashTable {
  HashTableItem** items;
  size_t          size;
  size_t          count; // could be used for rehashing in future
} HashTable;

// Creates a pointer to a new hash table item
HashTableItem* ht_create_item(ht_key_t key, ht_value_t value, ht_type_t type) {
  HashTableItem* item = malloc(sizeof *item);
  item->key           = strdup(key);
  item->value         = value;
  item->type          = type;
  item->type          = NULL;
  item->next          = NULL;
  return item;
}

// Creates a new HashTable
HashTable* ht_create(size_t size) {
  HashTable* table = malloc(sizeof *table);
  table->size      = size;
  table->count     = 0;
  table->items = calloc(table->size, sizeof *table->items); // NOLINT sizeof ptr
  return table;
}

// Frees an item
void ht_free_item(HashTableItem* item) {
  free(item->key);
  free(item);
}

// Frees the whole hashtable
void ht_free(HashTable* table) {
  // Frees the table
  for (size_t i = 0; i < table->size; i++) {
    HashTableItem* item = table->items[i];
    while (item) {
      HashTableItem* next = item->next;
      ht_free_item(item);
      item = next;
    }
  }
  free(table->items);
  free(table);
}

size_t hash_function(HashTable* table, const char* str) {
  size_t sum = 0;
  for (size_t j = 0; str[j]; ++j) sum += str[j];
  return sum % table->size;
}

// Inserts (or updates if exists) an item
void ht_insert(HashTable* table, ht_key_t key, ht_value_t value, ht_type_t type) {
  HashTableItem** slot = &table->items[hash_function(table, key)];
  HashTableItem*  item = *slot;
  if (!item) table->count++;  // HashTable accounting (while will not run)
  while (item) {
    if (strcmp(item->key, key) == 0) {
      item->value = value; // exists, update value
      item->type = type;
      return;
    }
    slot = &item->next;
    item = *slot;
  }
  *slot = ht_create_item(key, value, type);
}

void ht_update(HashTable* table, ht_key_t key, ht_type_t type) {
    HashTableItem** slot = &table->items[hash_function(table, key)];
    HashTableItem*  item = *slot;
    while (item) {
        if (strcmp(item->key, key) == 0) {
            item->type = type;
            return;
        }
        slot = &item->next;
        item = *slot;
    }
}

void ht_insert_value(HashTable* table, ht_key_t key, ht_value_of value_of) {
    HashTableItem** slot = &table->items[hash_function(table, key)];
    HashTableItem*  item = *slot;
    while (item) {
        if (strcmp(item->key, key) == 0) {
            item->value_of = value_of;
            return;
        }
        slot = &item->next;
        item = *slot;
    }
}

// Deletes an item from the table
void ht_delete(HashTable* table, ht_key_t key) {
  HashTableItem** slot = &table->items[hash_function(table, key)];
  HashTableItem*  item = *slot;
  bool is_direct_slot = true;
  while (item) {
    if (strcmp(item->key, key) == 0) {
      *slot = item->next; // remove item from linked list
      ht_free_item(item);
      if (is_direct_slot) --table->count; // HashTable accounting
      return;
    }
    slot = &item->next;
    item = *slot;
    is_direct_slot = false;
  }
}

// Searches the key in the hashtable
// and returns NULL if it doesn't exist
HashTableItem* ht_search(HashTable* table, ht_key_t key) {
  HashTableItem* item = table->items[hash_function(table, key)];

  while (item) {
    if (strcmp(item->key, key) == 0) return item;
    item = item->next;
  }
  return NULL;
}

void ht_print(HashTable* table) {
  printf("\n---- Hash Table ---\n");
  for (size_t i = 0; i < table->size; i++) {
    printf("@%zu: ", i);
    HashTableItem* item = table->items[i];
    while (item) {
      printf("%s => %d : %d  - %s| ", item->key, item->value, item->type, item->value_of);
      item = item->next;
    }
    printf("\n");
  }
  printf("-------------------\n");
}

void ht_print_search(HashTable* table, ht_key_t key) {
  HashTableItem* val;
  if ((val = ht_search(table, key)) == NULL) {
    printf("Key:%s does not exist\n", key);
    return;
  }
  printf("Key:%s => %d\n", key, val->value);
}
