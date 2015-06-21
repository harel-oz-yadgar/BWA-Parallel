//Group.h

#define INITIAL_SIZE 5
#define GROWTH_FACTOR 10

typedef struct{
int** elements;
int numOfElements,maxElements;
}Group;

Group* newGroup();
void addElements(Group* g,int k,int l);
void shiftRight(Group* g,int index);
void shiftLeft(Group* g,int index);
void rebalance(Group* g,int index);
Group* unionGroups(Group* A,Group* B);
void freeMemory(Group* g);
void printGroup(Group* g);

