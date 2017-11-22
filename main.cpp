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
vector<vector<string> > commands;//bally
vector<vector<string> > ctypes;//bally

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

void comman_grouping (){ //bally
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

void ss(stack<string> s){
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
        ss(s);
        if(s.empty()) return false;
        string top=s.top();
        string token=(strcmp(Class[i].c_str(),"keyword")==0|| strcmp(Class[i].c_str(),"operator")==0 ||strcmp(Class[i].c_str(),"paran")==0)? command[i] : Class[i]; //bally

        cout<<setw(25)<<token;

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

    for(int i=0;i<commands.size();i++){
        for(int j=0;j<commands[i].size();j++)
            cout<<commands[i][j]<<" ";
        cout<<endl;
    }
    for(int i=0;i<commands.size();i++){
        for(int j=0;j<commands[i].size();j++)
            cout<<ctypes[i][j]<<" ";
        cout<<endl;
    }
    for(int i=0;i<commands.size();i++){
        if(parser(ctypes[i],commands[i],table1)) cout<<"success 1\n";
        else cout<<"Fail\n";
        cout<<"\n\n\n";
    }
    file.close();
    return 0;
}
