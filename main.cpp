#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <string.h>
#include <iomanip>
using namespace std;

vector<string>tokens;
vector<string>types;
vector<vector<string> > commands;
vector<vector<string> > ctypes;

map<string, map <string,vector<string> > >table1;

void build_parser_table(){
    vector <string> v;
    //S rule
    v.push_back("int");
    v.push_back("Z");
    table1["S"]["int"]=v;
    v.clear();
    v.push_back("float");
    v.push_back("Z");
    table1["S"]["float"]=v;
    v.clear();
    v.push_back("if");
    v.push_back("J");
    table1["S"]["if"]=v;
    v.clear();
     v.clear();
    v.push_back("loop");
    v.push_back("E");
    table1["S"]["loop"]=v;
    v.clear();
     v.clear();
    v.push_back("out");
    v.push_back("E");
    table1["S"]["out"]=v;
    v.clear();
     v.clear();
    v.push_back("in");
    v.push_back("identifier");
    table1["S"]["in"]=v;
    v.clear();
    //J rule
    v.push_back("(");
    v.push_back("E");
    v.push_back(")");
    table1["J"]["("]=v;
    v.clear();
    //Z rule
    v.push_back("identifier");
    v.push_back("H");
    table1["Z"]["identifier"]=v;
    v.clear();
    //H rule
    v.push_back("=");
    v.push_back("E");
    table1["H"]["="]=v;
    v.clear();
    //E rule
    v.push_back("T");
    v.push_back("X");
    table1["E"]["("]=v;
    table1["E"]["identifier"]=v;
    table1["E"]["num"]=v;
    v.clear();
    //T rule
    v.push_back("M");
    v.push_back("Y");
    table1["T"]["identifier"]=v;
    table1["T"]["num"]=v;
    v.clear();
    v.push_back("(");
    v.push_back("E");
    v.push_back(")");
    table1["T"]["("]=v;
    v.clear();
    //M rule
    v.push_back("identifier");
    v.push_back("K");
    table1["M"]["identifier"]=v;
    v.clear();
    v.push_back("num");
    v.push_back("K");
    table1["M"]["num"]=v;
    v.clear();
    //K rule
    v.push_back("eps");
    table1["K"][")"]=v;
    table1["K"]["+"]=v;
    table1["K"]["-"]=v;
    table1["K"]["|"]=v;
    table1["K"]["*"]=v;
    table1["K"]["/"]=v;
    table1["K"]["&"]=v;
    table1["K"][";"]=v;
    v.clear();
    v.push_back("==");
    v.push_back("F");
    table1["K"]["=="]=v;
    v.clear();
    v.push_back("!=");
    v.push_back("F");
    table1["K"]["!="]=v;
    v.clear();
    v.push_back("<=");
    v.push_back("F");
    table1["K"]["<="]=v;
    v.clear();
    v.push_back(">=");
    v.push_back("F");
    table1["K"][">="]=v;
    v.clear();
    v.push_back("!=");
    v.push_back("F");
    table1["K"]["!="]=v;
    v.clear();
    //F rule
    v.push_back("identifier");
    table1["F"]["identifier"]=v;
    v.clear();
    v.push_back("num");
    table1["F"]["num"]=v;
    v.clear();
    //X rule
    v.push_back("eps");
    table1["X"][")"]=v;
    table1["X"][";"]=v;
    v.clear();
    v.push_back("+");
    v.push_back("E");
    table1["X"]["+"]=v;
    v.clear();
    v.push_back("-");
    v.push_back("E");
    table1["X"]["-"]=v;
    v.clear();
    v.push_back("|");
    v.push_back("E");
    table1["X"]["|"]=v;
    v.clear();
    //Y rule
    v.push_back("eps");
    table1["Y"][")"]=v;
    table1["Y"][";"]=v;
    table1["Y"]["|"]=v;
    table1["Y"]["+"]=v;
    table1["Y"]["-"]=v;
    v.clear();
    v.push_back("*");
    v.push_back("T");
    table1["Y"]["*"]=v;
    v.clear();
    v.push_back("/");
    v.push_back("T");
    table1["Y"]["/"]=v;
    v.clear();
    v.push_back("&");
    v.push_back("T");
    table1["Y"]["&"]=v;
}

void comman_grouping (){
    int j=0;
    while(true){
        vector<string> command;
        vector<string> type;
        for(;j<tokens.size();j++){
            command.push_back(tokens[j]);
            type.push_back(types[j]);
            if(strcmp(tokens[j].c_str(),";")==0){
                    j++;
                    break;

            }else if(strcmp(tokens[j].c_str(),"}")==0 || strcmp(tokens[j].c_str(),"{")==0){
                j++;
                command.pop_back();
                type.pop_back();
                command.push_back(";");
                type.push_back(";");
                break;
            }
        }
        commands.push_back(command);
        ctypes.push_back(type);
        if (j== tokens.size()) break;
    }
}
//DFA
int numCheck(int state,char s){
    int DFA[4][2]={
                {1,2},
                {1,2},
                {3,-1},
                {3,-1}
                };

    if(s-'0'>=0 && s-'0'<=9){
        return DFA[state][0];
    }else if (s == '.'){
        return DFA[state][1];
    }
    return -1;
}

int idCheck(int state,char s){
        int DFA[2][2]={
                        {1,-1},
                        {1,1}
                    };
        if(s-'a'>=0 && s-'z'<=25 || s-'A'>=0 && s-'Z'<=25 || s=='_'){
            return DFA[state][0];
        }
        else if(s-'a'>=0 && s-'z'<=25 || s-'A'>=0 && s-'Z'<=25 || s=='_'|| s-'0'>=0 && s-'0'<=9){
            return DFA[state][1];
        }
        return -1;
}

void separation(string line){
    string currentWord="";
    char separators[16]={'+','-','/','*','=',' ','>','<','\t','(',')','{','}',';','|','&'};
    for(int i=0;i<line.size();i++){

        bool sep=false;
        for(int j=0; j<16;j++){
            //Character not in separators.
            if(line[i]==separators[j]) {
                    sep=true;
                    break;
            }
        }
        if(!sep) currentWord+=line[i];
        else{
            if (currentWord!="") {
                tokens.push_back(currentWord);
                currentWord="";
            }
            switch (line[i]){
                case '=':
                    if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("==");
                                    i++;
                    }
                    else tokens.push_back("=");
                    break;

                case '+':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("+=");
                                    i++;
                    }
                    else tokens.push_back("+");
                    break;
                case '-':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("-=");
                                    i++;
                    }
                    else tokens.push_back("-");
                    break;
                case '*':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("*=");
                                    i++;
                    }
                    else tokens.push_back("*");
                    break;
                case '/':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("/=");
                                    i++;
                    }
                    else tokens.push_back("/");
                    break;
                case '<':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back("<=");
                                    i++;
                    }
                    else tokens.push_back("<");
                    break;
                case '>':
                     if(i+1<line.size() && line[i+1]=='=') {
                                    tokens.push_back(">=");
                                    i++;
                    }
                    else tokens.push_back(">");
                    break;
                case ' ': break;
                case ';': tokens.push_back(";"); break;
                case '(': tokens.push_back("("); break;
                case ')': tokens.push_back(")"); break;
                case '{': tokens.push_back("{"); break;
                case '}': tokens.push_back("}"); break;
                default: break;
            }
        }
    }
}

void DFA(){
    string keywords[]={"int","if","float","else","loop","out","in"};
    string operators[]={"+","-","/","*","=",">","<","+=","-=","*=","/=","==","<=",">=","!="};
    string separators[]={";"};
    string paran[]={"(",")","{","}"};

    for(int i=0;i<tokens.size();i++){
        string currentToken=tokens[i];
        bool f=false;
        for(int j=0;j<7;j++){
            if(currentToken==keywords[j]){
                f=true;
                break;
            }
        }
        if(f){
            types.push_back("keyword");
            continue;
        }
        for(int j=0;j<15;j++){
            if(currentToken==operators[j]){
                f=true;
                break;
            }
        }
        if(f){
            types.push_back("operator");
            continue;
        }
        for(int j=0;j<4;j++){
            if(currentToken==paran[j]){
                f=true;
                break;
            }
        }
        if(f){
            types.push_back("paran");
            continue;
        }
        if(currentToken==";"){
            types.push_back(";");
            continue;
        }
        //DFA number
        int state=0;
        for(int j=0;j<currentToken.size();j++){
            state=numCheck(state,currentToken[j]);
            if(state==-1) break;
        }
        if(state==1 || state == 3){
            types.push_back("num");
            continue;
        }
        //DFA identifier
        state=0;
        for(int j=0;j<currentToken.size();j++){
            state=idCheck(state,currentToken[j]);
            if(state==-1) break;
        }
        if(state==1){
            types.push_back("identifier");
            continue;
        }
        types.push_back("syntax error");
    }
}

void stack_print(stack<string> s){
    string ss=s.top();
    s.pop();
    while(!s.empty()){
        ss=ss+' '+s.top();
        s.pop();
    }
    cout<<setw(25)<<ss;

}

bool parser(vector<string>Class,vector <string> command ,map<string, map<string,vector<string> > >table){

    stack<string> s;

    s.push(";"); s.push("S");
    cout<<setw(15)<<"STACK"<<setw(25)<<"INPUT"<<setw(25)<<"ACTION\n";
    int i=0;
    while(i<Class.size()){
        stack_print(s);
        if(s.empty()) return false;
        string top=s.top();
        string token=(strcmp(Class[i].c_str(),"keyword")==0|| strcmp(Class[i].c_str(),"operator")==0 ||strcmp(Class[i].c_str(),"paran")==0)? command[i] : Class[i]; //bally

        string sss=command[i];
        for(int j=i+1;j<Class.size();j++){
            sss+=' '+command[j];
        }
        cout<<setw(25)<<sss;


        if(!(isupper(top[0]))){
            if(strcmp(top.c_str(),token.c_str())!=0) {cout<<"###\n";return false;}
            s.pop();
            i++;
        }else{
            if(!(table.find(top)!=table.end() && table[top].find(token)!=table[top].end()))
                return false;
            vector<string> rule = table[top][token];
            s.pop();
            if(strcmp(rule[0].c_str(),"eps") != 0){
                cout<<setw(20);
                for(int j=rule.size()-1;j>=0;j--){
                    cout<<rule[j]<<" ";
                    s.push(rule[j]);
                }

            }
        }
        cout<<endl;
    }
    if(!s.empty()) return false;
    return true;

}

class TreeNode
{
    private:
        string token;
        string type;

        TreeNode *parent;

        vector<TreeNode *> children;

        int countNodesRec(TreeNode *root, int& count){
            TreeNode *parent = root;
            TreeNode *child = NULL;

            for(int i = 0; i < parent->childrenNumber(); i++)
            {
                if (parent->token=="main") cout<<"--main--"<<endl;
                child = parent->getChild(i);
                count++;
                cout<<child->getToken()<<", "<<child->getType()<<" \t node : "<<count<<endl;
                if(child->childrenNumber() > 0)
                {
                    countNodesRec(child, count);
                }
            }

            return count;
        }

    public:
        TreeNode(){};
        TreeNode(string Token, string Type) :
            token(Token),
            type(Type),
            parent(NULL)
        {}

        void appendChild(TreeNode *child){
            child->setParent(this);
            children.push_back(child);
        }

        void setParent(TreeNode *theParent){
            parent = theParent;
        }

        TreeNode* getParent(){
                return parent;
        }

        TreeNode* getChild(int pos){
            if(children.size() < pos)
                return NULL;
            else
                return children[pos];
        }


        int childrenNumber(){
            return children.size();
        }

        int treeDFS(){
            int t = 0;

            if(children.size() < 1)
            {
                return 0;
            }

            countNodesRec(this, t);

            return t;
        }

        void setToken(string tok){
            token = tok;
        }

        void setType(string typ){
            type = typ;
        }

        string getToken(){
            return token;
        }

        string getType(){
            return type;
        }
};



int k;
void expTree(TreeNode *root, vector<string>s){
    if(k<0) return;
    TreeNode *node = root;

    if(s[k]=="*" || s[k]=="/" || s[k]=="+" || s[k]=="-" || s[k]=="^" || s[k]=="==" || s[k]==">" || s[k]=="<" || s[k]=="<=" || s[k]==">=" || s[k]=="==" || s[k]=="+=" || s[k]=="-=" || s[k]=="="){
        node->setToken(s[k]);
        node->setType("operator");
        k--;

        TreeNode *leftChild = new TreeNode();
        TreeNode *rightChild = new TreeNode();

        node->appendChild(leftChild);
        node->appendChild(rightChild);

        expTree(rightChild, s);
        expTree(leftChild, s);
    }
    else if((s[k][0] >= '0' && s[k][0] <= '9')){
        node->setToken(s[k]); node->setType("number"); k--;
        return;
    }
    else{
        node->setToken(s[k]); node->setType("variable"); k--;
        return;
    }
}


//Function to return precedence of operators
int prec(char c)
{
    if(c == '^')
        return 6;
    else if(c == '*' || c == '/')
        return 5;
    else if(c == '+' || c == '-')
        return 4;
    else if(c == '>' || c == '<' || c == '=' || c == '#' || c == '@' || c == '$' || c == '~' || c == '`')
        return 3;
    else if(c == '&')
        return 2;
    else if(c == '|')
        return 1;
    else
        return -1;
}
// The main function to convert infix expression
//to postfix expression
string infixToPostfix(string s)
{
    stack<char> st;
    st.push('N');
    int l = s.length();
    string d;
    for (int i=0; i < l; i++){
        // Exchange >= with $
        if(s[i]=='>' && i+1<l){
            if(s[i+1]=='='){
                d+='$';
                i++; continue;
            }
        }
        // Exchange <= with #
        else if(s[i]=='<' && i+1<l){
            if(s[i+1]=='='){
                d+='#';
                i++; continue;
            }
        }
        // Exchange == with @
        else if(s[i]=='=' && i+1<l){
            if(s[i+1]=='='){
                d+='@';
                i++; continue;
            }
        }
        // Exchange += with ~
        else if(s[i]=='+' && i+1<l){
            if(s[i+1]=='='){
                d+='~';
                i++; continue;
            }
        }
        // Exchange -= with `
        else if(s[i]=='-' && i+1<l){
            if(s[i+1]=='='){
                d+='`';
                i++; continue;
            }
        }

        d+=s[i];

    }

    s=d;
    l = s.length();
    string ns;
    for(int i = 0; i < l; i++)
    {
        // If the scanned character is an operand, add it to output string.
        if((s[i] >= 'a' && s[i] <= 'z')||(s[i] >= 'A' && s[i] <= 'Z')||(s[i] >= '0' && s[i] <= '9'))
            {
                while(i<l && ((s[i] >= 'a' && s[i] <= 'z')||(s[i] >= 'A' && s[i] <= 'Z')||(s[i] >= '0' && s[i] <= '9')) ){
                        ns+=s[i];
                        i++;
                }
                if(i<l) i--;
                ns+=" ";
            }


        // If the scanned character is an ‘(‘, push it to the stack.
        else if(s[i] == '(')

        st.push('(');

        // If the scanned character is an ‘)’, pop and to output string from the stack
        // until an ‘(‘ is encountered.
        else if(s[i] == ')')
        {
            while(st.top() != 'N' && st.top() != '(')
            {
                char c = st.top();
                st.pop();
               ns += c;
               ns += " ";
            }
            if(st.top() == '(')
            {
                char c = st.top();
                st.pop();
            }
        }

        //If an operator is scanned
        else{
            while(st.top() != 'N' && prec(s[i]) <= prec(st.top()))
            {
                char c = st.top();
                st.pop();
                ns += c;
                ns += " ";
            }
            st.push(s[i]);
        }
    }
    //Pop all the remaining elements from the stack
    while(st.top() != 'N')
    {
        char c = st.top();
        st.pop();
        ns += c;
        ns += " ";
    }

    d="";
    for (int i=0; i < ns.length(); i++){
        switch(ns[i]){
            case '$':
                d+=">=";
                break;
            case '#':
                d+="<=";
                break;
            case '@':
                d+="==";
                break;
            case '~':
                d+="+=";
                break;
            case '`':
                d+="-=";
                break;
            default:
                d+=ns[i];
                break;
        }
    }

    return d;

}

TreeNode *infixHandle(string ss){

    string s = ss;
    string exp = infixToPostfix(s);
    vector<string>postfix;
    for(int i=0; i<exp.length(); i++){
        string x;
        while(i<exp.length() && exp[i]!=' ')
            { x+=exp[i]; i++; }
        postfix.push_back(x);
    }
    k=postfix.size()-1;
    TreeNode *postfixTree=new TreeNode();
    //TreeNode *child=new TreeNode(); postfixTree->appendChild(child);
    expTree(postfixTree, postfix);
    return postfixTree;
}



int currentTok=0;
void generateTree(TreeNode *parent){

    TreeNode *node = parent;
    string nodeType = node->getType();
    string nodeVal = node->getToken();

    while(currentTok<tokens.size()){

        if(tokens[currentTok]=="}") {
                currentTok++;
                return;
        }

        if(nodeType=="block"){
            TreeNode *child = new TreeNode();
            node->appendChild(child);
            generateTree(child);
            continue;
        }

        if(types[currentTok]=="keyword"){

            if(tokens[currentTok]=="int" || tokens[currentTok]=="float"){
                node->setToken(tokens[currentTok]) ; node->setType("datatype") ;
                currentTok++;

                    string s;
                    while(tokens[currentTok]!=";"){
                        s+=tokens[currentTok];
                        currentTok++;
                    }
                    TreeNode *val = infixHandle(s);
                    node->appendChild(val);

                currentTok++; //Skip separator

                return;
            }

            else if(tokens[currentTok]=="if" || tokens[currentTok]=="loop"){


                node->setToken(tokens[currentTok]) ; node->setType("keyword") ;
                currentTok++;
                if(tokens[currentTok]=="("){
                    currentTok++;
                    string s;
                    while(tokens[currentTok]!=")"){
                        s+=tokens[currentTok];
                        currentTok++;
                    }
                    TreeNode *condition = infixHandle(s);
                    node->appendChild(condition);
                    currentTok++;
                }

                if(tokens[currentTok]=="{"){

                    TreeNode *block = new TreeNode("code", "block");
                    node->appendChild(block);
                    currentTok++;
                    generateTree(block);
                }
                return;
            }

            else if(tokens[currentTok]=="else"){
                currentTok++;
                if(tokens[currentTok]=="{"){

                    TreeNode *block = new TreeNode("code", "block");
                    node->appendChild(block);
                    currentTok++;
                    generateTree(block);
                }
                return;
            }
        }
    }
}

int main(){
    string line;
    ifstream file;
    file.open("mycode.gh");
    while(getline(file,line)){
        line+=" ";
        separation(line);
    }
    DFA();
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i]<<"\t \t"<<types[i]<<endl;
    }

    comman_grouping();

    build_parser_table();

    cout<<"\n--- CODE INSTRUCTIONS ---\n\n";

    for(int i=0;i<commands.size();i++){
        for(int j=0;j<commands[i].size();j++)
            cout<<commands[i][j]<<" ";
        cout<<endl;
    }

    cout<<"\n--- CODE CLASSIFIED ---\n\n";

    for(int i=0;i<commands.size();i++){
        for(int j=0;j<commands[i].size();j++)
            cout<<ctypes[i][j]<<" ";
        cout<<endl;
    }

    cout<<"\n---ALGORITHM PREVIEW---\n\n";
    int fail = 0;
    for(int i=0;i<commands.size();i++){
        if(parser(ctypes[i],commands[i],table1)) cout<<"success 1\n";
        else { cout<<"Fail\n"; fail = 1; }
        cout<<"\n\n\n";
    }
    file.close();

    if(!fail){
        cout<<"\n\n----TREE DFS----\n\n";
        TreeNode *root = new TreeNode("main", "block");
        generateTree(root);
        cout<<root->treeDFS();
    }
    else cout<<"\n\n----SYNTAX FAILURE----\n----TREE NOT GENERATED----\n";
    return 0;
}
