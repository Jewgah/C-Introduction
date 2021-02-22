


struct node_t {
    struct node_t* next;
    struct node_t* prev;
    void* ptr;
    unsigned int alloc_sz;
};

struct node_t ExtraBytes_LL = { .next = NULL, .prev = NULL, .ptr = NULL, .alloc_sz = 0x0 };

int insert_to_list(void* new_ptr, unsigned int new_alloc_sz)
{
    struct node_t* new_node = sys_malloc(sizeof(struct node_t));
    if (new_node == NULL) {
        return -1;
    }

    new_node->ptr = new_ptr;
    new_node->alloc_sz = new_alloc_sz;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (ExtraBytes_LL.next == NULL) {
        new_node->prev = &ExtraBytes_LL;
        new_node->next = &ExtraBytes_LL;
        ExtraBytes_LL.next = new_node;
        ExtraBytes_LL.prev = new_node;
    } else {
        new_node->next = ExtraBytes_LL.next;
        new_node->prev = &ExtraBytes_LL;
        ExtraBytes_LL.next->prev = new_node;
        ExtraBytes_LL.next = new_node;
    }

    return 0;
}

struct node_t* search_in_list(void* ptr)
{
    struct node_t* iter = ExtraBytes_LL.next;

    while (iter != NULL) {
        if (iter->ptr == ptr) {
            break;
        }

        iter = iter->next;
    }

    if (iter == NULL) {
        return NULL;
    }

    return iter;
}

int remove_from_list(void* ptr)
{
    struct node_t* iter = search_in_list(ptr);
    if (iter == NULL) {
        return -1;
    }

    iter->next->prev = iter->prev;
    iter->prev->next = iter->next;

    sys_free(iter, sizeof(struct node_t));

    return 0;
}

void* malloc(unsigned int num_bytes)
{
    void* alloc_mem = sys_malloc(num_bytes);

    if (alloc_mem == NULL) {
        return NULL;
    }

    int res = insert_to_list(alloc_mem, num_bytes);
    if (res == -1) {
        sys_free(alloc_mem, num_bytes);
        return NULL;
    }

    return alloc_mem;
}

void free(void* ptr)
{
    struct node_t* elem = search_in_list(ptr);

    if (elem == NULL) {
        return;
    }

    sys_free(ptr, elem->alloc_sz);

    remove_from_list(ptr);
}

