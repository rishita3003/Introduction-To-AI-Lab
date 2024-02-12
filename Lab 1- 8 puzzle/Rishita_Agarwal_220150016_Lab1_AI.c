#include <bits/stdc++.h>
#define ll long long
#define vll vector<long long>
using namespace std;

class puzzle_8{
    public:
    set<pair<vll,vll>> vecSam;
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
    
    if(((solvability(start)%2==0 && solvability(end)%2==0)|| solvability(start)%2==1 && solvability(end)%2==1) && vecSam.find({start,end})==vecSam.end()){
        vecSam.insert({start,end});
        samples++;
        for(ll i=0;i<8;i++){
            cout<<start[i]<<" ";
        }
        cout<<endl;
        for(ll i=0;i<8;i++){
            cout<<end[i]<<" ";
        }
        cout<<endl;
        cout<<solvability(start)<<" "<<solvability(end)<<endl<<endl;
    }
    if(samples==10){
        return;
    }
    genSamples();
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

void bfs(){
    
    
}

};

int main() {
    puzzle_8 instance;
    instance.genSamples();
    cout<<"Enter 1 to check solvability of the puzzle.\nEnter 2 to run variant 1.\n ";
}