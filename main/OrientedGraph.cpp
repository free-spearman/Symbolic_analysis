#include "OrientedGraph.hpp"
#define COMPSIZE 1
//поиск компоненты  
void OrientedGraph::findComponent(size_t vertex) {
    static size_t time = 0;
    disc[vertex] = lowLink[vertex] = ++time;
    stk.push(vertex);
    stkItem[vertex] = true;
    //using graph_t = map<size_t, set<size_t> >;
    // проверка на наличие хоть каких-то смежных вершин
    for ( size_t neighbor :adjList[vertex]){
        if(disc[neighbor] == NOVISITED){
            findComponent(neighbor);
            lowLink[vertex] = min(lowLink[vertex], lowLink[neighbor]);
        }
        else if (stkItem[neighbor])
            lowLink[vertex] = min(lowLink[vertex], disc[neighbor]);
    }
        
    size_t poppedItem = 0;
    if (lowLink[vertex] == disc[vertex]) {
        //выводим компоненту
        vector<size_t> component;
        while (stk.top() != vertex) {
            //csc_t csc;
            // using csc_t = vector<vector<size_t>>;
            poppedItem = stk.top();
            component.push_back(poppedItem);
            //cout << poppedItem << " ";
            stkItem[poppedItem] = false;
            stk.pop();
        }
        poppedItem = stk.top();
        component.push_back(poppedItem);
        //cout << poppedItem << endl;
        csc.push_back(component);
        stkItem[poppedItem] = false;
        stk.pop();
    }
};

csc_t OrientedGraph::findStrongConComponents() {
    for (size_t vertex = 1; vertex < numVertices + 1; vertex++)
        if (disc[vertex] == NOVISITED)
            findComponent(vertex);
    return csc;
};
// нужно доделать потом
void OrientedGraph::printStrongConComponents(){
    cout<<"Количество вершин:"<<numVertices<<endl;
    cout<<"Количество компонент:"<<csc.size()<<endl;

    for (vector<size_t> component: csc){
        cout<<"компонента:";
        cout << *min_element(component.begin(), component.end());
        cout<<" -- Глубина:"<< component.size()<<endl;
        for (size_t v : component)
            cout << v <<" ";
        cout<<endl;
    }
};
size_t OrientedGraph::findMaxRangComponent(){
    if (csc.empty())
        return 0;
    size_t max_rang = 0;
    size_t temp = 0;
    for (vector<size_t> component: csc){
        temp = component.size();
        max_rang = max(max_rang,temp); 
    }
    return max_rang;
} 
csc_t OrientedGraph::filterComponents(){
    //using csc_t = vector<vector<size_t>>;
    if (!filtered_csc.empty())
        filtered_csc.clear();
    //size_t max_size_comp = findMaxRangComponent();
    for (vector<size_t> component: csc){
        if (component.size() > COMPSIZE)
            filtered_csc.push_back(component);         
    }
    return filtered_csc;
};