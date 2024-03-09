#ifndef BSP_H_
#define BSP_H_

struct partition {
    int starti;
    int endi;
    int size;
} typedef struct partition Part;

// x = i / size
// y = i % size 


void bsp_init(Partition *p);
void bsp(Partition *p, int res);

#endif
