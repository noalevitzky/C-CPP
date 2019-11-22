#include <stdio.h>
#include <stdlib.h> // for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> //for strlen
#include <errno.h>
#include "queue.h" // for BFS queue implementation

#define LINE_MAX 1024

/** num of args including program name */
#define NUM_ARGS 4
#define BASE 10

#define NO_CHILDREN '-'

/** returning values to indicate special errors */
#define MEMORY_ALL_FAILED -2
#define FILE_DONT_EXIST -3
#define NOT_A_TREE -4
#define NULL_VALUE -1

/** delim for strtok */
const char *delim = " \r\n\0";

/** infinity for BFS */
const size_t inf = 100000000;


/** represents a node in a graph, which has key value (unsigned int), and array of children keys */
typedef struct Node
{
    size_t key;
    size_t numOfChildren;
    struct Node **children;     // array of children (represented by there key)
    struct Node *parent;
} Node;

/**
 *
 * @param p pointer to key, which ends when char == '\0'
 * @return if key valid (unsigned int), else -1
 */
int isKeyValid(char *p)
{
    // illegal key value of type 0xxx (where x != ' ')
    if (*p == '0' && strchr(delim, *(p++)))
    {
        return EXIT_FAILURE;
    }

    // validate each char is digit
    while (strchr(delim, *p) == NULL)
    {
        if (*p < '0' || *p > '9')
        {
            return EXIT_FAILURE;
        }
        p++;
    }
    return EXIT_SUCCESS;
}

/**
 * update provided pointer num of vertex (as appears in first line of txt)
 * @param line first line of txt
 * @param numV pointer to be updated
 * @return if the process was successful
 */
int getVertexNum(char *line, size_t *numV)
{
    char *value = strtok(line, delim);
    if (value == NULL)
    {
        return EXIT_FAILURE;
    }

    // validate value
    int isErr = isKeyValid(value);
    if (isErr == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    // convert to size_t
    char *end = NULL;
    size_t num = strtol(value, &end, BASE);
    if (!strchr(delim, *end))
    {
        return EXIT_FAILURE;
    }

    // validate only 1 num was inserted
    char *next = strtok(NULL, delim);
    if (next != NULL)
    {
        return EXIT_FAILURE;
    }

    *numV = num;
    return EXIT_SUCCESS;
}

/**
 * add child to node children list and update child's parent to be the node
 * user need to free children allocation!
 * @param childKey
 * @param node
 * @param indexOfChild out of all node children
 * @param pNode
 * @return if the process was successful
 */
int updateChild(const size_t childKey, Node *node, const size_t indexOfChild, Node **pNode)
{
    if (childKey == 0 && errno != 0)
    {
        // unspecified error occured
        return EXIT_FAILURE;
    }

    // update node
    node->children = (Node **) realloc(node->children, (indexOfChild + 1) * sizeof(Node *));
    if (node->children == NULL)
    {
        return MEMORY_ALL_FAILED;
    }

    // update node
    node->children[indexOfChild] = &((*pNode)[childKey]);

    // update child
//    if (((*pNode)[childKey]).parent != NULL)
//    {
//        return NOT_A_TREE;
//    }
    ((*pNode)[childKey]).parent = node;

    return EXIT_SUCCESS;
}

/**
 * update node children array to hold children's pointers
 * @param line
 * @param nodeKey
 * @param pNode
 * @param numV
 * @return if the process was successful
 */
int processChildren(char *line, size_t nodeKey, Node **pNode, const size_t numV)
{
    int isErr = 0;
    char *p = NULL;
    size_t childKey = 0;    // key value of child
    size_t indexOfChild = 0;   // num of child out of node children
    char *end; // pointer to additional chars in line
    Node *node = &((*pNode)[nodeKey]);

    // get first key
    p = strtok(line, delim);
    if (p == NULL)
    {
        return EXIT_FAILURE;
    }

    // validate key
    isErr = isKeyValid(p);
    if (isErr == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    // update first child in childrenKey array
    childKey = strtol(p, &end, BASE);
    if (childKey >= numV)
    {
        return EXIT_FAILURE;
    }

    isErr = updateChild(childKey, node, indexOfChild, pNode);
    if (isErr == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
//    else if (isErr == NOT_A_TREE)
//    {
//        return NOT_A_TREE;
//    }
    else if (isErr == MEMORY_ALL_FAILED)
    {
        return MEMORY_ALL_FAILED;
    }

    // get next
    indexOfChild++;
    p = strtok(NULL, delim);

    // recursively validate each key, and update each child
    while (p != NULL)
    {
        // validate num lines
        if (indexOfChild >= numV)
        {
            return EXIT_FAILURE;
        }

        // validate key
        isErr = isKeyValid(p);
        if (isErr == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }

        // update first child in childrenKey array
        childKey = (int) strtol(p, &end, BASE);
        isErr = updateChild(childKey, node, indexOfChild, pNode);
        if (isErr == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
//        else if (isErr == NOT_A_TREE)
//        {
//            return NOT_A_TREE;
//        }
        else if (isErr == MEMORY_ALL_FAILED)
        {
            return MEMORY_ALL_FAILED;
        }

        // get next
        indexOfChild++;
        p = strtok(NULL, delim);
    }
    // end of process
    node->numOfChildren = indexOfChild;
    return EXIT_SUCCESS;
}

/**
 * create node by provided information
 * @param line represents children keys, separated by space
 * @param nodeKey
 * @param numV number of vertex
 * @return if the process was successful or not
 */
int line2Node(char *line, const size_t nodeKey, Node **pNode, const size_t numV)
{
    if (nodeKey >= numV)
    {
        return EXIT_FAILURE;
    }

    // check if line is starting with a space (illegal and is not covered by strtok)
    if (line[0] == ' ')
    {
        return EXIT_FAILURE;
    }

    // check if node is a leaf
    if (line[0] == NO_CHILDREN && strchr(delim, line[1]) != NULL)
    {
        return EXIT_SUCCESS;
    }

    // update children
    int isErr = processChildren(line, nodeKey, pNode, numV);
    if (isErr == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
//    else if (isErr == NOT_A_TREE)
//    {
//        return NOT_A_TREE;
//    }
    else if (isErr == MEMORY_ALL_FAILED)
    {
        return MEMORY_ALL_FAILED;
    }

    return EXIT_SUCCESS;
}

/**
 * update node fields
 * @param n num of nodes in graph
 * @param pNode pointer to nodes' array
 * @return if the process was successful
 */
int updateNodesData(const size_t n, Node **pNode)
{
    for (size_t i = 0; i < n; i++)
    {
        (*pNode)[i].key = i;
        (*pNode)[i].children = NULL;
        (*pNode)[i].parent = NULL;
        (*pNode)[i].numOfChildren = 0;
    }
    return EXIT_SUCCESS;
}


/**
 * process given vertex- check if the input is valid, and assign values to given pointers.
 * @param uStr str representation of first vertex key
 * @param vStr str representation of second vertex key
 * @param uKey pointer to first vertex value
 * @param vKey pointer to second vertex value
 * @return if the process was successful
 */
int vertexProcess(const char *uStr, const char *vStr, size_t *uKey, size_t *vKey)
{
    // validate strings that represent vertex keys don't start with spaces
    // (is not covered by strtol)
    if (uStr[0] == ' ' || vStr[0] == ' ')
    {
        // given vertex start with a space
        return EXIT_FAILURE;
    }

    // initialize pointer to for strtol. if str is invalid (not only digits), strtol will assign
    // this pointer to point at the first not converted char --> thus won't be null.
    char *end = NULL;

    *uKey = strtol(uStr, &end, BASE);
    if (!strchr(delim, *end))
    {
        // given vertex is not representing an unsigned int (vertex key)
        return EXIT_FAILURE;
    }

    *vKey = strtol(vStr, &end, BASE);

    // check given vertex is representing an unsigned int (vertex key)
    if (!strchr(delim, *end))
    {
        // there is an unprocessed char in str- thus not an int
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * find node with no parent -> root og a tree
 * @param allNodes all nodes in tree
 * @param numV number of nodes in tree
 * @return root
 */
int findRootOfTree(Node **allNodes, const size_t numV, Node **root)
{
    size_t i = 0;
    for (; i < numV; i++)
    {
        Node *node = &(*allNodes)[i];
        if (node->parent == NULL)
        {
            *root = node;
            return EXIT_SUCCESS;
        }
    }
    return NOT_A_TREE;
}

/**
 * helper for the DFS calculation.
 * @param allNodes all grph nodes
 * @param nodeIndex index of explored node
 * @param visited flag array, 1 is visited, else 0
 * @param pre holds previsit clock value for each node
 * @param post holds postvisit clock value for each node
 */
int explore(Node **allNodes, const size_t nodeIndex, size_t **visited, size_t **pre, size_t **post)
{
    static size_t clock = 0;

    // mark as visited
    (*visited)[nodeIndex] = 1;

    // previsit
    (*pre)[nodeIndex] = clock++;

    // initialized variables
    Node *node = &(*allNodes)[nodeIndex];

    // explore children
    size_t i = 0;
    for (; i < node->numOfChildren; i++)
    {
        Node *curChild = node->children[i];
        if (curChild->key == node->key)
        {
            // node points to itself, reflexive relations. not a tree
            return NOT_A_TREE;
        }

        if ((*visited)[curChild->key] == 0)
        {
            int isErr = explore(allNodes, curChild->key, visited, pre, post);
            if (isErr == NOT_A_TREE)
            {
                return NOT_A_TREE;
            }
        }
    }

    // postvisit
    (*post)[nodeIndex] = clock++;
    return EXIT_SUCCESS;
}

/**
 *
 * @param allNodes array of all nodes in graph
 * @param numV number of nodes in graph
 * @param pre previsit timestamps (after dfs)
 * @param post postvidit timestamps (after dfs)
 * @return success if there is no backward edge, failure otherwise
 */
int findBackwardEdge(Node **allNodes, const size_t numV, size_t **pre, size_t **post)
{
    // go over all nodes in tree (u) and their neighbors (v)
    size_t uKey = 0;
    for (; uKey < numV; uKey++)
    {
        Node u = (*allNodes)[uKey];
        size_t i = 0;
        for (; i < u.numOfChildren; i++)
        {
            // v = allNodes[vKey];
            // check if edge (u,v) is a backward one
            size_t childKey = u.children[i]->key;
            if ((*pre)[childKey] < (*pre)[uKey] && (*post)[uKey] < (*post)[childKey])
            {
                return NOT_A_TREE;
            }
        }
    }
    // no backward edges- no cycles
    return EXIT_SUCCESS;
}

/**
 * runs the dfs algorithm
 * @param allNodes
 * @param numV
 * @param visited
 * @param pre
 * @param post
 * @return if the process was successful
 */
int dfs(Node **allNodes, const size_t numV, size_t **visited, size_t **pre, size_t **post)
{
    size_t i = 0;
    // go over all nodes in graph
    for (; i < numV; i++)
    {
        if ((*visited)[i] == 0) // if not visited
        {
            int isErr = explore(allNodes, i, visited, pre, post);
            if (isErr == NOT_A_TREE)
            {
                return NOT_A_TREE;
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * using DFS algorithm, we search if there are cycles in graph --> thus not a tree
 * @param allNodes all nodes in graph
 * @param numV number of nodes in graph
 * @return success if there are no cycles, failure otherwise
 */
int findCycles(Node **allNodes, const size_t numV)
{
    // initialized visited flags
    int isErr = 0;
    size_t *visited = (size_t *) calloc(numV, sizeof(size_t));
    size_t *preVisit = (size_t *) calloc(numV, sizeof(size_t));
    size_t *postVisit = (size_t *) calloc(numV, sizeof(size_t));
    if (visited == NULL || preVisit == NULL || postVisit == NULL)
    {
        free(postVisit);
        free(preVisit);
        free(visited);
        return MEMORY_ALL_FAILED;
    }

    isErr = dfs(allNodes, numV, &visited, &preVisit, &postVisit);
    if (isErr == NOT_A_TREE)
    {
        // there is a backwards edge in graph --> there's a cycle --> graph is not a tree
        free(postVisit);
        free(preVisit);
        free(visited);
        return NOT_A_TREE;
    }

    // find out if there's a cycle (backward edge)
    isErr = findBackwardEdge(allNodes, numV, &preVisit, &postVisit);
    if (isErr == NOT_A_TREE)
    {
        // there is a backwards edge in graph --> there's a cycle --> graph is not a tree
        free(postVisit);
        free(preVisit);
        free(visited);
        return NOT_A_TREE;
    }

    // no backward edges
    free(postVisit);
    free(preVisit);
    free(visited);
    return EXIT_SUCCESS;
}

/**
 * update numE pointer to actual graph num of edges
 * @param pNode all nodes in graph
 * @param numV number of nodes in graph
 */
void countEdges(Node **pNode, const size_t numV, size_t *numE)
{
    // init edges counter
    size_t count = 0;

    size_t i = 0;
    for (; i < numV; i++)
    {
        count += (*pNode)[i].numOfChildren;
    }
    *numE = count;
}

/**
 * print all tree variables
 * @param allNodes all nodes in tree
 * @param tree
 */
void printResult(const size_t rootKey, const size_t numV, const size_t numE, const size_t minH,
                 const size_t maxH, const int diameter, const size_t uKey, const size_t vKey,
                 size_t **path, const size_t pathLength)
{
    printf("Root Vertex: %zu\n", rootKey);
    printf("Vertices Count: %zu\n", numV);
    printf("Edges Count: %zu\n", numE);
    printf("Length of Minimal Branch: %zu\n", minH);
    printf("Length of Maximal Branch: %zu\n", maxH);
    printf("Diameter Length: %d\n", diameter);
    printf("Shortest Path Between %zu and %zu:", uKey, vKey);

    // check if path is empty
    if (*path == NULL)
    {
        printf(" %d", 0);
    }
    else
    {
        size_t i = 0;
        for (; i < pathLength; i++)
        {
            printf(" %zu", (*path)[i]);
        }
    }
    printf("\n");
}

/**
 * runs BFS algorithm
 * @param allNodes
 * @param start
 * @param dist pointer to distance array to be updated
 * @param prev pointer to previous node array to be updated
 * @return if the process was successful
 */
int bfs(Node **allNodes, size_t start, size_t **dist, int **prev)
{
    (*dist)[start] = 0;
    (*prev)[start] = NULL_VALUE; // indicates that the start node don't have a prev

    // create queue
    Queue *q = allocQueue();
    if (q == NULL)
    {
        return MEMORY_ALL_FAILED;
    }

    enqueue(q, start);
    while (!queueIsEmpty(q))
    {
        size_t uKey = dequeue(q);
        Node *u = &(*allNodes)[uKey];

        // go over all edges (u,w)
        size_t i = 0;
        for (; i < u->numOfChildren; i++)
        {
            Node *w = u->children[i];
            size_t wKey = w->key;
            if ((*dist)[wKey] == inf)
            {
                enqueue(q, wKey);
                (*prev)[wKey] = uKey;
                (*dist)[wKey] = (*dist)[uKey] + 1;
            }
        }
        // go over parent if not null
        Node *parent = u->parent;
        if (parent != NULL)
        {
            size_t pKey = parent->key;
            if ((*dist)[pKey] == inf)
            {
                enqueue(q, pKey);
                (*prev)[pKey] = uKey;
                (*dist)[pKey] = (*dist)[uKey] + 1;
            }
        }

    }
    freeQueue(&q);
    return EXIT_SUCCESS;
}

/**
 *
 * @param allNodes
 * @param numV
 * @param root
 * @param minH pointer to be updated
 * @return if process was successful
 */
int calcMinH(Node **allNodes, const size_t numV, const Node *root, size_t *minH)
{
    // initialize data structures for BFS
    size_t *dist = (size_t *) malloc(numV * sizeof(size_t));
    int *prev = (int *) calloc(numV, sizeof(int));
    if (dist == NULL || prev == NULL)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // init dist to infinity
    for (size_t i = 0; i < numV; i++)
    {
        dist[i] = inf;
    }

    int isErr = bfs(allNodes, root->key, &dist, &prev);
    if (isErr == MEMORY_ALL_FAILED)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // find minH
    size_t min = inf;
    size_t i = 0;
    while (i < numV)
    {
        // find shortest dist, where node is a leaf
        if (dist[i] < min && (*allNodes)[i].numOfChildren == 0)
        {
            min = dist[i];
        }
        i++;
    }

    free(prev);
    free(dist);
    *minH = min;
    return EXIT_SUCCESS;
}

/**
 *
 * @param allNodes
 * @param numV num of vertex in graph
 * @param root
 * @return maximum distance from root
 */
int calcMaxH(Node **allNodes, const size_t numV, Node *root, size_t *max)
{
    size_t *dist = (size_t *) malloc(numV * sizeof(size_t));
    int *prev = (int *) calloc(numV, sizeof(int));
    if (dist == NULL || prev == NULL)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // init dist to infinity
    for (size_t i = 0; i < numV; i++)
    {
        dist[i] = inf;
    }

    int isErr = bfs(allNodes, root->key, &dist, &prev);
    if (isErr == MEMORY_ALL_FAILED)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // find manH
    size_t maxSize = 0;
    size_t i = 0;
    while (i < numV)
    {
        // find longest dist, where node is a leaf
        if (dist[i] > maxSize && (*allNodes)[i].numOfChildren == 0)
        {
            maxSize = dist[i];
        }
        i++;
    }

    free(prev);
    free(dist);
    *max = maxSize;
    return EXIT_SUCCESS;
}

/**
 * calc diameter by using BFS algorithm
 * @param allNodes
 * @param root
 * @param numV
 * @param diameter pointer to be updated
 * @return if process is successful
 */
int calcDiameter(Node **allNodes, Node *root, const size_t numV, int *diameter)
{
    size_t *dist = (size_t *) malloc(numV * sizeof(size_t));
    int *prev = (int *) calloc(numV, sizeof(int));
    if (dist == NULL || prev == NULL)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // init sit to infinity
    for (size_t i = 0; i < numV; i++)
    {
        dist[i] = inf;
    }

    int isErr = bfs(allNodes, root->key, &dist, &prev);
    if (isErr == MEMORY_ALL_FAILED)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // find furthest node index from root
    size_t furthest = 0;
    size_t max = 0;
    size_t i = 0;
    while (i < numV)
    {
        // find longest dist, where node is a leaf
        if (dist[i] > max && (*allNodes)[i].numOfChildren == 0)
        {
            max = dist[i];
            furthest = i;
        }
        i++;
    }
    free(prev);
    free(dist);

    // find the largest distance to furthestNode
    // assigned to diameter pointer
    size_t *dist2 = (size_t *) malloc(numV * sizeof(size_t));
    int *prev2 = (int *) calloc(numV, sizeof(int));
    if (dist2 == NULL || prev2 == NULL)
    {
        free(prev2);
        free(dist2);
        return MEMORY_ALL_FAILED;
    }

    // init dist2t to infinity
    for (size_t j = 0; j < numV; j++)
    {
        dist2[j] = inf;
    }

    isErr = bfs(allNodes, furthest, &dist2, &prev2);
    if (isErr == MEMORY_ALL_FAILED)
    {
        free(prev2);
        free(dist2);
        return MEMORY_ALL_FAILED;
    }

    // find diameter
    size_t val = 0;
    size_t j = 0;
    while (j < numV)
    {
        // find longest dist, where node is a leaf
        if (dist2[j] > val)
        {
            val = dist2[j];
        }
        j++;
    }
    free(prev2);
    free(dist2);
    *diameter = val;
    return EXIT_SUCCESS;
}

/**
 * user should free [*path] allocation outside
 * @param allNodes
 * @param numV
 * @param uKey
 * @param vKey
 * @param dist
 * @param path
 * @return
 */
int calcShortestPath(Node **allNodes, const size_t numV, const size_t uKey,
                     const size_t vKey, size_t *pathLength, size_t **path)
{
    size_t *dist = (size_t *) malloc(numV * sizeof(size_t));
    int *prev = (int *) calloc(numV, sizeof(int));
    if (dist == NULL || prev == NULL)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // init dist to infinity
    for (size_t i = 0; i < numV; i++)
    {
        dist[i] = inf;
    }

    // run BFS to find shortest path from u to v
    int isErr = bfs(allNodes, uKey, &dist, &prev);
    if (isErr == MEMORY_ALL_FAILED)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    // update u-v dist as been found by BFS
    *pathLength = dist[vKey] + 1;
    *path = (size_t *) calloc(*pathLength, sizeof(size_t));
    if (*path == NULL)
    {
        free(prev);
        free(dist);
        return MEMORY_ALL_FAILED;
    }

    size_t i = *pathLength - 1;
    size_t key = vKey;
    while (i > 0)
    {
        (*path)[i] = key;
        key = prev[key];
        i--;
    }
    (*path)[0] = uKey;

    free(prev);
    free(dist);
    return EXIT_SUCCESS;
}

/**
 * free all allocations
 * @param allNodes nodes array to free (each node's children list should be freed as well
 * @param path between given vertex, to be freed
 * @param numV number of vertex
 */
void freeAllFromMain(Node **allNodes, size_t **path, const size_t numV)
{
    // free all nodes' children allocation
    size_t i = 0;
    for (; i < numV; i++)
    {
        free((*allNodes)[i].children);
    }
    // free nodes'array and path
    free(*allNodes);
    free(*path);
}

/**
 * program to validate a given graph (in text file) is a tree, provide information about the
 * tree (num of vertex, root key, min and max heights, diameter), and shortest path between the 2
 * given vertex.
 * @param argc num of arguments (should be equal to NUM_ARGS)
 * @param argv given arguments (text file path, and 2 vertex)
 * @return if the process was successful
 */
int main(int argc, const char *argv[])
{
    // initialize tree variables
    size_t numV = 0; // num of vertex
    size_t numE = 0; // num of edges
    size_t minH = 0; // min height
    size_t maxH = 0; // max height
    int diameter = 0;
    size_t pathLength = 0;
    int error = 0;

    if (argc != NUM_ARGS)
    {
        fprintf(stderr, "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n");
        return EXIT_FAILURE;
    }

    // initialize arguments pointers, path and root pointers
    const char *filePath = argv[1];
    const char *uStr = argv[2];
    const char *vStr = argv[3];
    size_t *path = NULL;
    Node *root = NULL;

    // process file- init pointers for txt analyze
    FILE *fp = NULL;
    char line[LINE_MAX];
    int isErr = 0;

    fp = fopen(filePath, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Invalid input\n");
        return FILE_DONT_EXIST;
    }

    // read first line & process vertex num. update numV & allocate nodes array
    if (fgets(line, LINE_MAX, fp) == NULL)
    {
        fclose(fp);
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    isErr = getVertexNum(line, &numV);
    if (isErr == EXIT_FAILURE || numV == 0)
    {
        fclose(fp);
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    Node *allNodes = (Node *) malloc((numV) * sizeof(Node));
    if (allNodes == NULL)
    {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return MEMORY_ALL_FAILED;
    }

    isErr = updateNodesData(numV, &allNodes);
    if (isErr == MEMORY_ALL_FAILED)
    {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    // read rest of lines
    size_t nodeKey = 0;
    while (fgets(line, LINE_MAX, fp) != NULL)
    {
        if (nodeKey >= numV)
        {
            fclose(fp);
            fprintf(stderr, "Invalid input\n");
            freeAllFromMain(&allNodes, &path, numV);
            return EXIT_FAILURE;
        }

        isErr = line2Node(line, nodeKey, &allNodes, numV);
        if (isErr == EXIT_FAILURE)
        {
            fclose(fp);
            fprintf(stderr, "Invalid input\n");
            freeAllFromMain(&allNodes, &path, numV);
            return EXIT_FAILURE;
        }
//        else if (isErr == NOT_A_TREE)
//        {
//            fclose(fp);
//            fprintf(stderr, "The given graph is not a tree\n");
//            freeAllFromMain(&allNodes, &path, numV);
//            return NOT_A_TREE;
//        }
        else if (isErr == MEMORY_ALL_FAILED)
        {
            fclose(fp);
            fprintf(stderr, "Memory allocation failed\n");
            freeAllFromMain(&allNodes, &path, numV);
            return MEMORY_ALL_FAILED;
        }
        nodeKey++;
    }

    if (nodeKey < numV)
    {
        fclose(fp);
        fprintf(stderr, "Invalid input\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }
    fclose(fp);

    // process given vertex, and check if they are in tree
    size_t uKey = 0;
    size_t vKey = 0;
    error = vertexProcess(uStr, vStr, &uKey, &vKey);
    if (error == EXIT_FAILURE)
    {
        fprintf(stderr, "Invalid input\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }
    else if (uKey >= numV || vKey >= numV) // not in tree
    {
        fprintf(stderr, "Invalid input\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    if (numV == 1)
    {
        error = findRootOfTree(&allNodes, numV, &root);
        if (error == NOT_A_TREE) // no root
        {
            fprintf(stderr, "The given graph is not a tree\n");
            freeAllFromMain(&allNodes, &path, numV);
            return EXIT_FAILURE;
        }

        printResult(root->key, numV, numE, minH, maxH, diameter, uKey, vKey, &path, pathLength);
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_SUCCESS;
    }

    // check if graph is a tree: validate graph has |v|-1 edges, and no cycles
    countEdges(&allNodes, numV, &numE);
    if (numE != numV - 1)
    {
        fprintf(stderr, "The given graph is not a tree\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    error = findCycles(&allNodes, numV);
    if (error == NOT_A_TREE)
    {
        fprintf(stderr, "The given graph is not a tree\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }
    else if (error == MEMORY_ALL_FAILED)
    {
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    // end of tree validation. the graph is a TREE, with 2 or more vertex. find tree root
    error = findRootOfTree(&allNodes, numV, &root);
    if (error == NOT_A_TREE)
    {
        fprintf(stderr, "The given graph is not a tree\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    error = calcMinH(&allNodes, numV, root, &minH);
    if (error == MEMORY_ALL_FAILED)
    {
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    error = calcMaxH(&allNodes, numV, root, &maxH);
    if (error == MEMORY_ALL_FAILED)
    {
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    error = calcDiameter(&allNodes, root, numV, &diameter);
    if (error == MEMORY_ALL_FAILED)
    {
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    error = calcShortestPath(&allNodes, numV, uKey, vKey, &pathLength, &path);
    if (error == MEMORY_ALL_FAILED)
    {
        fprintf(stderr, "Memory allocation failed\n");
        freeAllFromMain(&allNodes, &path, numV);
        return EXIT_FAILURE;
    }

    printResult(root->key, numV, numE, minH, maxH, diameter, uKey, vKey, &path, pathLength);
    freeAllFromMain(&allNodes, &path, numV);
    return EXIT_SUCCESS;
}