#include "hash.h"
#include <stdio.h>
#include <assert.h>

void test_hash()
{
    struct element *table = initTable();
    set(table, 10, 1000);
    set(table, 17, 1006);
    set(table, 1, 1006);
    set(table, 0, 1003);
    set(table, -210, -1004);
    set(table, 10, 1002);
    short int value;
    assert(get(table, 10, &value));
    assert(value == 1002);
    assert(value != 1000);
    assert(get(table, 0, &value));
    assert(value == 1003);
    assert(get(table, -210, &value));
    assert(value == -1004);
    assert(!delete(table, 4582));
    assert(delete(table, 10));
    assert(!get(table, 10, &value));
}

int main(int argc, char *argv[])
{
    test_hash();
    printf("everything well");
    return 0;
}
