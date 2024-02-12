#include <bits/stdc++.h>
#define ll long long
#define vll vector<long long>
using namespace std;
#define N 3
#define pb push_back
using namespace std::chrono;

class Node{
    public:
    vector<vll> init;
    ll fn;
    ll gn;
    Node(){
        gn=0;
        fn=0;
    }
};

class puzzle_8{
    public:
    set<pair<vll,vll>> vecSam;
    vector<pair<vector<vll>,vector<vll>>> matSam;
    ll samples;
    mt19937 g; // Mersenne Twister random number generator
    ll nodesTakenOut=0;

    puzzle_8() {
        samples=0;
        random_device rd;
        g.seed(rd()); // Properly seed the random number generator
    }

    //generating random 10 samples of start and end states
    void genSamples() {
        while (samples < 10) {
            vll start={0,1,2,3,4,5,6,7,8};
            vll end={0,1,2,3,4,5,6,7,8};

            vector<vll> initial(3,vll(3)),goal(3,vll(3));


            shuffle(start.begin(), start.end(), g);
            shuffle(end.begin(), end.end(), g);

            if (solvability(start) % 2 == 0 && start != end && vecSam.find({start, end})==vecSam.end()) {
                samples++;

                for (ll i=0;i<9;i++) {
                    //cout << start[i] << " ";
                    ll row=i/3;
                    ll col=i%3;
                    initial[row][col]=start[i];

                }
                //cout << "\nEnd State: ";
                for (ll i=0;i<9;i++) {
                    //cout << end[i] << " ";
                    ll row=i/3;
                    ll col=i%3;
                    goal[row][col]=end[i];
                }

                /*cout << "Sample " << samples << ":" << endl;
                cout << "Start State: ";
                for (ll i=0;i<9;i++) {
                    cout << start[i] << " ";
                    ll row=i/3;
                    ll col=i%3;
                    initial[row][col]=start[i];

                }
                cout << "\nEnd State: ";
                for (ll i=0;i<9;i++) {
                    cout << end[i] << " ";
                    ll row=i/3;
                    ll col=i%3;
                    goal[row][col]=end[i];
                }

                cout << "\nSolvability Start: " << solvability(start);
                cout << " Solvability End: " << solvability(end) << "\n\n";*/
                matSam.pb({initial,goal});
            }
            if(samples==10){
                return;
            }
        }
        return;
    }

    //checking the solvability of the 8-puzzle
    ll solvability(vector<ll> v) {
    ll inversions = 0;
    ll n=v.size();
    for (ll i = 0; i < (n-1); i++) {
        for (ll j = i + 1; j <n; j++) {
            // Count pairs(i, j) such that i appears before j, but i > j.
            if (v[i] > v[j] && v[i] != 0 && v[j] != 0) {
                inversions++;
            }
        }
    }
    return inversions;
    }

    //applying the hamming priority heuristic
    ll hamming(vector<vll>& initial,vector<vll>& goal){
        ll dist=0;
        for(ll i=0;i<3;i++){
            for(ll j=0;j<3;j++){
                if(initial[i][j]!=goal[i][j] && initial[i][j]!=0){
                    dist++;
                }
            }
        }
        return dist;
    }

    pair<ll,ll> getGoalPos(ll tile,vector<vll>& goal){
        for(ll i=0;i<3;i++){
            for(ll j=0;j<3;j++){
                if(goal[i][j]==tile){
                    return {i,j};
                }
            }
        }
        return {};
    }

    //applying the manhattan distance heuristic
    ll manhattan(vector<vll>& initial,vector<vll>& goal){
        ll manDist=0;
        for(ll i=0;i<3;i++){
            for(ll j=0;j<3;j++){
                if(initial[i][j]!=0){
                    auto goal_pos=getGoalPos(initial[i][j],goal);
                    manDist+=abs(goal_pos.first-i)+abs(goal_pos.second-j);
                }
            }
        }
        return manDist;
    }

    pair<ll,ll> goalPos(int x,vector<vll>& goal){
        for(ll i=0;i<3;i++){
            for(ll j=0;j<3;j++){
                if(goal[i][j]==x){
                    return {i,j};
                }
            }
        }
        return {};
    }

    ll linear_conflict(vector<vll>& initial, vector<vll>& goal) {
    ll conflicts = 0;

    // Check conflicts in rows
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (initial[i][j] == 0) continue; // Skip empty tile
            for (int k = j + 1; k < 3; ++k) {
                if (initial[i][k] == 0) continue; // Skip empty tile

                pair<ll,ll> goalPos1 = goalPos(initial[i][j], goal);
                pair<ll,ll> goalPos2 = goalPos(initial[i][k], goal);

                if (goalPos1.first == i && goalPos2.first == i && goalPos1.second > goalPos2.second) {
                    conflicts++;
                }
            }
        }
    }

    // Check conflicts in columns
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 3; ++i) {
            if (initial[i][j] == 0) continue; // Skip empty tile
            for (int k = i + 1; k < 3; ++k) {
                if (initial[k][j] == 0) continue; // Skip empty tile

                pair<ll,ll> goalPos1 = getGoalPos(initial[i][j], goal);
                pair<ll,ll> goalPos2 = getGoalPos(initial[k][j], goal);

                if (goalPos1.second == j && goalPos2.second == j && goalPos1.first > goalPos2.first) {
                    conflicts++;
                }
            }
        }
    }
    return conflicts;
    }

    pair<ll,ll> getCoordinates(vector<vll>& initial){
        for(ll i=0;i<3;i++){
            for(ll j=0;j<3;j++){
                if(initial[i][j]==0){
                    return {i,j};
                }
            }
        }
        return {};
    }

    vector<vector<vll>> get_succcessor(vector<vll>& initial){
        vector<vll> copy=initial;
        pair<ll,ll> coord=getCoordinates(initial);
        vll row={0,1,0,-1};
        vll col={1,0,-1,0};
        vector<vector<vll>> successors;
        for(ll i=0;i<4;i++){
            ll newrow=coord.first+row[i];
            ll newcol=coord.second+col[i];

            if(newrow>=0 && newcol>=0 && newrow<3 && newcol<3 ){
                swap(copy[coord.first][coord.second],copy[newrow][newcol]);
                successors.pb(copy);
                copy=initial;
            }
        }
        return successors;
    }

    struct comp{
    bool operator()(const Node* lhs, const Node* rhs) const{
        if(lhs->fn==rhs->fn) return lhs->gn>lhs->gn;
        return lhs->fn > rhs->fn;
    }
    };

    vector<ll> runHamming(vector<vll>& initial,vector<vll>& goal){
        auto start = high_resolution_clock::now();
        priority_queue<Node*,vector<Node*>,comp> open;
        set<vector<vll>> closed;
        Node* root=new Node();
        root->init=initial;
        root->fn=hamming(initial,goal); //reaching goal state from current initial state, gn=0 here, so fn=hn
        root->gn=0;
        open.push(root);
        map<vector<vll>,vector<vll>> parent;
        ll lenPath=0;

        while(!open.empty()){
            nodesTakenOut++;
            Node* curr=open.top();
            open.pop();
            vector<vector<vll>> neighbour=get_succcessor(curr->init);
            vector<vll> inMat=curr->init;
            closed.insert(inMat);
            if(inMat==goal){
                stack<vector<vector<ll>>> path;
                while(inMat!=initial){
                    path.push(inMat);
                    inMat=parent[inMat];
                }
                path.push(initial);
                lenPath=path.size();
                cout<<"The path to reach the solution is as follows: "<<endl;
                printPath(path);
                auto stop=high_resolution_clock::now();
                auto duration=duration_cast<microseconds>(stop-start);
                return {lenPath,nodesTakenOut,duration.count()};
            }
            for(auto x:neighbour){
                if(closed.find(x)==closed.end()){
                    Node* newnode=new Node();
                    newnode->gn=curr->gn+1;
                    newnode->fn=newnode->gn+hamming(x,goal); //this is the heuristic function f(n)
                    newnode->init=x;
                    open.push(newnode);
                    parent[x]=inMat;
                    closed.insert(x);
                }
            }
        }
        return {};
    }

    vector<ll> runManhattan(vector<vll>& initial,vector<vll>& goal){
        auto start = high_resolution_clock::now();
        priority_queue<Node*,vector<Node*>,comp> open;
        set<vector<vll>> closed;
        Node* root=new Node();
        root->init=initial;
        root->fn=manhattan(initial,goal); //reaching goal state from current initial state, gn=0 here, so fn=hn
        root->gn=0;
        open.push(root);
        map<vector<vll>,vector<vll>> parent;
        ll lenPath=0;
        //parent[initial]={};

        while(!open.empty()){
            nodesTakenOut++;
            Node* curr=open.top();
            open.pop();
            vector<vector<vll>> neighbour=get_succcessor(curr->init);
            vector<vll> inMat=curr->init;
            closed.insert(inMat);
            if(inMat==goal){
                stack<vector<vector<ll>>> path;
                while(inMat!=initial){
                    path.push(inMat);
                    inMat=parent[inMat];
                }
                path.push(initial);
                lenPath=path.size();
                cout<<"The path to reach the solution is as follows: "<<endl;
                printPath(path);
                auto stop=high_resolution_clock::now();
                auto duration=duration_cast<microseconds>(stop-start);
                return {lenPath,nodesTakenOut,duration.count()};
            }
            for(auto x:neighbour){
                if(closed.find(x)==closed.end()){
                    Node* newnode=new Node();
                    newnode->gn=curr->gn+1;
                    newnode->fn=newnode->gn+manhattan(x,goal); //this is the heuristic function f(n)
                    newnode->init=x;
                    open.push(newnode);
                    parent[x]=inMat;
                    closed.insert(x);
                    /*for(int i=0;i<3;i++){
                        for(int j=0;j<3;j++){
                            cout<<x[i][j]<<" ";
                        }
                        cout<<endl;
                    }
                    cout<<endl;*/
                }
            }
        }
        return {};
    }

    vll runLinear(vector<vll>& initial,vector<vll>& goal){
        auto start = high_resolution_clock::now();
        priority_queue<Node*,vector<Node*>,comp> open;
        set<vector<vll>> closed;
        Node* root=new Node();
        root->init=initial;
        root->fn=manhattan(initial,goal)+2*linear_conflict(initial,goal); //reaching goal state from current initial state, gn=0 here, so fn=hn
        root->gn=0;
        open.push(root);
        map<vector<vll>,vector<vll>> parent;
        ll lenPath=0;
        //parent[initial]={};

        while(!open.empty()){
            nodesTakenOut++;
            Node* curr=open.top();
            open.pop();
            vector<vector<vll>> neighbour=get_succcessor(curr->init);
            vector<vll> inMat=curr->init;
            closed.insert(inMat);
            if(inMat==goal){
                stack<vector<vector<ll>>> path;
                while(inMat!=initial){
                    path.push(inMat);
                    inMat=parent[inMat];
                }
                path.push(initial);
                lenPath=path.size();
                cout<<"The path to reach the solution is as follows: "<<endl;
                printPath(path);
                auto stop=high_resolution_clock::now();
                auto duration=duration_cast<microseconds>(stop-start);
                return {lenPath,nodesTakenOut,duration.count()};
            }
            for(auto x:neighbour){
                if(closed.find(x)==closed.end()){
                    Node* newnode=new Node();
                    newnode->gn=curr->gn+1;
                    newnode->fn=newnode->gn+manhattan(x,goal)+2*linear_conflict(x,goal); //this is the heuristic function f(n)
                    newnode->init=x;
                    open.push(newnode);
                    parent[x]=inMat;
                    closed.insert(x);
                    /*for(int i=0;i<3;i++){
                        for(int j=0;j<3;j++){
                            cout<<x[i][j]<<" ";
                        }
                        cout<<endl;
                    }
                    cout<<endl;*/
                }
            }
        }
        return {};

    }

    void printPath(stack<vector<vll>>& path){
        int steps=1;
        while(!path.empty()){
                cout<<"Step "<<steps<<":"<<endl<<endl;
            vector<vll> inn=path.top();
            for(ll i=0;i<3;i++){
                for(ll j=0;j<3;j++){
                    cout<<inn[i][j];
                }
                cout<<endl;
            }
            cout<<endl;
            path.pop();
            steps++;
        }
        return;
    }

};

int main(){
    puzzle_8 instance;
    instance.genSamples();
    cout<<"Enter 1 for Hamming Priority.\nEnter 2 for Manhattan Distance.\nEnter 3 for Manhattan with Linear Conflict.\n";
    int c;
    cin>>c;
    switch(c){
    case 1:
        cout<<"Enter 1 for computer generated input.\nEnter 2 for custom input.\n";
        int y;
        cin>>y;
        if(y==1){
        for(ll i=0;i<1;i++){
            //giving the start and the end state as parameters to the function
            vector<ll> hamm=instance.runHamming(instance.matSam[i].first,instance.matSam[i].second);
            cout<<"The number of nodes removed from the frontier are: "<<hamm[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<hamm[0]<<endl;
            cout<<"The time taken by hamming distance heuristic is: "<<hamm[2]<<" microseconds."<<endl;
        }
        }
        else{
            vector<vll> input(3,vll(3));
            cout<<"Enter a valid input matrix: "<<endl;
            //7 2 4 5 0 6 8 3 1
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    cin>>input[i][j];
                }
            }
            vector<vll> goal(3,vll(3));
            goal={{0,1,2},{3,4,5},{6,7,8}};
            vector<ll> hamm=instance.runHamming(input,goal);
            cout<<"The number of nodes removed from the frontier are: "<<hamm[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<hamm[0]<<endl;
            cout<<"The time taken by hamming distance heuristic is: "<<hamm[2]<<" microseconds."<<endl;

        }
        break;
    case 2:
        cout<<"Enter 1 for computer generated input.\nEnter 2 for custom input.\n";
        int z;
        cin>>z;
        if(z==1){
        for(ll i=0;i<1;i++){
            vector<ll> mann=instance.runManhattan(instance.matSam[i].first,instance.matSam[i].second);
            cout<<"The number of nodes removed from the frontier are: "<<mann[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<mann[0]<<endl;
            cout<<"The time taken by manhattan distance heuristic is: "<<mann[2]<<" microseconds."<<endl;
        }
        }
        else{
            vector<vll> input(3,vll(3));
            cout<<"Enter a valid input matrix: "<<endl;
            //7 2 4 5 0 6 8 3 1
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    cin>>input[i][j];
                }
            }
            vector<vll> goal(3,vll(3));
            goal={{0,1,2},{3,4,5},{6,7,8}};
            vector<ll> mann=instance.runManhattan(input,goal);
            cout<<"The number of nodes removed from the frontier are: "<<mann[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<mann[0]<<endl;
            cout<<"The time taken by manhattan distance heuristic is: "<<mann[2]<<" microseconds."<<endl;
        }
        break;

     case 3:
        cout<<"Enter 1 for computer generated input.\nEnter 2 for custom input.\n";
        int k;
        cin>>k;
        if(k==1){
        for(ll i=0;i<1;i++){
            //giving the start and the end state as parameters to the function
            vector<ll> linn=instance.runLinear(instance.matSam[i].first,instance.matSam[i].second);
            cout<<"The number of nodes removed from the frontier are: "<<linn[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<linn[0]<<endl;
            cout<<"The time taken by linear conflict heuristic is: "<<linn[2]<<" microseconds."<<endl;
        }
        }
        else{
            vector<vll> input(3,vll(3));
            cout<<"Enter a valid input matrix: "<<endl;
            //7 2 4 5 0 6 8 3 1
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    cin>>input[i][j];
                }
            }
            vector<vll> goal(3,vll(3));
            goal={{0,1,2},{3,4,5},{6,7,8}};
            vector<ll> linn=instance.runLinear(input,goal);
            cout<<"The number of nodes removed from the frontier are: "<<linn[1]<<endl;
            cout<<"The number of steps to reach the solution are: "<<linn[0]<<endl;
            cout<<"The time taken by linear conflict heuristic is: "<<linn[2]<<" microseconds."<<endl;

        }
        break;
    }

}
