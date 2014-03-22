#include <vector>
#include <stack>
#include <stdio.h>

using namespace std; 

struct Node {
	bool onStack;
	int value;
	int d;
	int low;
	Node* rep;
	Node() : onStack(false)
	{}
};

vector<Node*> nodes;
vector< vector<Node*> > edges;
vector< vector<Node*> > sccs;

void makeSet(Node* node){
	node->rep = node;

}

void unionSet(Node* node1, Node* node2){
	node2->rep = node1->rep;
}

Node* findSet(Node* node){
	if(node->rep == node){
		return node;
	}
	node->rep = findSet(node->rep);
	return NULL;
}

void tarjanVisit(Node* nodeU, int* pvisited, stack<Node*> *pL){
	nodeU->d = *pvisited;
	nodeU->low = *pvisited;
	*pvisited = (*pvisited) + 1;
	pL->push(nodeU);
	nodeU->onStack = true;
	for (unsigned int i=0; i<edges[nodeU->value-1].size(); i++){
		Node* nodeV = edges[nodeU->value-1][i];
		if(nodeV->d == -1){
			tarjanVisit(nodeV, pvisited, pL);
			nodeU->low = min(nodeU->low, nodeV->low);
		}else{
			if(nodeV->onStack){
				nodeU->low = min(nodeU->low, nodeV->d);
			}
		}
	}
	/* SCC was found */
	if(nodeU->d == nodeU->low){
		vector<Node*> vecNodes;
		makeSet(nodeU);
		vecNodes.push_back(nodeU);
		Node* nodeV = pL->top();
		pL->pop();
		nodeV->onStack = false;
		while(nodeV != nodeU){
			vecNodes.push_back(nodeV);
			unionSet(nodeU, nodeV);
			nodeV = pL->top();
			pL->pop();
			nodeV->onStack = false;
		}
		sccs.push_back(vecNodes);
	}
}

void scc_Tarjan(vector<Node*> nodes, vector< vector<Node*> > edges){
	int visitedVal = 0;
	int* pvisited = &visitedVal;
	stack<Node*> L;
	stack<Node*> *pL = &L;
	for(unsigned int i=0; i<nodes.size(); i++){
		nodes[i]->d = -1;
	}
	for(unsigned int i=0; i<nodes.size(); i++){
		if(nodes[i]->d == -1){
			tarjanVisit(nodes[i], pvisited, pL);
		}
	}
}


int main(){

	int nrPeople;
	int nrShares;
	
  if( scanf ("%d %d", &nrPeople, &nrShares) != 2){
  	printf("Expected two numbers as input\n");
  	return 1;
  }
   
  for(int i=1; i<nrPeople+1; i++){
  	Node* pnode = new Node();
  	pnode->value = i;
  	nodes.push_back(pnode);  
  }
  
  	edges.resize(nrPeople);
	
	for(int i=0; i<nrShares; i++){
		int valNode1, valNode2;
		if( scanf("%d %d", &valNode1, &valNode2) != 2){
			printf("Expected two numbers as input\n");
  		return 1;
		}
		Node* pnode2 = nodes[valNode2-1];
		edges[valNode1-1].push_back(pnode2);
	}
	
	scc_Tarjan(nodes, edges);
	printf("%zu\n", sccs.size());

	int big = 0;
	for(unsigned int i=0; i< sccs.size(); i++){
		if(sccs[i].size() > (unsigned) big){
			big = sccs[i].size();
		}
	}
	printf("%d\n" ,big);

	vector<bool> toErase;
	toErase.resize(sccs.size(), false);

	for(unsigned int i=0; i<sccs.size(); i++){
		for(unsigned int j=0; j<sccs[i].size(); j++){
			if(!(toErase[i])){
				int valSccs = (sccs[i][j]->value)-1;
				for(unsigned int h=0; h<edges[valSccs].size(); h++){
					if(nodes[valSccs]->rep != edges[valSccs][h]->rep){
						toErase[i] = true;
					}
				}
			}
		}
	}
	for(unsigned int i=0; i<toErase.size(); i++){
		if(toErase[i]){
			sccs.erase(sccs.begin()+i);
		}
	}
	
	printf("%zu\n", sccs.size());

return 0;
}
