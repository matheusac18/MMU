#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#define MEM_SIZE 256
#define HASH_SIZE 65536

typedef struct hashNode
{

    uint8_t flag_cache:1;
    uint8_t flag_referenciada:1;
    uint8_t flag_modificada:1;
    uint8_t flag_protecao:3;
    uint8_t flag_padding:2; // inutilizado,apenas para preenchimento
    uint64_t offeset_moldura:12;
    uint64_t offeset_pagina:52;
    /*uint16_t offeset_moldura:12;
    uint16_t offeset_padding:4;*/ // inutilizado, apenas para preenchimento
}HashNode;

typedef struct hash{
    HashNode hashNode;
    struct hash *next;
}Hash;

Hash MapVirtual[HASH_SIZE];

void create_hash_table()
{
   for(int i = 0; i<HASH_SIZE;i++)
   {
       MapVirtual[i].next = NULL;
   } 
}

Hash *create_hash(HashNode new)
{
    Hash *node = (Hash*)malloc(sizeof(Hash));
    node->hashNode = new;
    node->next = NULL;
    return node;
}


int insertHash(HashNode new)
{
    int i = (new.offeset_pagina)%HASH_SIZE;
    Hash *node = &MapVirtual[i];
      
    while(node->next != NULL)//empty list
    {
        node = node->next;
    }
    node->next = create_hash(new);
    return 1;
}

void removeHash(HashNode r)
{
    int i = r.offeset_pagina%HASH_SIZE;
    Hash *node = MapVirtual[i].next;
    Hash *remove;
    while(node->next != NULL)
    {
        if(node->next->hashNode.offeset_pagina == r.offeset_pagina)
        {
            remove = node->next;
            node->next = node->next->next;
            free(remove);
            break;
        }
        node = node->next;
    }
}


Hash *search_map_virtual(uint64_t virtual_addr)
{
    int i = (virtual_addr>>12)%MEM_SIZE;
    Hash *node = MapVirtual[i].next;

    while(node != NULL)
    {
        if((node->hashNode.offeset_pagina) == (virtual_addr>>12))
            return node;
        node = node->next;
    }
    return NULL;
}

void print_page(Hash *node)
{
    if(node != NULL)
    {   
        printf("-----------Hash Position %d-----------\n",(int)(node->hashNode.offeset_pagina%HASH_SIZE));
        while(node != NULL)
        {
            printf("\tPage: %lu \n\tMoldura: %d \n\tFlag Cache: %d \n\tFlag Referencia: %d \n\tFlag Modificada: %d \n\tFlag Proteção: %d",
                    (uint64_t)node->hashNode.offeset_pagina, node->hashNode.offeset_moldura, node->hashNode.flag_cache, node->hashNode.flag_referenciada, 
                    node->hashNode.flag_modificada, node->hashNode.flag_protecao);
            
            node = node->next;
        }
        printf("\n\n");
    }
}

void print_page_search(Hash *node, uint64_t virtual_addr)
{
    if(node != NULL)
    {   
        printf("---------------Page Hit----------------\n");
        printf("-----------Hash Position: %d------------\n",(int)(node->hashNode.offeset_pagina%HASH_SIZE));

        printf("\tVirtual Address: %lu \n\tPage: %lu \n\tMoldura: %d \n\tFlag Cache: %d \n\tFlag Referencia: %d \n\tFlag Modificada: %d \n\tFlag Proteção: %d",
                    virtual_addr,(uint64_t)node->hashNode.offeset_pagina, node->hashNode.offeset_moldura, node->hashNode.flag_cache, node->hashNode.flag_referenciada, 
                    node->hashNode.flag_modificada, node->hashNode.flag_protecao);

        printf("\n\n");
    }
    else
    {
        printf("---------------Page Miss!---------------");
        printf("\n\tVirtual address: %lu\n",virtual_addr);

    }
}

void print_hash()
{
    for(int i = 0;i<HASH_SIZE;i++)
    {
        Hash *node = MapVirtual[i].next;
        print_page(node);
    }
}




int main()
{
    FILE *file;

    file = fopen("input_unique.txt","r");

    if(file == NULL)
        return -1;

    int inputs[60000];
    int i = 0;
    while(!feof(file))
    {
        fscanf(file, "%d",&inputs[i]);
        i++;
    }
    create_hash_table();
    /*for(i = 0;i<20;i++)
    {
        HashNode new;
        new.offeset_pagina = inputs[i]>>12;
        insertHash(new);
    }*/

    HashNode new;

    new.offeset_pagina = 120000;
    new.flag_cache = 1;
    new.flag_modificada = 0;
    new.flag_protecao = 7;
    new.flag_referenciada = 1;
    insertHash(new);

    new.offeset_pagina = 65537;
    new.flag_cache = 1;
    new.flag_modificada = 1;
    new.flag_protecao = 3;
    new.flag_referenciada = 1;
    insertHash(new);

    new.offeset_pagina = 1;
    new.flag_cache = 0;
    new.flag_modificada = 1;
    new.flag_protecao = 7;
    new.flag_referenciada = 1;
    insertHash(new);

    new.offeset_pagina = 15;
    new.flag_cache = 1;
    new.flag_modificada = 1;
    new.flag_protecao = 7;
    new.flag_referenciada = 1;
    insertHash(new);

    new.offeset_pagina = 1024;
    new.flag_cache = 0;
    new.flag_modificada = 0;
    new.flag_protecao = 5;
    new.flag_referenciada = 1;
    insertHash(new);


    new.offeset_pagina = 66000;
    new.offeset_pagina = 1024;
    new.flag_cache = 1;
    new.flag_modificada = 0;
    new.flag_protecao = 5;
    new.flag_referenciada = 1;
    insertHash(new);

    print_page_search(search_map_virtual(268442326),268442326);
    print_page_search(search_map_virtual(4525),4525);
    print_page_search(search_map_virtual(41000),41000);
    print_page_search(search_map_virtual(62000),62000);
    print_page_search(search_map_virtual(80000),80000);

    
    return 0;
}