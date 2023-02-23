#include <iostream>
#include <sys/mman.h>

using namespace std;

typedef struct __mmalloc_t
{
    int hdr_size;
} mmalloc_t;

typedef struct __mmfree_t
{
    int size;
    struct __mmfree_t *next;
} mmfree_t;

mmfree_t *head; // free list head

// traverse the free space list starting from the head
// printing out info (for debugging)
void traverse_free_list()
{
    // debugging - error handling
    if (head == NULL)
    {
        cout << "\n Empty free list - 'traverse_free_list()' \n";
        return;
    }
    mmfree_t *temp = head;
    cout << "\n\n------------------------\n";
    while (temp != NULL)
    {
        cout << endl;
        cout << "Current:" << (void *)temp << endl;
        cout << "Size:" << temp->size << endl;
        temp = temp->next;
    }
    cout << "\n------------------------\n\n";
    return;
}

// checks if two nodes are adjacent (or can be coalesced)
bool isAdjacent(mmfree_t *n1, mmfree_t *n2)
{
    // debugging - error handling
    if (n1 == NULL || n2 == NULL)
    {
        // cout << "Null nodes can't be merged - 'isAdjacent()' \n";
        return 0;
    }

    int n1_size = n1->size; // n1 free node size
    int n2_size = n2->size; // n2 free node size

    long int n1_addr = (long int)((void *)n1); // n1 free node address
    long int n2_addr = (long int)((void *)n2); // n2 free node address

    // check if the two nodes are adjacent
    if ((n1_addr + n1_size) == n2_addr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// merge two free nodes
mmfree_t *merge(mmfree_t *n1, mmfree_t *n2)
{
    // check if two nodes are adjacent
    // and perform the merge
    if (isAdjacent(n1, n2))
    {
        n1->size = n1->size + n2->size;
        n1->next = n2->next;
        n2 = NULL;
    }

    // debugging for non-adjacent nodes
    // else
    // {
    //     cout << (void *)n1 << " and " << (void *)n2;
    //     cout << " are not adjacent.\n";
    // }

    return n1;
}

// coalesces nodes in the free list
void coalesce_list(mmfree_t *prev, mmfree_t *curr, mmfree_t *next)
{
    if (isAdjacent(prev, curr))
    {
        // merge a node with its previous node
        prev = merge(prev, curr);
    }

    if (isAdjacent(prev, next))
    {
        // check if next node can be merged
        // on already merged node and merge
        merge(prev, next);
    }
    else if (isAdjacent(curr, next))
    {
        // try merging a node to its next node
        merge(curr, next);
    }
}

// called by free
// locate the freed memory insert position
// so free space nodes are sorted by address
void find_sorted_location(mmfree_t *new_free_node)
{
    // debugging - error handling
    if (new_free_node == NULL || head == NULL)
    {
        cout << "Null pointer error - 'find_sorted_location()' \n";
        return;
    }

    long int head_addr = (long int)((void *)head);              // int address of head
    long int new_free_addr = (long int)((void *)new_free_node); // int address of updated freed space

    // check to see if free_node comes before head
    if (new_free_addr < (long int)(head_addr))
    {
        new_free_node->next = head;
        head = new_free_node;
        coalesce_list(head, head->next, head->next->next);
        return;
    }

    mmfree_t *tmp = head; // head of free list

    // add free node to proper position in list
    while (tmp->next != NULL)
    {
        if (new_free_addr <= (long int)((void *)tmp->next))
        {
            new_free_node->next = tmp->next;
            tmp->next = new_free_node;
            coalesce_list(tmp, new_free_node, new_free_node->next);
            break;
        }
        tmp = tmp->next;
    }

    return;
}

// called by malloc after splitting
// update an old node in the free list to a new one
void update_free_list(mmfree_t *new_free_node, mmfree_t *old_free_node)
{
    // debugging - error handling
    if (old_free_node == NULL || new_free_node == NULL || head == NULL)
    {
        cout << "Null pointer error - 'update_free_list()' \n";
        return;
    }

    long int old_free_addr = (long int)((void *)old_free_node); // int address of old freed space

    mmfree_t *tmp = head; // head of free list

    // find an old node and update it to new one
    while (tmp->next != NULL)
    {
        // is current node (tmp) an old one
        if (old_free_addr == (long int)((void *)tmp->next))
        {
            tmp->next = new_free_node;
            if (tmp->next->next != NULL)
            {
                new_free_node->next = tmp->next->next;
            }
            coalesce_list(tmp, new_free_node, new_free_node->next);
            break;
        }
        tmp = tmp->next;
    }

    return;
}

// called by malloc
// find a free space chunk large enough for the requested allocation
// obtain some memory from that chunk
void *locate_split(int size)
{
    // debugging - error handling
    if (head == NULL)
    {
        cout << "\n Empty free list - 'locate_split()' \n";
        return NULL;
    }

    mmfree_t *temp = head; // head of free list

    // traverse the free list & look for a free block
    // where block size >= size + memory header size and free header size
    while (temp != NULL)
    {
        if (temp->size >= (size + sizeof(mmfree_t) + sizeof(mmalloc_t)))
        {
            // exit loop with the first free block of required size
            break;
        }
        temp = temp->next;
    }

    // Check if reached at the end of list without a free block
    if (temp == NULL)
    {
        cout << "Not enough contiguous space of " << size << " bytes. \n";
        return NULL;
    }

    // store current free block address and size
    void *curr = (void *)temp;
    int curr_free = temp->size;

    // create a new memory block header at first split address
    mmalloc_t *new_mem_header = (mmalloc_t *)(curr);
    new_mem_header->hdr_size = size;

    // Pointer to the about-to-be returned memory block:
    // Address of memory block header + size of the header itself
    long int curr_addr = (long int)(curr);
    void *mem_ptr = (void *)(curr_addr + sizeof(mmalloc_t));

    // update value of free block that is being split
    // to the new freed address (at split location)
    void *new_free_addr = (void *)((long int)mem_ptr + (long int)size);
    mmfree_t *new_free_node = (mmfree_t *)new_free_addr;
    new_free_node->size = curr_free - (size + sizeof(mmalloc_t));
    new_free_node->next = NULL;

    // update free list
    if ((void *)new_mem_header == (void *)head)
    {
        if (head->next != NULL)
        {
            new_free_node->next = head->next;
        }

        head = new_free_node;
    }
    else
    {
        update_free_list(new_free_node, temp);
    }

    // debugging
    cout << "\nAllocated ";
    cout << size + 4 << " bytes at ";
    cout << (void *)curr_addr << endl;
    traverse_free_list();

    // return pointer to memory
    return mem_ptr;
}

// malloc
void *mem_manager_malloc(int size)
{
    // debugging - error handling
    if (head == NULL)
    {
        cout << "\n Empty free list - 'mem_manager_malloc()' \n";
        return NULL;
    }

    // perform split and return memory address
    return (void *)locate_split(size);
}

void mem_manager_free(void *ptr)
{
    // error handling
    if (ptr == NULL)
    {
        cout << "Null pointer error - 'mem_manager_free()' \n";
        return;
    }

    int mem_total = 0, count = 0; // total memory size allocated at ptr & counter for loop
    void *hptr = ptr;             // starting address of allocated memory (header pointer)
    void *tmp = NULL;             // for temporarily storing an address in loop

    // subtract header size to get to header pointer
    long int curr_addr = (long int)ptr - sizeof(mmalloc_t);
    hptr = (void *)curr_addr;

    // Get memory block size at ptr from header
    mmalloc_t *hdr = (mmalloc_t *)(hptr);
    mem_total = hdr->hdr_size + sizeof(mmalloc_t);

    // nullify (free) the memory block
    while (count < mem_total)
    {
        tmp = (void *)((long int)hptr + 1);
        ptr = NULL;
        ptr = tmp;
        count += 1;
    }

    // create new free node to add to free list
    mmfree_t *new_free_node = (mmfree_t *)(hptr);
    new_free_node->next = NULL;
    new_free_node->size = mem_total;

    // insert free node to free list
    find_sorted_location(new_free_node);

    // debugging
    cout << "\nFreed space of ";
    cout << mem_total << " bytes at ";
    cout << (void *)new_free_node << endl;
    traverse_free_list();

    return;
}

void init_mem(int free_space_size)
{
    // Get memory chunk from OS
    void *mem_chunk = mmap(
        NULL,
        free_space_size,
        PROT_READ | PROT_WRITE,
        MAP_ANON | MAP_PRIVATE,
        -1,
        0);

    // instantiate free list with head at start of mmap'ed chunk
    head = (mmfree_t *)(mem_chunk);
    head->size = free_space_size - sizeof(mmfree_t);
    head->next = NULL;

    // debugging
    traverse_free_list();

    return;
}

int main()
{
    init_mem(4096);

    void *num1 = mem_manager_malloc(17);  // malloc 21 bytes
    void *num2 = mem_manager_malloc(18);  // malloc 22 bytes
    void *num3 = mem_manager_malloc(40);  // malloc 44 bytes
    void *num4 = mem_manager_malloc(50);  // malloc 54 bytes
    void *num5 = mem_manager_malloc(100); // malloc 104 bytes
    void *num6 = mem_manager_malloc(16);  // malloc 20 bytes

    mem_manager_free(num3); // free 44 bytes
    mem_manager_free(num4); // free 54 bytes
    mem_manager_free(num5); // free 104 bytes
    mem_manager_free(num6); // free 20 bytes

    void *num7 = mem_manager_malloc(78); // malloc 82 bytes

    mem_manager_free(num1); // free 21 bytes
    mem_manager_free(num7); // free 82 bytes

    void *num8 = mem_manager_malloc(16); // malloc 20 bytes

    mem_manager_free(num2); // free 22 bytes
    mem_manager_free(num8); // free 20 bytes

    void *num11 = mem_manager_malloc(17);  // malloc 21 bytes
    void *num21 = mem_manager_malloc(18);  // malloc 22 bytes
    void *num31 = mem_manager_malloc(400); // malloc 404 bytes

    mem_manager_free(num31); // free 404 bytes

    void *num41 = mem_manager_malloc(50); // malloc 54 bytes

    mem_manager_free(num41); // free 54 bytes
    mem_manager_free(num11); // free 21 bytes
    mem_manager_free(num21); // free 22 bytes

    return 0;
}
