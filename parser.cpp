#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef struct map{
  string key;
  string value;  
}Map;

class Tag{
    string name;
    vector<Map> values;
    vector<Tag*> childs;
  public:
    Tag(string name) {this->name = name;}
    string get_name() {return this->name;}
    vector<Tag*>& get_childs(){return this->childs;}
    vector<Map> get_values(){return this->values;}
    void set_values(vector<Map> values) {this->values = values;}
    void print_value(string s);
    ~Tag();
};

void take_NQ(int &N, int &Q) { cin >> N >> Q; }
string take_Lines(int N);
vector<string> take_Queries(int Q);
void Build(string lines, int N);
void Run_Queries(vector<string> queries, int Q);
void Run_Query(string query);
bool contains(string s, char c);
string take_node(string s,int& iter);
string get_tagName(string s);
Tag* get_address(vector<Tag*> &tags,string s);
Tag* find_tag(vector<Tag*> &tags, string s);
vector<Map> get_attributes(string s);

Tag rootTag = Tag("root");

int main() {
    int N = 0,Q = 0;
    take_NQ(N, Q);
    string lines = take_Lines(N);
    vector<string> queries = take_Queries(Q);
    Build(lines,N);
    Run_Queries(queries,Q);
    return 0;
}

void Build(string lines, int N)
{
    int glob_iter = 0;
    vector<string> stack;
    for(int i = 0; i < N; i++)
    {
        string buffer = take_node(lines,glob_iter);
        if(!contains(buffer,'/'))
        {
            string tag_name = get_tagName(buffer);
            vector<Map> attributes = get_attributes(buffer);
            Tag *new_tag = new Tag(tag_name);
            new_tag->set_values(attributes);
            if(stack.size() == 0)
            {
                rootTag.get_childs().push_back(new_tag);
            }
            else
            {
                Tag* t = get_address(rootTag.get_childs(),stack.back());
                t->get_childs().push_back(new_tag);
            }
            stack.push_back(tag_name);
        }
        else
        {
            stack.pop_back();
        }
    }
}

void Run_Query(string query)
{
    Tag *T = &rootTag;
    unsigned long int i = 0;
    string word = "";
    char delimeter = '.';
    while(true)
    {
        while(query[i] != '.' && query[i] != '~' &&  i < query.size())
        {
            word += query[i];
            i++;
        }
        if (delimeter == '.')
        {
            T = find_tag(T->get_childs(),word);
            if(!T){
                cout << "Not Found!"<< endl;
                break;
            }
        }else{
            T->print_value(word);
            break;
        }
        delimeter = query[i];
        i++;
        word = "";
    }
}

void Run_Queries(vector<string> queries, int Q)
{
    for(int i = 0; i < Q; i++)
    {
        Run_Query(queries[i]);
    }
}

bool contains(string s, char c)
{
    for(unsigned int i = 0; i < s.size(); i++)
    {
        if (s[i] == c) return true;
    }
    return false;
}

string take_node(string s,int& iter){
    string result = "";
    while(s[iter] != '>'){
        result += s[iter];
        iter++;
    }
    result += s[iter];
    iter++;
    return result;
}

string get_tagName(string s){
    int i = 1;
    string name = "";
    while(s[i] != ' ' && s[i] != '>')
    {
        name += s[i];
        i++;
    }
    return name;
}

Tag* get_address(vector<Tag*> &tags, string s)
{
    Tag* result = NULL;
    for(unsigned int i = 0; i < tags.size(); i++)
    {
        if(tags[i]->get_name() == s)
        {
            return tags[i];
        }
    }
    for(unsigned int i = 0; i < tags.size(); i++)
    {
        result = get_address(tags[i]->get_childs(),s);
    }
    return result;
}

Tag* find_tag(vector<Tag*> &tags, string s)
{
    Tag* result = NULL;
    for(unsigned int i = 0; i < tags.size(); i++)
    {
        if(tags[i]->get_name() == s)
        {
            result = tags[i];
        }
    }
    return result;
}

vector<Map> get_attributes(string s)
{
    int i = 0;
    vector<Map> m;
    while(s[i] != ' ' && s[i] != '>') i++;
    while(s[i] != '>')
    {   
        if(s[i] == ' ') i++;
        string att_name = "", att_value = "";
        while(s[i] != ' '){
            att_name += s[i];
            i++;
        }
        i+=4;
        while(s[i] != '"'){
            att_value += s[i];
            i++;
        }
        i++;
        Map new_att;
        new_att.key = att_name;
        new_att.value = att_value;
        m.push_back(new_att);
    }
    return m;
}
string take_Lines(int N)
{
    string s, result = "";
    int counter = 0;
    getline(cin,s); //take first \n
    while(counter < N)
    {
        getline(cin,s);
        result += s;
        counter ++;
    }
    return result;
}

vector<string> take_Queries(int Q)
{
    string s;
    vector<string> result;
    int counter = 0;
    while(counter < Q)
    {
        getline(cin,s);
        result.push_back(s);
        counter ++;
    }
    return result;
}

void Tag::print_value(string s)
{
    bool contains = false;
    for(unsigned int i = 0; i < values.size() ;i++)
    {
        if(values[i].key == s){
            cout << values[i].value << endl;
            contains = true;
            break;
        }
    }
    if(!contains){
        cout << "Not Found!" << endl;
    }
}

Tag::~Tag()
{
    for(unsigned int i = 0; i < this->childs.size();i++)
    {
        delete childs[i];
    }
}