// Perceptron.h
// Matt Grant(teamuba@gmail.com)

#include<cstdlib>
#include<algorithm>
using std::find;
#include<list>
using std::list;
#include<utility>
using std::pair;
#include<map>
using std::map;
#include<functional>
using std::function;
#include<vector>
using std::vector;
#include<cmath>
using std::abs;
#include<iostream>
using std::cout;
using std::endl;
// TODO
// Literally a fucking perceptron in perceptrons
// Rules about linking nodes?



// Implements a sigmoid neuron by default
class Node{
    public:
        // can specify activation function or NULL for fast sigmoid approx
        Node(function<float( vector<float> )> activation);
        auto activate(vector<float> weightedInput);
        
    private:
        // Activation function
        function<float( vector<float> )> _activation;
};

// Node functions
Node::Node(function<float( vector<float> )> activation){
    if(activation){
        _activation = activation;
    }
    else{
            _activation = [](vector<float> x ) -> float{
                // z = sum of weighted inputs
                float z = 0;
                for(auto i : x){ z += i; }
                return (z / (1 + abs(z)));
            };

    }
}

auto Node::activate(vector<float> weightedInput){
    return(_activation(weightedInput));
}

class BinNode : public Node{
    public:
        BinNode(int bias);
    private:
        const int _bias = 0;
};

//BinNode functions
BinNode::BinNode(int bias) : Node(
        [bias](vector<float> x) -> float{
        // z = sum of weighted inputs
        float z = 0;
        for(auto i : x){ z += i; }
        if(z > bias){ return 1; }
        return 0;
        }),
    _bias(bias){

}

class Perceptron{
    public:

        // returns an integer id to reference the node
        int createBNode(int bias);
        int createSNode();
        int createNode(function<float (vector<float>)> activation);
        void linkNode(int from, int to, float weight);
        void linkInput(int to);

        map<int, float> run(vector<float> input);
        void printLinks();
    private:
        map<int, float> run();
        void sendInput(vector<float> input);
        Node* get(int id);
        int getid(Node* n);

        int _id = 0;
        vector< pair<int, Node> > _nodes;
        // Maps node* to all forward edges and weights
        map< Node*, vector< pair<Node*, int> >> _links;
        vector<Node*> _inputLinks;
        vector<float> _input;


};

// Perceptron functions
int Perceptron::createBNode(int bias){
    Node b = BinNode(bias);
    _nodes.push_back(pair<int, Node>(_id, b));
    return _id++;
}

int Perceptron::createSNode(){
    Node n = Node(NULL);
    _nodes.push_back( pair<int, Node>(_id, n) );
    return _id++;
}

int Perceptron::createNode(function<float ( vector<float> )> activation){
    Node n = Node(activation);
    _nodes.push_back( pair<int, Node>(_id,n) );
    return _id++;

}

void Perceptron::linkNode(int to, int from, float weight){
    pair<Node*, float> p;
    p.first = get(from);
    p.second = weight;
    _links[get(to)].push_back(p);
}

void Perceptron::linkInput(int to){
    _inputLinks.push_back(get(to));
}

void Perceptron::sendInput(vector<float> input){
    _input = input;
}

void Perceptron::printLinks(){
    int idx = 0;
    int idy = 0;
    float weight = 0;
    cout << "---- Links ----" << endl;
    for(auto const& x : _links){
        idx = getid(x.first);
        cout << "Node " << idx << endl;
        for(auto const& y : x.second){
            idy = getid(y.first);
            weight = y.second;
            cout << idx << " -" << weight <<"-> " << idy << endl;
        }

    }
    cout << "---- Links----" << endl << endl;

}

map<int, float> Perceptron::run(vector<float> input){
    sendInput(input);
    return run();
}

map<int, float> Perceptron::run(){
    // Queue of nodes to be evaluated. Enforces layer by layer evaluation
    list<Node*> q;
    // Keeps track of total weighted input to be sent to a node
    map< Node*, vector<float> > inputs;
    map<int, float> outputs;
    // Initial setup
    for(auto i : _inputLinks){
        q.push_back(i);
        inputs[i].insert(inputs[i].end(), _input.begin(), _input.end());
    }
    Node* curr = NULL;
    float val = 0;
    
    while(!q.empty()){
        curr = q.front();
        q.pop_front();
        // Activate node and capture value
        val = curr->activate(inputs[curr]);
        // If node is not an output node, register its output for connected nodes
        if(!_links[curr].empty()){
            for(auto i : _links[curr]){
                Node* n = i.first;
                float weight = i.second;
                inputs[n].push_back(val * weight);
                if(find(q.begin(), q.end(), n) == q.end()){ 
                    q.push_back(n);
                }
            }
        }
        // output node, capture value
        else{
            outputs[getid(curr)] = val;
        }

    }
    return outputs;
}

// As long as id is valid than this shouldn't ever fail
Node* Perceptron::get(int id){
    if(id > _id){ return NULL; }
    Node* ret = NULL;
    bool found = false;
    int i = 0;
    while(found == false && i < _id){
        if(_nodes[i].first == id){
            found = true;
            ret = &(_nodes[i].second);
        }
        i++;
    }
    return ret;

}

int Perceptron::getid(Node* n){
    int ret = -1;
    bool found = false;
    size_t i = 0;
    while(found == false && i < _nodes.size()){
        if(&(_nodes[i].second) == n){
            found = true;
            ret = (_nodes[i].first);
        }
        i++;
    }
    return ret;
}