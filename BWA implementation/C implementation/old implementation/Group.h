//Group.h

#define INITIAL_SIZE 5
#define GROWTH_FACTOR 10

typedef struct{
int* elements;
int numOfElements,maxElements;
}Group;

Group* newGroup();
void addElement(Group* g,int element);
void addElements(Group* g,int from,int to);
Group* unionGroups(Group* A,Group* B);
void freeMemory(Group* g);
void printGroup(Group* g);

