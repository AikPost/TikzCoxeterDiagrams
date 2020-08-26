#include<iostream>
#include<random>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<string>
#include<math.h>
#include<cstdlib>

using namespace std;


#include <sstream>

namespace patch
{
    template < typename T > string to_string( const T& n )
    {
        ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


//VARIABLEN --------------------------------

double ux=2;
double uy=2;
double nodedist=1.95; //Abstand der Knoten
double labeldist=0.15;
double linewidth=0.9; //Liniendicke in mm
string nodecolor="black";
string nodelabelcolor="white";
string linelabelcolor="black";
string Textmodifier="\\small"; // Enter latex text style modifier for the node labels, like /small /huge,...

double orbitradius=0.45;
double orbitlinewidth = 0.4;
double reldiagdist=2.5;


//constants

double Pi=3.14159265;

//CODE--------------------------------------

class D2V{
public:
  double a;
  double b;

  D2V(double ia,double ib):a(ia),b(ib){};

  void print(){
    cout << "(" << a << "," << b << ") \n";
  }

  string getTupel(){
    return "(" + patch::to_string(a)  + "," + patch::to_string(b) + ")";
  }

  double average(){
    return (a+b)/2;
  }


  D2V add(D2V p2){
    return D2V(a+p2.a,b+p2.b);
  }


  D2V subtract(D2V p2){
    return D2V(a-p2.a,b-p2.b);
  }

  D2V getvect(D2V p2){
    D2V ret=D2V(a-p2.a,b-p2.b);
    return ret;

  }

  D2V orthonorm(){
    D2V ret=D2V(b/sqrt(a*a + b*b),-a/sqrt(a*a + b*b));
    return ret;

  }

  D2V scale(double scalar){
    return D2V(scalar*a,scalar*b);

  }

  bool compare(D2V other){
    return (a == other.a && b == other.b);
  }

};

class Knoten{
public:
  Knoten *prev;
  string ord_prev;
  string nodelabel;
  double xpos;
  double ypos;
  Knoten *next1;
  Knoten *next2;
  Knoten *next3;
  Knoten *next4;

public:

  Knoten(Knoten *p, double winkel, string ord,string nl):prev(NULL),xpos(ux),ypos(uy),next1(NULL),next2(NULL),next3(NULL),next4(NULL),ord_prev(ord),nodelabel(nl){
    prev=p;
    xpos=p->xpos + nodedist*cos(2*Pi/360*winkel);
    ypos=p->ypos + nodedist*sin(2*Pi/360*winkel);
    if(!prev->next1) prev->next1=this;
    else if(!prev->next2) prev->next2=this;
      else if(!prev->next3) prev->next3=this;
        else if(!prev->next4) prev->next4=this;



  };

  Knoten(string nl):prev(NULL),xpos(ux),ypos(uy),next1(NULL),next2(NULL),next3(NULL),next4(NULL),ord_prev("0"),nodelabel(nl){};


  void Kprint(){
    cout <<this->nodelabel << ": X,Y,order: " << this->xpos << "," << this->ypos << ", "<< this->ord_prev <<"\n";


  }

  void clear(){
    this->next1=NULL;
    this->next2=NULL;
    this->next3=NULL;
    this->next4=NULL;

  }

  D2V getouternodesxcoord(){
    D2V r=  D2V(this->xpos,this->xpos);
    D2V comp = D2V(0,0);
    if(this->next4) {
      comp=this->next4->getouternodesxcoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next3) {
      comp=this->next3->getouternodesxcoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next2) {
      comp=this->next2->getouternodesxcoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next1){
      comp=this->next1->getouternodesxcoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }else return D2V(this->xpos,this->xpos);
    if(this->xpos < r.a) r.a = this->xpos;
    if(this->xpos >r.b) r.b = this->xpos;
    return r;

  }

  D2V getouternodesycoord(){
    D2V r=  D2V(this->ypos,this->ypos);
    D2V comp = D2V(0,0);
    if(this->next4) {
      comp=this->next4->getouternodesycoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next3) {
      comp=this->next3->getouternodesycoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next2) {
      comp=this->next2->getouternodesycoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }
    if(this->next1){
      comp=this->next1->getouternodesycoord();
      if(r.a > comp.a) r.a=comp.a;
      if(r.b < comp.b) r.b=comp.b;
    }else return D2V(this->ypos,this->ypos);
    if(this->ypos < r.a) r.a = this->ypos;
    if(this->ypos > r.b) r.b = this->ypos;
    return r;

  }

  void centerx(double tx){
    double cent = this->getouternodesxcoord().average();
    this->shiftx((-1)*cent+tx);

  }

  void centery(double ty){
    double cent = this->getouternodesycoord().average();
    this->shifty((-1)*cent+ty);

  }

  void shiftx(double value){
    if(this->next4) this->next4->shiftx(value);
    if(this->next3) this->next3->shiftx(value);
    if(this->next2) this->next2->shiftx(value);
    if(this->next1) this->next1->shiftx(value);
    this->xpos += value;
    return;

  }

  void shifty(double value){
    if(this->next4) this->next4->shifty(value);
    if(this->next3) this->next3->shifty(value);
    if(this->next2) this->next2->shifty(value);
    if(this->next1) this->next1->shifty(value);
    this->ypos += value;
    return;

  }

  D2V getposofnamednode(string Name){
    D2V ret = D2V(-100000,-100000);
    D2V comp=ret;
    if(this->nodelabel == Name) {
      ret= D2V(this->xpos,this->ypos);
    }else{
      if(this->next1 && ret.compare(comp)){
        ret=this->next1->getposofnamednode(Name);
      }
      if(this->next2 && ret.compare(comp)){
        ret=this->next2->getposofnamednode(Name);
      }
      if(this->next3 && ret.compare(comp)){
        ret=this->next3->getposofnamednode(Name);
      }
      if(this->next4 && ret.compare(comp)){
        ret=this->next4->getposofnamednode(Name);
      }
    }
    return ret;


  }

  string tikznode(){
    return "\\draw[" + nodecolor + ",fill="+ nodecolor +"] ("+ patch::to_string(this->xpos) + "," + patch::to_string(this->ypos) + ") circle (.3) node[text="+ nodelabelcolor + "] {"+ Textmodifier +" "+ nodelabel + "}; \n";

  }

  string tikzline(){
    string r;
    string four="4";
    string negone="-1";
    double nx=0,ny=0;
    if(this->ord_prev == "4") {
      r= "\\draw[double, line width="+ patch::to_string(linewidth) + "mm] ";
    }else if(this->ord_prev == "-1"){
       r= "\\draw[dashed, line width="+ patch::to_string(linewidth) + "mm] ";
    }else r= "\\draw[line width="+ patch::to_string(linewidth) + "mm] ";
    r+="(" +  patch::to_string(this->xpos) + "," + patch::to_string(this->ypos) + ") -- ";
    r+="(" +  patch::to_string(prev->xpos) + "," + patch::to_string(prev->ypos) +"); \n";

  	if(this->ord_prev != "4" && this->ord_prev != "3" && this->ord_prev != "-1") {
      if(this->xpos - prev->xpos > 0){
        nx=(this->xpos+prev->xpos)/2+labeldist*(prev->ypos - this->ypos);
        ny=(this->ypos+prev->ypos)/2+labeldist*(this->xpos - prev->xpos);

      }else{
        nx=(this->xpos+prev->xpos)/2-labeldist*(prev->ypos - this->ypos);
        ny=(this->ypos+prev->ypos)/2-labeldist*(this->xpos - prev->xpos);
      }
      r+= "\\node[text="+ linelabelcolor + "] at (" + patch::to_string(nx) + "," + patch::to_string(ny) +") {"+ this->ord_prev + "}; \n";

    }
    return r;

  }

  string Diagret(){
    string ret= "\n%%----------- Diagram ----------------------------\n";
    ret+= "\n%%Lines: \n";
    ret += this->Lineret();
    ret += "\n \n%%Nodes: \n";
    ret += this->Nodesret();
    ret += "\n";
    ret += "\n%%------------------------------------------------------\n";
    return ret;
  }

  string Nodesret(){
    string r="";
    if(this->next4) r+=this->next4->Nodesret();
    if(this->next3) r+=this->next3->Nodesret();
    if(this->next2) r+=this->next2->Nodesret();
    if(this->next1) r+=this->next1->Nodesret();
    r+= this->tikznode();

    return r;
  }

  string Lineret(){
    string r="";
    if(this->next4) r+=this->next4->Lineret();
    if(this->next3) r+=this->next3->Lineret();
    if(this->next2) r+=this->next2->Lineret();
    if(this->next1) r+=this->next1->Lineret();
    if(this->prev) r+= this->tikzline();

    return r;
  }

  string Interpret(string in){
    string order;
    bool processed;
    double angle;
    string anglestr="";
    int i=0;
    int j=0;
    int k=0;
    int max= 2*in.length();

    while(!(in.substr(i,1) == "(" || in.substr(i,1)=="[" || in.substr(i,1)==")"  || i==in.length() )){
      i++;
    }
    this->nodelabel = in.substr(0,i); // Name dieses Nodes abgehakt
    in.erase(0,i);
    i=0;

    while(in != "" && j < 10*in.length() ){
      processed=true;
      order="";


      if(in.substr(0,1)== ")"){
        processed=false;
        return in.erase(0,1);

      }
      if(processed){
        if(in.substr(0,1) == "("){
          processed=false;
          in.erase(0,1);

          k=0;
          while(k< in.length()){
            if(in.substr(k,1) == ","){
              order=in.substr(0,k);
              in.erase(0,k+1);
              if(!this->next1){
                this->next1= new Knoten(this,0,order,"a");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next1->Interpret(in);
                break;
              }
              if(!this->next2){
                this->next2= new Knoten(this,0,order,"b");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next2->Interpret(in);
                break;
              }
              if(!this->next3){
                this->next3= new Knoten(this,0,order,"c");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next3->Interpret(in);
                break;
              }
              if(!this->next4){
                this->next4= new Knoten(this,0,order,"d");
                  //cout << "\nÜbergabe: " << in << "\n";
                in= this->next4->Interpret(in);
                break;
              }

            }
            k++;
          }

        }
      }

      if(processed){

        if(in.substr(0,1) == "["){


          in.erase(0,1);
          k=0;
          while(k< in.length()){
            if(in.substr(k,1) == "]"){
              anglestr=  in.substr(0,k);
            //  cout <<anglestr << ";\n";
              angle = stod(anglestr);
            //  cout << "Done; \n";
              in.erase(0,k+2);
              break;
            }
            k++;
          }
          k=0;

          while(k< in.length()){
            if(in.substr(k,1) == ","){
              order=in.substr(0,k);
              in.erase(0,k+1); //löscht order und , nächstes muss name sein
              k=0;
              if(!this->next1){
                this->next1= new Knoten(this,angle,order,"a");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next1->Interpret(in);
                break;
              }
              if(!this->next2){
                this->next2= new Knoten(this,angle,order,"b");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next2->Interpret(in);
                break;
              }
              if(!this->next3){
                this->next3= new Knoten(this,angle,order,"c");
                //cout << "\nÜbergabe: " << in << "\n";
                in= this->next3->Interpret(in);
                break;
              }
              if(!this->next4){
                this->next4= new Knoten(this,angle,order,"d");
                  //cout << "\nÜbergabe: " << in << "\n";
                in= this->next4->Interpret(in);
                break;
              }

            }
            k++;
          }


        }
      }



      if (i < in.length() ){
        i++;
      }else i=0;
    //  cout << in << "Aktuelle Eingabe \n";



    }
  return in;

  }



};


class Orbits{
public:
  string permutation;
  Knoten *startnode;
  int i;

public:
  Orbits(Knoten *nodein,string perm):permutation(perm),startnode(nodein){};

  void printperm(){
    cout << permutation <<"\n";
  }

  string twoorbitcode(string n1, string n2){
    string ret="";
    D2V pos1= startnode->getposofnamednode(n1);
    D2V pos2= startnode->getposofnamednode(n2);
    D2V orth= pos1.getvect(pos2).orthonorm().scale(orbitradius);

    ret+="%% Orbit---------------------------------\n";
    ret+="\\draw[dotted, line width="+ patch::to_string(orbitlinewidth) + "mm] " + pos1.getTupel() +" circle (" + patch::to_string(orbitradius) + ");\n";
    ret+="\\draw[dotted, line width="+ patch::to_string(orbitlinewidth) + "mm] " + pos2.getTupel() +" circle (" + patch::to_string(orbitradius) + ");\n";
    ret+="\\draw[fill=white,white] " + pos1.add(orth).getTupel() +  " -- " + pos1.subtract(orth).getTupel() +" -- "+ pos2.subtract(orth).getTupel() + " -- " + pos2.add(orth).getTupel() + " -- cycle;\n";
    ret+="\\draw[dotted, line width="+ patch::to_string(orbitlinewidth) + "mm] " +pos1.subtract(orth).getTupel() +" -- "+ pos2.subtract(orth).getTupel() +" ;\n";
    ret+="\\draw[dotted, line width="+ patch::to_string(orbitlinewidth) + "mm] " + pos1.add(orth).getTupel() +" -- "+ pos2.add(orth).getTupel() + ";\n";
    ret+="%% Orbitend -------------------------------------\n\n";
    return ret;
  }

  string oneorbitcode(string n){
    string ret="";
    D2V pos1= startnode->getposofnamednode(n);

    ret+="%% Orbit---------------------------------\n";
    ret+="\\draw[dotted, line width="+ patch::to_string(orbitlinewidth) + "mm] " + pos1.getTupel() +" circle (" + patch::to_string(orbitradius) + ");\n";
    ret+="%% Orbitend -------------------------------------\n\n";
    return ret;
  }

  string allorbitcode(){
    string str=permutation;
    int lenorig=str.length();
    bool processed=true;
    string ret="";
    string Nodename1,Nodename2;
    int i,j;
    int ctsel=0; //countertostopendlessloop
    if(str.length() == 0) return "";


    while(str.length()>0 && ctsel< lenorig) {
      processed=true;
      i=0;j=0;
      if(str.substr(0,1) != "("){//No ( could be found
        cout << "Wrong format\n" << "\n";
        cout << str  << "\n";
        return "";
      }

      if(str.substr(0,1) == "(") str.erase(0,1); //erases (
      i=0;
      while(i<str.length() && str.substr(i,1) != "&" && str.substr(i,1)!=")"){//searches for codespecifics
        i++;
      }

      if(i >= str.length()) {// no valid input
        cout << "error\n";
        return "";
      }

      if(str.substr(i,1) == "&"){// Creates a twoorbit
        Nodename1=str.substr(0,i);
        str.erase(0,i+1); //first Nodename found, search for next

        j=0;
        while(j<str.length() && str.substr(j,1)!=")"){//search for end of permutation
          j++;
        }
        if(j >= str.length()) {// not found
          cout << "error\n";
          cout << str << "\n";
          return "";
        }
        Nodename2=str.substr(0,j); // second Nodename found
        str.erase(0,j+1);
        ret+=twoorbitcode(Nodename1,Nodename2);
        processed=false;
      }
      if(processed){
        if(str.substr(i,1) == ")"){// Creates a oneorbit
          Nodename1=str.substr(0,i);
          str.erase(0,i+1); //first Nodename found, search for next


          ret+=oneorbitcode(Nodename1);
        }
      }
      ctsel++;

    }

    return ret;
  }

};
//_________________________________________Knoten____________________________________________

Knoten *node1 = new Knoten("1");
Knoten *node2 = new Knoten("2");
Knoten *node3 = new Knoten("3");
Knoten *node4 = new Knoten("4");
Knoten *node5 = new Knoten("5");

Orbits Orb1= Orbits(node1,"");
Orbits Orb2= Orbits(node2,"");
Orbits Orb3= Orbits(node3,"");
Orbits Orb4= Orbits(node4,"");
Orbits Orb5= Orbits(node5,"");

string Input1;
string Input2;
string Input3;
string Input4;
string Input5;

Knoten *getnode(int in){
  switch(in){
    case(1):return node1;
    break;
    case(2):return node2;
    break;
    case(3):return node3;
    break;
    case(4):return node4;
    break;
    case(5):return node5;
    break;
    default: return NULL;

  }
  return NULL;
}

Orbits getOrbits(int in){
  switch(in){
    case(1):return Orb1;
    break;
    case(2):return Orb2;
    break;
    case(3):return Orb3;
    break;
    case(4):return Orb4;
    break;
    case(5):return Orb5;
    break;
    default: cout << "Fehler"; return Orb1;

  }
  return Orb1;
}

//________________________________________________Console_______________________________________

int settings(){
  string Option;
  cout << "Current settings: \n";
  cout << "[A] First node x: " << ux << "\n";
  cout << "[B] First node y: " << uy << "\n";
  cout << "[C] Distance of nodes:" << nodedist << "\n";
  cout << "[D] Distance of labels to lines:" << labeldist << "\n";
  cout << "[E] Linewidth:" << linewidth << "\n";
  cout << "[F] Linelabelcolor:" << linelabelcolor << "\n";

  cout << "[G] Nodecolor:" << nodecolor << "\n";
  cout << "[H] Nodelabelcolor:" << nodelabelcolor << "\n";
  cout << "[I] Textmodifier:" << Textmodifier << "\n";

  cout << "[X] Exit \n";
  cout << "Select a letter to change the settings:\n";
  cin >> Option;
  if (Option== "A"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> ux;
  }
  if (Option== "B"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> uy;
  }
  if (Option== "C"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> nodedist;
  }
  if (Option== "D"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> labeldist;
  }
  if (Option== "E"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> linewidth;
  }
  if (Option== "F"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> linelabelcolor;
  }
  if (Option== "G"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> nodecolor;
  }
  if (Option== "H"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> nodelabelcolor;
  }
  if (Option== "I"){
    cout << "Enter new Setting in the appropriate format:\n";
    cin >> Textmodifier;
  }

  if (Option== "X"){
    return 0;
  }
  return 1;


}

int testcode(){
  string msg = "\nPress [Enter] to continue...";

  Orb1=Orbits(node1,"(1)(2)(n-1)(n)(n+1&n+2)(n+3&n+4)(m-1&m)(m+1&m+2)");
  node1->Interpret("1(a,2(-1,n-1(b,n[60](3,n+1(c,n+3(-1,m-1(d,m+1))))[-60](3,n+2(c,n+4(-1,m(d,m+2)))))))");
  node1->centerx(0);
  node2->Interpret("1(a,2(-1,n-1(b,n(4,n+1(c,n+3(-1,m-1(d,m+1)))))))");
  node2->centerx(0);
  Orb1.printperm();

  cout << msg << "\n";
  cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
  cin.clear();
  cin.get();

  return 0;
}

int coxenter(){
  Knoten *akt;
  char Option;
  string msg = "\nPress [Enter] to continue...";
  cout << "Coxeter Diagrams | Get Code | Enter Code | Get tikz | \n";
  cout << "Diagram 1        |    [A]   |     [a]    |   [1]    | \n";
  cout << "Diagram 2        |    [B]   |     [b]    |   [2]    | \n";
  cout << "Diagram 3        |    [C]   |     [c]    |   [3]    | \n";
  cout << "Diagram 4        |    [D]   |     [d]    |   [4]    | \n";
  cout << "Diagram 5        |    [E]   |     [e]    |   [5]    | \n";
  cout << "[X] to return to menu \n";
  cin >> Option;

  switch(Option){
    case 'A': cout << Input1;
              break;
    case 'B': cout << Input2;
              break;
    case 'C': cout << Input3;
              break;
    case 'D': cout << Input4;
              break;

    case 'E': cout << Input5;
              break;

    case 'X': return 0;

    case 'a':
      cout << "Enter: \n";
      cin >> Input1;
      break;
    case 'b':
      cout << "Enter: \n";
      cin >> Input2;
      break;
    case 'c':
      cout << "Enter: \n";
      cin >> Input3;
      break;
    case 'd':
      cout << "Enter: \n";
      cin >> Input4;
      break;
    case 'e':
      cout << "Enter: \n";
      cin >> Input5;
      break;

    case '1':
      msg="";
      akt=getnode(1);
      akt->clear();
      akt->Interpret(Input1);
      akt->centerx(0);
      system("cls");
      cout << "\n"<<"%%"<<"Lines \n";
      cout << akt->Lineret();
      cout << "\n%%" << "Nodes \n";
      cout << akt->Nodesret();
      break;
    case '2':
      msg="";
      akt=getnode(2);
      akt->clear();
      akt->Interpret(Input2);
      akt->centerx(0);
      system("cls");
      cout << "\n"<<"%%"<<"Lines \n";
      cout << akt->Lineret();
      cout << "\n%%" << "Nodes \n";
      cout << akt->Nodesret();
      break;
    case '3':
      msg="";
      akt=getnode(3);
      akt->clear();
      akt->Interpret(Input3);
      akt->centerx(0);
      system("cls");
      cout << "\n"<<"%%"<<"Lines \n";
      cout << akt->Lineret();
      cout << "\n%%" << "Nodes \n";
      cout << akt->Nodesret();
      break;
    case '4':
      msg="";
      akt=getnode(4);
      akt->clear();
      akt->Interpret(Input4);
      akt->centerx(0);
      system("cls");
      cout << "\n"<<"%%"<<"Lines \n";
      cout << akt->Lineret();
      cout << "\n%%" << "Nodes \n";
      cout << akt->Nodesret();
      break;
    case '5':
      msg="";
      akt=getnode(5);
      akt->clear();
      akt->Interpret(Input5);
      akt->centerx(0);
      system("cls");
      cout << "\n"<<"%%"<<"Lines \n";
      cout << akt->Lineret();
      cout << "\n%%" << "Nodes \n";
      cout << akt->Nodesret();
      break;


    default: return 2;
  }


  cout << msg << "\n";
  cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
  cin.clear();
  cin.get();
  cout << "\n";
  return 2;

}

int enterorbit(){
  string msg = "\nPress [Enter] to continue...";
  int Option=-1;
  string Inputcode;
  while(Option > 5 || Option < 0){
    cout << "\n[0] Exit\nFor which diagram do you want to enter an orbit? [1-5]?\n";
    cin >> Option;
    if (Option==0) return 0;
  }
  cout << "Enter Orbit Code:\n";
  cin >> Inputcode;
  if(Option== 1)  Orb1= Orbits(node1,Inputcode);
  if(Option== 2)  Orb2= Orbits(node2,Inputcode);
  if(Option== 3)  Orb3= Orbits(node3,Inputcode);
  if(Option== 4)  Orb4= Orbits(node4,Inputcode);
  if(Option== 5)  Orb5= Orbits(node5,Inputcode);



  cout << msg << "\n";
  cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
  cin.clear();
  cin.get();
  cout << "\n";
  return 2;


}

int makeindex(){
  int Diagabs=-1;
  int Diagrel=-1;
  while(Diagabs >5 || Diagabs <0){
    cout << "\n[0] Exit\nWhich diagram should be the absolute one [1-5]?\n";
    cin >> Diagabs;
    if(Diagabs == 0 ) return 0;
  }
  Knoten *nodeabs= getnode(Diagabs);
  Orbits absOrb=getOrbits(Diagabs);
  while(Diagrel >5 || Diagrel <0){
    cout << "\n[0] Exit\nWhich diagram should be the relative one? [1-5] \n";
    cin >> Diagrel;
    if(Diagrel == 0 ) return 0;

  }

  Knoten *noderel = getnode(Diagrel);

  cout << "Hallo----------------------------------------";
  double minabs = nodeabs->getouternodesycoord().a;
  double maxrel = noderel->getouternodesycoord().b;


  //cout << minabs << ":" << maxrel << ":";
  noderel->shifty(-reldiagdist-maxrel+minabs);
  maxrel = noderel->getouternodesycoord().b;
  cout << maxrel << ";\n";


  system("cls");
  cout << "%%Orbits" << "\n";


  cout << absOrb.allorbitcode() << "\n";

  cout << "%%Absolute Diagram: \n";
  cout << nodeabs->Diagret();
  cout << "%%Relative Diagram: \n";
  cout << noderel->Diagret();


  cout << "\\draw[<->]" << D2V(0,minabs).add(D2V(0,0.25*-reldiagdist)).getTupel() << " -- " << D2V(0,maxrel).add(D2V(0,0.25*reldiagdist)).getTupel()  << ";\n";

  cout << "\n";

  cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
  cin.clear();
  cin.get();
  cout << "\n";
  return 0;



}

int topmenu(){
  string Option;
  int r=-1;
  while(true){
    system("cls");
    r=-1;
    cout << "What do you want to do? \n";
    cout << "[A] Enter coxeterdiagrams\n";
    cout << "[B] Enter Orbitcode\n";
    cout << "[C] Make Tits Index\n";
    cout << "[D] Testcode \n";
    cout << "[E] Settings\n";
    cin >> Option;
    if(Option == "A") {
      while(r!=0){
        system("cls");
        r=coxenter();
      }
    }
    if(Option == "B"){

      while(r!=0){
        system("cls");
        r=enterorbit();
      }
    }

    if(Option == "C"){

      while(r!=0){
        system("cls");
        r=makeindex();
      }
    }

    if(Option == "D"){
      while(r!=0){
        system("cls");
        r=testcode();
      }
    }
    if(Option == "E"){

      while(r!=0){
        system("cls");
        r=settings();
      }
    }
    Option="";
  }


  return 1;

}


int main(){

  topmenu();
  return 1;
}
