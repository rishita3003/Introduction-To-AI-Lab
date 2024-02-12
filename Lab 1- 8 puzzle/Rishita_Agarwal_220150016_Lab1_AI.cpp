#include <bits/stdc++.h>
#define ll long long
#define vll vector<long long>
using namespace std;
#define N 3

class Node{
public:
    // stores the parent node of the current node
    // helps in tracing path when the answer is found
    Node* parent;

    // stores matrix
    vector<vll> mat;

    // stores blank tile coordinates
    int x, y;

    // stores the number of misplaced tiles
    int cost;

    // stores the number of moves so far
    int level;

    // Constructor
    Node() : mat(N, vll(N)) {} // Initialize the matrix with size N x N
};


class puzzle_8{
    public:
    set<pair<vll,vll>> vecSam;
    set<pair<vector<vll>,vector<vll>>> matSam;
    ll samples=0;
void genSamples(){
    vector<ll> start,end;
    set<ll>s1,s2;
    while(start.size()<8){
        ll x=rand()%9,y=rand()%9;
        if(s1.find(x)==s1.end() && x!=0){
            start.push_back(x);
        }
        if(s2.find(y)==s2.end() && y!=0){
            end.push_back(y);
        }
        s1.insert(x);
        s2.insert(y);
    }
    while(end.size()<8){
        ll y=rand()%9;
        if(s2.find(y)==s2.end() && y!=0){
            end.push_back(y);
        }
        s2.insert(y);
    }
    if(start==end){
        genSamples();
        return;
    }

    if(solvability(start)%2==0 && vecSam.find({start,end})==vecSam.end()){
        vecSam.insert({start,end});
        vector<vll> initial(3,vll(3)),goal(3,vll(3));
        int k=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                initial[i][j]=start[k];
                goal[i][j]=end[k];
                k++;
            }
        }
        samples++;
        /*for(ll i=0;i<8;i++){
            cout<<start[i]<<" ";
        }
        cout<<endl;
        for(ll i=0;i<8;i++){
            cout<<end[i]<<" ";
        }
        cout<<endl;
        cout<<solvability(start)<<" "<<solvability(end)<<endl<<endl;*/

    }
    if(samples==10){
        return;
    }
    genSamples();
}
struct comp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

Node* newNode(vector<vll> mat, int x, int y, int newX,int newY, int level, Node* parent)
{
    Node* node = new Node;

    // set pointer for path to root
    node->parent = parent;

    // copy data from parent node to current node using assignment
    node->mat = mat;

    vector<vll> prevmat=mat;
    // move tile by 1 position
    swap(node->mat[x][y], node->mat[newX][newY]);

    // set number of misplaced tiles
    node->cost = calculateCost(prevmat, node->mat); // Add the final matrix as a global or pass it here

    // set number of moves so far
    node->level = level;

    // update new blank tile coordinates
    node->x = newX;
    node->y = newY;

    return node;
}


// Function to calculate the number of misplaced tiles
// ie. number of non-blank tiles not in their goal position
int calculateCost(vector<vll> initial, vector<vll> final)
{
    int count = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j] && initial[i][j] != final[i][j])
           count++;
    return count;
}

// Function to check if (x, y) is a valid matrix coordinate
int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// Function to print N x N matrix
int printMatrix(vector<vll> mat)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

// print path from root node to destination node
void printPath(Node* root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->mat);

    printf("\n");
}

int solvability(vector<ll> v){
   /* vector<ll> v(8);
    for(ll i=0;i<8;i++){
        cin>>v[i];
        //7 2 4 5 6 8 3 1 ->even
        //2 7 4 5 6 8 3 1 -> odd
    }*/
    multiset<ll> set1;
    set1.insert(v[0]);
    ll inversions = 0; // Initialize result
    multiset<ll>::iterator itset1; // Iterator for the set
    for (ll i=1; i<8; i++)
    {
        set1.insert(v[i]);
        // Set the iterator to first greater element than arr[i]
        //upper bound takes logn time
        itset1 = set1.upper_bound(v[i]);
        // Get distance of first greater element from end and this distance is count of greater  elements on left side of arr[i]
        inversions += distance(itset1, set1.end());
    }
    //because every step will change the parity twice so the number of inversions should be even for us to reach the final goal state
    //cout<<inversions<<endl;
    return inversions;
}

vector<ll> coordinates(vector<vector<ll>> initial){
    int x=-1,y=-1;
    int row=initial.size();
    int col=initial[0].size();
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(initial[i][j]==0){
                return {i,j};
            }
        }
    }
}


void bfs(vector<vll>initial ,int x, int y,vector<vll>goal){
    // Create a priority queue to store live nodes of
    // search tree;
    priority_queue<Node*,vector<Node*>, comp> pq;

    // create a root node and calculate its cost
    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, goal);

    // bottom, left, top, right
    //movements to move to the next block
    int row[] = { 1, 0, -1, 0 };
    int col[] = { 0, -1, 0, 1 };

    // Add root to list of live nodes;
    pq.push(root);

    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node* sol = pq.top();

        // The found node is deleted from the list of
        // live nodes
        pq.pop();

        // if min is an answer node
        if (sol->cost == 0)
        {
            // print the path from root to destination;
            printPath(sol);
            return;
        }

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++){
            if (isSafe(sol->x + row[i], sol->y + col[i])){
                // create a child node and calculate
                // its cost
                Node* child = newNode(sol->mat, sol->x,sol->y, sol->x + row[i],sol->y + col[i],sol->level + 1, sol);
                child->cost = calculateCost(child->mat, goal);

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
}
};

int main() {
    puzzle_8 instance;
    instance.genSamples();
    cout<<"Enter 1 to run variant 1.\n ";
    int x;
    cin>>x;
    switch(x){

    case 1:
        for(auto it=instance.matSam.begin();it!=instance.matSam.end();it++){
        int x=(instance.coordinates(it->first))[0];
        int y=(instance.coordinates(it->first))[1];
        //int a=(instance.coordinates(it->second))[0];
        //int b=(instance.coordinates(it->second))[1];
        instance.bfs(it->first,x,y,it->second);
        //instance.printPath(instance.newNode(it->first, x, y, x, y, 0, NULL));

        }




    }
}
