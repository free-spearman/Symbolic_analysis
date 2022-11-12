#include "OrientedGraph.cpp"
#include "SymbolicImage.hpp"
#define NODE 8
using namespace std;
int graph[NODE][NODE] = {
   {0, 0, 1, 0, 0, 0, 0, 0},//1
   {1, 0, 0, 1, 0, 0, 0, 0},//2
   {0, 1, 0, 1, 0, 0, 0, 0},//3
   {0, 0, 0, 0, 1, 0, 0, 0},//4
   {0, 0, 0, 0, 0, 1, 0, 0},//5
   {0, 0, 0, 0, 1, 0, 1, 0},//6
   {0, 0, 0, 0, 0, 0, 0, 1},//7
   {1, 0, 0, 0, 1, 0, 0, 0}//8
};
/*

void findComponent(){
   static int time = 0;
   cout<<"findComponent u = "<<u<<" time: "<<time<<endl;
   disc[u] = low[u] = ++time;    //inilially discovery time and low value is 1
   stk.push(u);
   stkItem[u] = true;    //flag as u in the stack
   for(int v = 0; v<NODE; v++) {
      if(graph[u][v]) {
         if(disc[v] == -1) {   //when v is not visited
            findComponent(v, disc, low, stk, stkItem);
            low[u] = min(low[u], low[v]);
         } else if(stkItem[v])    //when v is in the stack, update low for u
            low[u] = min(low[u], disc[v]);
      }
   }
   int poppedItem = 0;
   cout <<"poppedItem:" << time << endl;
   cout<<"low:"<<low[u]<<" disc:" << disc[u] << endl;
   if(low[u] == disc[u]) {
      //cout << "while(stk.top() != u)"<<endl;
      cout << "Выводим компоненту" << endl;
      while(stk.top() != u) {
         poppedItem = stk.top();
         cout << poppedItem + 1 << " ";
         stkItem[poppedItem] = false;    //mark as item is popped
         stk.pop();
      }
      poppedItem = stk.top();
      cout << poppedItem + 1 <<endl;
      stkItem[poppedItem] = false;
      stk.pop();
   }
}
void strongConComponent() {
   int disc[NODE], low[NODE];
   bool stkItem[NODE];
   stack<int> stk;
   for(int i = 0; i<NODE; i++) {    //initialize all elements
      disc[i] = low[i] = -1;
      stkItem[i] = false;
   }
   for(int i = 0; i<NODE; i++)    //initialize all elements
      if(disc[i] == -1)
         findComponent(i, disc, low, stk, stkItem);
   for (int i :disc)
      cout<<i<<"--";
   cout<<endl;
   for (int i :low)
      cout<<i<<"--";
   cout<<endl;
}
*/
int main() {
   graph_t test_graph;
   for( int i = 0; i < NODE; i++){
      set<size_t> row;
      for (int j = 0; j < NODE; j++){
         if (graph[i][j] == 1)
            row.insert(j+1);
      }
      test_graph.insert({i+1, row} );
   }
   for (pair<size_t, set<size_t> > v: test_graph){
      cout<< v.first << ":"; 
      for ( size_t i : v.second)
         cout<<i<<" ";
      cout<<endl;
   }
   OrientedGraph g =  OrientedGraph (test_graph);
   cout<<"\n\n\n";
   g.findStrongConComponents();
   g.printStrongConComponents();
   //strongConComponent();
   return 0; 
}