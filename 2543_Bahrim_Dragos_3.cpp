#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <stack>
#include <typeinfo>
using namespace std;


// https://stackoverflow.com/questions/4077609/overloading-output-stream-operator-for-vectort
template<typename T>
ostream& operator<<(ostream& out, vector<T> t) {
    for (size_t i = 0; i < t.size(); i++) {
        out << t[i] << endl;
    }
    out<<endl;
    return out;
}

template<typename T>
istream& operator>>(istream& in, vector<T> *t) {
    int counter;
    cout<<"Numar elemente:";
    in>>counter;
    T aux;
    for (int i = 0; i < counter; i++) {
        cin>>aux;
        try{
        t->push_back(aux);
        }
        catch(bad_alloc){
            cout<<"ALLOCATION ERROR";
            exit(EXIT_FAILURE);
        }
    }
    return in;
}


class Singleton {
private:
   static Singleton *instance;
   string *data;
   int nr;
   Singleton() {
        nr=0;
        data = NULL;
   }
public:
    static Singleton* getInstance() {
        if (!instance)
            instance = new Singleton;
        return instance;
    }
    void insertData(string data) {
        string *aux=this->data;
        nr+=1;
        try{
            this->data=new string[nr];
        }
        catch(bad_alloc){
            cout<<"Bad Alloc";
            exit(EXIT_FAILURE);
        }

        for(int i=0;i<nr-1;i++){
            this->data[i]=aux[i];
        }
        this->data[nr-1]=data;
    }
    void viewData(){
        for(int i=0;i<nr;i++)
            cout<<data[i]<<endl;
        cout<<"Optiune: ";
    }
};
Singleton* Singleton::instance = 0;

class Examen{
private:
    static int ExamenSeed;
protected:
    int id_examen;
    string denumire;
    double nota_scris;
public:
    Examen(string,double,int);
    Examen(Examen&);
    virtual ~Examen();
    Examen& operator=(Examen&);
    Examen& operator=(const Examen&);
    virtual void citire(istream&);  ///citire virtuala - late binding
    virtual void afisare(ostream&); ///afisare virtuala - late binding
    friend istream& operator>>(istream&, Examen&);
    friend ostream& operator<<(ostream&, Examen&);
    static int getSeed(){return ExamenSeed;}
    void set_nota_scris(double nota_examen){
        this->nota_scris=nota_examen;
    }
    void set_denumire(string denumire){
        this->denumire=denumire;
    }
    double get_nota_scris(){
        return this->nota_scris;
    }
    string get_denumire(){
        return this->denumire;
    }
    int getNotaScris() const {return nota_scris;}
    operator int(){return id_examen;}

};

class Examen_Final;
int Examen::ExamenSeed=0;

Examen::Examen(string denumire="", double nota_scris=0,int id_examen=-1){
    try {
        if(nota_scris<0 || nota_scris>10) throw nota_scris;
        this->nota_scris=nota_scris;
    }
    catch(double x){cout<<"Eroare in constructor, nota_scris nu poate fi <0 sau >10";exit(EXIT_FAILURE);}
    (id_examen==-1)? this->id_examen=ExamenSeed++ : this->id_examen = id_examen;
    this->denumire=denumire;
}
Examen::Examen(Examen &aux){
    id_examen=aux.id_examen;
    denumire=aux.denumire;
    nota_scris=aux.nota_scris;
}
Examen::~Examen(){
    id_examen=-1;
    denumire="";
    nota_scris=-1;
}
Examen& Examen::operator=(Examen& aux){
    if(this!=&aux){
        id_examen=aux.id_examen;
        denumire=aux.denumire;
        nota_scris=aux.nota_scris;
    }
    return *this;
}
Examen& Examen::operator=(const Examen& aux){
    if(this!=&aux){
        id_examen=aux.id_examen;
        denumire=aux.denumire;
        nota_scris=aux.nota_scris;
    }
    return *this;
}
void Examen::citire(istream& in){
    cin.get();
    cout<<"Introduceti denumirea: ";
    getline(in,denumire);
    cout<<"Introduceti nota_scris: ";
    in>>nota_scris;
    while( (nota_scris>10) || (nota_scris<0)){
        cout<<"\nNota scris nu poate fi >10 sau <0\nIntroduceti nota_scris: ";
        in>>nota_scris;
    }
}
void Examen::afisare(ostream& out){
    out<<"ID= "<<id_examen<<endl;
    out<<"Denumire= "<<denumire<<endl;
    out<<"Nota scris= "<<nota_scris<<endl;
}
istream& operator>>(istream& in, Examen& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Examen& aux){
    aux.afisare(out);
    return out;
}

class Partial: public Examen{
private:
    double nota_oral;
public:
    Partial(string,double,double,int);
    Partial(Partial&);
    Partial(Examen_Final&);
    ~Partial();
    Partial& operator=(Partial&);
    void citire(istream&);
    void afisare(ostream&);
    void set_nota_oral(double nota_examen){
        try{
            if(nota_examen>10 || nota_examen<0) throw nota_examen;
            this->nota_oral=nota_examen;
        } catch(double x){
            cout<<"Nota nu poate fi >10 || < 0 "<<x<<endl;
            exit(EXIT_FAILURE);
        }
    }
    double get_nota_oral(){
        return this->nota_oral;
    }
    friend istream& operator>>(istream&, Partial&);
    friend ostream& operator<<(ostream&, Partial&);
};

Partial::Partial(string denumire="",double nota_scris=0,double nota_oral=0,int id=-1):Examen(denumire,nota_scris,id){
    try{
        if(nota_oral>10 || nota_oral<0) throw nota_oral;
        this->nota_oral=nota_oral;
    }
    catch(double x){cout<<"Eroare in constructor. Nota oral nu poate fi <0 || >10\n";exit(EXIT_FAILURE);}
}
Partial::Partial(Partial &aux):Examen(aux){
    this->nota_oral=aux.nota_oral;
}
Partial::~Partial(){
    nota_oral=0;
}
Partial& Partial::operator=(Partial &aux){
    if(this!=&aux)
    {
        Examen::operator=(aux);
        this->nota_oral=aux.nota_oral;
    }
    return *this;
}
void Partial::citire(istream &in){
    Examen::citire(in);
    cout<<"Introduceti nota_oral: ";
    in>>nota_oral;
    while((nota_oral>10)||(nota_oral<0)){
        cout<<"Nota oral nu poate fi mai >10 || < 0\nIntroduceti nota_oral: ";
        in>>nota_oral;
    }
}
void Partial::afisare(ostream &out){
    Examen::afisare(out);
    cout<<"Nota oral: "<<nota_oral<<endl;

}
istream& operator>>(istream& in, Partial &aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Partial& aux){
    aux.afisare(out);
    return out;
}

class Intrebare{
private:
    string continut;
    int raspuns_corect;
    vector <string> raspunsuri;
public:
    Intrebare(string,int);
    Intrebare(Intrebare&);
    ~Intrebare();
    Intrebare& operator=(Intrebare&);
    void citire(istream&);
    void afisare(ostream&);
    friend istream& operator>>(istream&, Intrebare&);
    friend ostream& operator<<(ostream&, Intrebare&);
};

Intrebare::Intrebare(string continut="",int raspuns_corect=0){
    this->continut=continut;
    this->raspuns_corect=raspuns_corect;
}
Intrebare::Intrebare(Intrebare &aux){
    this->continut=aux.continut;
    this->raspuns_corect=aux.raspuns_corect;
    this->raspunsuri= aux.raspunsuri;
}
Intrebare::~Intrebare(){
    continut="";
    raspuns_corect=0;
    raspunsuri.clear();
}
Intrebare& Intrebare::operator=(Intrebare& aux){
    if(this!=&aux){
        this->continut=aux.continut;
        this->raspuns_corect=aux.raspuns_corect;
        this->raspunsuri= aux.raspunsuri;
    }
    return *this;
}
void Intrebare::citire(istream &in){
    in.get();
    cout<<"Introduceti continutul intrebarii: ";
    getline(in,continut);
    cout<<"Introduceti numarul de raspunsuri: ";
    int n;
    in>>n;
    while(true){
        cout<<"Introduceti numarul variantei corecte: ";
        in>>raspuns_corect;
        if(raspuns_corect<=n && raspuns_corect>0) break;
    }
    in.get();
    string raspuns;
    for(int i=0;i<n;i++){
        cout<<"Introduceti raspunsul "<<i<<" ";
        getline(in,raspuns);
        try{
            raspunsuri.push_back(raspuns);
        }
        catch(bad_alloc){
            cout<<"\nAllocation Faillure\n";
            exit(EXIT_FAILURE);
        }
    }
}
void Intrebare::afisare(ostream &out){
    out<<"Intrebare: "<<continut<<endl;
    out<<"Numar varianta corecta: "<<raspuns_corect<<endl;
    out<<"Raspunsuri\n";
    for (vector<string>::iterator it = raspunsuri.begin() ; it != raspunsuri.end(); ++it)
        out<<*it<<endl;
}
istream& operator>>(istream& in, Intrebare& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Intrebare& aux){
    aux.afisare(out);
    return out;
}

class Elev{
private:
    string nume;
    string prenume;
public:
    Elev(string,string);
    Elev(Elev&);
    Elev(const Elev&);
    ~Elev();
    Elev& operator=(Elev&);
    Elev& operator=(const Elev&);
    void citire(istream&);
    void afisare(ostream&);
    string getname(){
        return nume+" "+prenume;
    }
    friend istream& operator>>(istream&, Elev&);
    friend ostream& operator<<(ostream&, Elev&);
};

Elev::Elev(string nume="",string prenume=""){
    this->nume=nume;
    this->prenume=prenume;
}
Elev::Elev(Elev &aux){
    this->nume=aux.nume;
    this->prenume=aux.prenume;
}
Elev::Elev(const Elev &aux){
    this->nume=aux.nume;
    this->prenume=aux.prenume;
}
Elev::~Elev(){
    nume="";
    prenume="";
}
Elev& Elev::operator=(Elev& aux){
    if(this!=&aux){
        this->nume=aux.nume;
        this->prenume=aux.prenume;
    }
    return *this;
}
Elev& Elev::operator=(const Elev& aux){
    if(this!=&aux){
        this->nume=aux.nume;
        this->prenume=aux.prenume;
    }
    return *this;
}
void Elev::citire(istream &in){
    cout<<"Introduceti numele: ";
    in>>nume;
    cout<<"Introduceti prenumele: ";
    in>>prenume;
}
void Elev::afisare(ostream &out){
    out<<nume<<" "<<prenume;
}
istream& operator>>(istream& in, Elev& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Elev& aux){
    aux.afisare(out);
    return out;
}

class Quizz: public Examen{
private:
    vector<Intrebare*> intrebari;
public:
    Quizz(string,double,int id);
    Quizz(Quizz&);
    ~Quizz();
    Quizz& operator=(Quizz&);
    void citire(istream&);
    void citire_intrebari(istream&);
    void afisare(ostream&);
    void afisare_intrebari(ostream&);
    friend istream& operator>>(istream&, Quizz&);
    friend ostream& operator<<(ostream&, Quizz&);
};

Quizz::Quizz(string denumire="",double nota_scris=0,int id=-1):Examen(denumire,nota_scris,id){
}
Quizz::Quizz(Quizz& aux):Examen(aux){
    this->intrebari=aux.intrebari;
}
Quizz::~Quizz(){
    intrebari.clear();
}
Quizz& Quizz::operator=(Quizz &aux){
    if(this!=&aux){
        Examen::operator=(aux);
        this->intrebari=aux.intrebari;
    }
    return *this;
}
void Quizz::citire(istream &in){
    Examen::citire(in);
    int n;
    cout<<"Introduceti numarul de intrebari: ";
    in>>n;
    in.get();
    Intrebare *aux;
    for(int i=0;i<n;i++){
        aux=new Intrebare;
        in>>*aux;
        try{
            intrebari.push_back(aux);
        }
        catch(bad_alloc){
            cout<<"ALLOCATION FAILURE\n";
            exit(EXIT_FAILURE);
        }

    }
}
void Quizz::citire_intrebari(istream &in){
    int n;
    cout<<"Introduceti numarul de intrebari: ";
    in>>n;
    in.get();
    Intrebare *aux;
    for(int i=0;i<n;i++){
        aux=new Intrebare;
        in>>*aux;
        intrebari.push_back(aux);
    }
}
void Quizz::afisare(ostream &out){
    Examen::afisare(out);
    out<<"Numar intrebari: "<<intrebari.size()<<endl;
    for (vector<Intrebare*>::iterator it = intrebari.begin() ; it != intrebari.end(); ++it)
        out<<**it<<endl;
}
void Quizz::afisare_intrebari(ostream &out){
    for (vector<Intrebare*>::iterator it = intrebari.begin() ; it != intrebari.end(); ++it)
        out<<**it<<endl;
}
istream& operator>>(istream& in, Quizz& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Quizz& aux){
    aux.afisare(out);
    return out;
}

class Examen_Final: public Examen{
private:
    double nota_proiect;
    Partial *partial;
    Quizz *quizz;

public:
    Examen_Final(string,double,double,Partial*,Quizz*,int);
    Examen_Final(Examen_Final&);
    virtual ~Examen_Final();
    Examen_Final& operator=(Examen_Final&);
    void citire(istream&);
    void afisare(ostream&);
    friend istream& operator>>(istream&, Examen_Final&);
    friend ostream& operator<<(ostream&, Examen_Final&);

};

Examen_Final::Examen_Final(string denumire="",double nota_scris=0,double nota_proiect=0,Partial *aux=NULL ,Quizz *qz=NULL,int id=-1):Examen(denumire,nota_scris,id){

    try{
        if(nota_proiect<0 || nota_proiect>10) throw nota_proiect;
        this->nota_proiect=nota_proiect;
    }
    catch(double x){
        cout<<"Nota proiect nu poate fi <0 sau >10";
        exit(EXIT_FAILURE);
    }

    if(qz)
        quizz = new Quizz(*qz);
    else
        quizz = new Quizz(denumire,0,id_examen);

    if(aux)
        partial = new Partial(*aux);
    else
        partial = new Partial(denumire,0,0,id_examen);

}
Examen_Final::Examen_Final(Examen_Final &aux):Examen(aux){
    this->nota_proiect = aux.nota_proiect;
    this->partial = new Partial(*aux.partial);
    this->quizz = new Quizz(*aux.quizz);
}
Examen_Final& Examen_Final::operator=(Examen_Final& aux){
    if(this!=&aux){
        Examen::operator=(aux);
        this->nota_proiect = aux.nota_proiect;
        this->partial = new Partial(*aux.partial);
        this->quizz = new Quizz(*aux.quizz);
    }
    return *this;
}
Examen_Final::~Examen_Final(){
    this->nota_proiect = 0;
    delete partial;
    delete quizz;
}
void Examen_Final::citire(istream&in){
    Examen::citire(in);
    cout<<"Nota proiect: ";
    in>>nota_proiect;

    this->partial->set_denumire(this->denumire);

    double nota;
    cout<<"Nota oral: ";
    in>>nota;
    while(nota>10 || nota<0){
        cout<<"Nota nu poate >10 sau <0\nNota oral: ";
        in>>nota;
    }
    this->partial->set_nota_oral(nota);

    cout<<"Nota scris partial: ";
    in>>nota;
    while(nota>10 || nota<0){
        cout<<"Nota nu poate >10 sau <0\nNota oral: ";
        in>>nota;
    }
    this->partial->set_nota_scris(nota);

    cout<<"Nota scris quizz: ";
    in>>nota;
    while(nota>10 || nota<0){
        cout<<"Nota nu poate >10 sau <0\nNota oral: ";
        in>>nota;
    }
    this->quizz->set_nota_scris(nota);

    this->quizz->set_denumire(this->denumire);
    quizz->citire_intrebari(in);
}
void Examen_Final::afisare(ostream&out){
    Examen::afisare(out);
    out<<"Nota proiect: "<<nota_proiect<<endl;
    out<<"Nota scris partial: "<<partial->get_nota_scris()<<endl;
    out<<"Nota oral: "<<partial->get_nota_oral()<<endl;
    out<<"Nota scris quizz: "<<quizz->get_nota_scris()<<endl;
    quizz->afisare_intrebari(out);
    out<<endl;
}
istream& operator>>(istream& in, Examen_Final& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Examen_Final& aux){
    aux.afisare(out);
    return out;
}

template <class t> class CatalogIndividual{
private:
    static int NrMatricol;
    int numar_examene;
    t *examene;
    unordered_map <int, vector<Elev>> my_map;
public:
    CatalogIndividual(t *p=NULL, int numar_examene=0);
    CatalogIndividual(CatalogIndividual&aux);
    ~CatalogIndividual();
    friend istream& operator >>(istream &in, CatalogIndividual <t> &g){
        cout<<"Introduceti numarul de obiecte implicate: ";
        in>>g.numar_examene;
        try{
            g.examene=new t[g.numar_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        cout<<"Introduceti obiectele: \n";
        for(int i=0;i<g.numar_examene;i++)
            in>>g.examene[i];
        cout<<"Map instertion\n";
        cout<<"Numar examene:";
        int input_int_map;
        in>>input_int_map;
        int id_examen;
        vector <Elev> aux;
        for(int i=0;i<input_int_map;i++){
            cout<<"Introduceti ID Examen: ";
            in>>id_examen;
            in>>&aux;
            g.my_map[id_examen]=aux;
        }
        return in;
    }
    friend ostream& operator <<(ostream &out, CatalogIndividual <t> &g){
        out<<NrMatricol<<endl;
        out<<"Numarul de examene: "<<g.numar_examene<<endl;
        for(int i=0;i<g.numar_examene;i++)
            out<<g.examene[i];
        for (auto x : g.my_map)
            cout<<x.first<<" "<<x.second<<endl;
        return out;
    }
    CatalogIndividual<t>& operator+=(t aux){
        NrMatricol++;
        this->numar_examene+=1;
        t *auxx=examene;
        try{
            examene=new t[numar_examene];
        } catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<numar_examene-1;i++){
            examene[i]=auxx[i];
        }
        examene[numar_examene-1]=aux;
        return *this;
    }
    CatalogIndividual<t>& operator+=(unordered_map<int,vector<Elev>> a){
        NrMatricol++;
        try{
            for (auto x : a){
                if(my_map[x.first].empty()){
                    my_map[x.first]=x.second;
                } else {
                    my_map[x.first].insert(end(my_map[x.first]), begin(x.second), end(x.second));
                }
            }
        } catch(bad_alloc){
            cout<<"Allocation Faillure\n"<<endl;
            exit(EXIT_FAILURE);
        }
        return *this;
    }
    static int getSeed(){return NrMatricol;}
};

template<class t>
int CatalogIndividual<t>::NrMatricol=0;

template<class t>
CatalogIndividual<t>::CatalogIndividual(t *p, int numar_examene){
    this->numar_examene=numar_examene;
    examene=new t[numar_examene];
    for(int i=0;i<numar_examene;i++){
        examene[i]=p[i];
    }
}

template<class t>
CatalogIndividual<t>::CatalogIndividual(CatalogIndividual &aux){
    this->numar_examene=aux.numar_examene;
    examene=new t[numar_examene];
    for(int i=0;i<numar_examene;i++){
        examene[i]=aux.examene[i];
    }
    my_map=aux.my_map;
}

template<class t>
CatalogIndividual<t>::~CatalogIndividual(){
    delete [] examene;
    my_map.clear();
}


template <>
class CatalogIndividual <Quizz>{
private:
    static int NrMatricolSeed;
    Quizz* examene;
    int numar_examene;
    unordered_map <int, vector<Elev>> my_map;
public:
    CatalogIndividual(Quizz *p=NULL, int numar_examene=0){
        NrMatricolSeed++;
        this->numar_examene=numar_examene;
        try{
            examene=new Quizz[numar_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<numar_examene;i++)
            examene[i]=p[i];
    }
    CatalogIndividual(CatalogIndividual&aux){
        this->numar_examene=aux.numar_examene;
        try{
            examene=new Quizz[numar_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<numar_examene;i++){
            examene[i]=aux.examene[i];
        }
    }
    ~CatalogIndividual(){
        delete []examene;
        my_map.clear();
    }
    friend istream& operator >>(istream &in, CatalogIndividual <Quizz> &g){
        cout<<"Introduceti numarul de quizz-uri implicate: ";
        in>>g.numar_examene;
        try{
            g.examene=new Quizz[g.numar_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        cout<<"Introduceti obiectele: \n";
        for(int i=0;i<g.numar_examene;i++)
            in>>g.examene[i];
        cout<<"Insertie in map\n";
        cout<<"Numar examene:";
        int input_int_map;
        in>>input_int_map;
        int id_examen;
        vector <Elev> aux;
        for(int i=0;i<input_int_map;i++){
            cout<<"Introduceti ID Examen: ";
            cin>>id_examen;
            cin>>&aux;
            try{
                g.my_map[id_examen]=aux;
            }
            catch(bad_alloc){
                cout<<"Allocation Failure\n";
                exit(EXIT_FAILURE);
            }
        }
        return in;
    }
    friend ostream& operator <<(ostream &out, CatalogIndividual <Quizz> &g){
        out<<"Numarul de examene: "<<g.numar_examene<<endl;
        for(int i=0;i<g.numar_examene;i++)
            out<<g.examene[i];
        for (auto x : g.my_map)
            cout<<x.first<<" "<<x.second<<endl;
        return out;
    }
    CatalogIndividual<Quizz>& operator+=(Quizz aux){
        this->numar_examene+=1;
        Quizz *auxx=examene;
        try{
            examene=new Quizz[numar_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<numar_examene-1;i++){
            examene[i]=auxx[i];
        }
        examene[numar_examene-1]=aux;
        return *this;
    }
    CatalogIndividual<Quizz>& operator+=(unordered_map<int,vector<Elev>> a){
        try{
            for (auto x : a){
                if(my_map[x.first].empty()){
                    my_map[x.first]=x.second;
                } else {
                    my_map[x.first].insert(end(my_map[x.first]), begin(x.second), end(x.second));
                }
            }
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        return *this;
    }
    void afisare(){
        unordered_map<string,int> aux_map;
        for (auto x : my_map){
            for (auto& it : x.second) {
                aux_map[it.getname()]++;
            }
        }
        cout<<"Elevii cu cel putin doua quizz-uri"<<endl;
        for (auto& it : aux_map) {
            if(it.second>=2)
                cout<<it.first<<endl;
        }

    }
    static int getSeed(){return NrMatricolSeed;}
};

int CatalogIndividual<Quizz>::NrMatricolSeed=0;

template <class t>
class Template{
private:

static int NrMatricolSeed;
int nr_matricol;
t *examene;
int nr_examene;
Elev elev;

public:
    Template(t *examene=NULL,int nr_examene=0,Elev *elev=new Elev){
        nr_matricol=NrMatricolSeed;
        NrMatricolSeed=NrMatricolSeed+1;
        this->nr_examene=nr_examene;
        this->elev=*elev;
        try{
            this->examene=new t[nr_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<nr_examene;i++)
            this->examene[i]=examene[i];
    }
    Template(Template<t> &aux){
        this->nr_examene=aux.nr_examene;
        this->elev=aux.elev;
        try{
            this->examene=new t[nr_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<nr_examene;i++)
            this->examene[i]=aux.examene[i];
    }
    ~Template(){
        delete [] examene;
    }
    friend istream& operator >>(istream &in, Template <t> &g){
        cout<<"Introduceti numarul de obiecte: ";
        in>>g.nr_examene;
        in>>g.elev;
        try{
            g.examene=new t[g.nr_examene];
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<g.nr_examene;i++)
            try{
                in>>g.examene[i];
            }
            catch(bad_alloc){
                cout<<"Allocation Failure\n";
                exit(EXIT_FAILURE);
            }
        return in;
    }
    friend ostream& operator <<(ostream &out, Template <t> &g){
        out<<"Nr obiecte: "<<g.nr_examene<<endl;
        out<<"Elev "<<g.elev<<"\nNr. Matricol "<<g.nr_matricol<<endl;
        for(int i=0;i<g.nr_examene;i++)
            out<<g.examene[i]<<endl;
        return out;
    }
    static int getSeed(){return NrMatricolSeed;}
};
template <class t>
int Template<t>::NrMatricolSeed = 0;



template <class t> class Participant{
private:
    Elev elev;
    t *examen;
public:
    Participant(Elev *elev = NULL,t *examen = NULL){
        try{
            if(elev != NULL){
                this->elev=*elev;
            } else this->elev=*(new Elev);
            if(examen != NULL){
                this->examen=new t(*examen);
            } else {
                this->examen=new t;
            }
        }
        catch(bad_alloc){cout<<"Allocation Faillure";exit(EXIT_FAILURE);}
    }
    Participant(const Participant&aux){
        elev=aux.elev;
        try{
            examen=new t(*aux.examen);
        }
        catch(bad_alloc){
            cout<<"Allocation Faillure";
            exit(EXIT_FAILURE);
        }
    }
    ~Participant(){
    }
    Participant& operator=(const Participant& aux){
        elev=aux.elev;
        examen=new t(*aux.examen);
        return *this;
    }
    friend istream& operator>>(istream&in,Participant&aux){
        cout<<"\nCitire Participant\n";
        in>>aux.elev;
        in>>*aux.examen;
        return in;
    }
    friend ostream& operator<<(ostream&out,Participant&aux){
        out<<aux.elev<<endl;
        out<<*aux.examen<<endl;
        return out;
    }
    string getname(){return elev.getname();}
    t& getexamen(){return *examen;}
};

template <class t> class CatalogIndividual2{
private:
    unordered_map<int,vector<Participant<t>>> my_map;
    int NrExamene;
public:
    CatalogIndividual2( unordered_map<int,vector<Participant<t>>> *my_map = NULL,int NrExamene=0){
        if(my_map == NULL){
            this->NrExamene=0;
        } else {
            this->my_map=*my_map;
            this->NrExamene=NrExamene;
        }
    }
    CatalogIndividual2(CatalogIndividual2 &aux){
        NrExamene=aux.NrExamene;
        my_map=aux.my_map;
    }
    ~CatalogIndividual2(){
        NrExamene=0;
        my_map.clear();
    }
    CatalogIndividual2& operator=(CatalogIndividual2& aux){
        if(aux!=this){
            NrExamene=aux.NrExamene;
            my_map=aux.my_map;
        }
        return *this;
    }
    friend istream& operator>>(istream&in,CatalogIndividual2& aux){
        cout<<"Introduceti numarul de examene: ";
        in>>aux.NrExamene;
        vector<Participant<t>> new_vector;
        for(int i=0;i<aux.NrExamene;i++){
            cin>>&new_vector;
            try{
                aux.my_map[i]=new_vector;
            } catch(bad_alloc){
                cout<<"Allocation Failure\n";
                exit(EXIT_FAILURE);
            }
        }
        return in;
    }
    friend ostream& operator<<(ostream&out,CatalogIndividual2& aux){
        out<<"Numarul de examene: "<<aux.NrExamene<<endl;
        for(int i=0;i<aux.NrExamene;i++){
            out<<"Pentru examenul cu ID: "<<i<<endl;
            out<<aux.my_map[i];
        }
        return out;
    }
    CatalogIndividual2& operator+=(vector<Participant<t>> &aux){
        try{
            my_map[NrExamene]=aux;
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
        NrExamene+=1;
        return *this;
    }
};

template <>
class CatalogIndividual2<Quizz>{
private:
    unordered_map<int,vector<Participant<Quizz>>> my_map;
    int NrExamene;
public:
    CatalogIndividual2( unordered_map<int,vector<Participant<Quizz>>> *my_map = NULL,int NrExamene=0){
        if(my_map == NULL){
            this->NrExamene=0;
        } else {
            this->my_map=*my_map;
            this->NrExamene=NrExamene;
        }
    }
    CatalogIndividual2(CatalogIndividual2 &aux){
        NrExamene=aux.NrExamene;
        try{
            my_map=aux.my_map;
        }
        catch(bad_alloc){
            cout<<"Allocation Failure\n";
            exit(EXIT_FAILURE);
        }
    }
    ~CatalogIndividual2(){
        NrExamene=0;
        my_map.clear();
    }
    CatalogIndividual2& operator=(CatalogIndividual2& aux){
        NrExamene=aux.NrExamene;
        my_map=aux.my_map;
        return *this;
    }
    friend istream& operator>>(istream&in,CatalogIndividual2& aux){
        cout<<"Introduceti numarul de examene: ";
        in>>aux.NrExamene;
        vector<Participant<Quizz>> new_vector;
        for(int i=0;i<aux.NrExamene;i++){
            cin>>&new_vector;
            aux.my_map[i]=new_vector;
        }
        return in;
    }
    friend ostream& operator<<(ostream&out,CatalogIndividual2& aux){
        out<<"Numarul de examene: "<<aux.NrExamene<<endl;
        for(int i=0;i<aux.NrExamene;i++){
            out<<"Pentru examenul cu ID: "<<i<<endl;
            out<<aux.my_map[i];
        }
        return out;
    }
    CatalogIndividual2& operator+=(vector<Participant<Quizz>> &aux){
        my_map[NrExamene]=aux;
        NrExamene+=1;
        return *this;
    }
    void afisare(){
        unordered_map<string,int> aux_map;
        for (auto x : my_map){
            for (auto& it : x.second) {
                try{
                    aux_map[it.getname()]++;
                }
                catch(bad_alloc){
                    cout<<"Allocation Failure\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        cout<<"Elevii cu cel putin doua quizz-uri"<<endl;
        for (auto& it : aux_map) {
            if(it.second>=2)
                cout<<it.first<<endl;
        }
    }
};

template<class t>
class Baza{
protected:
    t valoare;
public:
    Baza(t aux){valoare=aux;}
    virtual t get_valoare(){return valoare;}
};

template<class t>
class Derivata:public Baza<t>{
t as;
public:
    Derivata(t aux,t aux2):Baza<t>(aux){as=aux2;}
    int get_valoare(){return this->valoare*this->valoare;}
    int get_three(){return as;}
};

template<typename t>
void template_afisare(t &x){
    cout<<x<<endl;
}

int main(){

    Singleton *menu = Singleton::getInstance();
    {
        menu->insertData("      Bahrim Dragos -- Grupa 254 -- Proiect 3 -- Tema 3 ");
        menu->insertData("=============================================================");
        menu->insertData("");
        menu->insertData("  Meniu");
        menu->insertData("");
        menu->insertData("1. Constructori clase cu parametri impliciti");
        menu->insertData("2. Constructori clase cu parametri parametrizati");
        menu->insertData("3. Constructor de copiere");
        menu->insertData("4. Destructor");
        menu->insertData("5. Operator =");
        menu->insertData("6. Operator >> si <<");
        menu->insertData("7. Operator clase derivate >> si <<");
        menu->insertData("8. CatalogIndividual Examen + supraincarcare +=");
        menu->insertData("9. CatalogIndividual Partial + supraincarcare +=");
        menu->insertData("10. CatalogIndividual Examen_Final + supraincaracre +=");
        menu->insertData("11. CatalogIndividual Quizz + supraincaracre +=");
        menu->insertData("12. CatalogIndividual Quizz afisare persoane cu cel putin doua quizz-uri");
        menu->insertData("13. Creat clasa sablon Template");
        menu->insertData("14. Utilizare STL: stack");
        menu->insertData("15. Utilizare functii statice si valori statice");
        menu->insertData("16. Contor +1 Examene");
        menu->insertData("17. Contor +1 CatalogIndividual<Examen>");
        menu->insertData("18. Contor +1 Template<Examen>");
        menu->insertData("19. Contor +1 Template<int>");
        menu->insertData("20. Contor +1 Template<string>");
        menu->insertData("21. Variabile constante");
        menu->insertData("22. RTTI");
        menu->insertData("23. Citire n obiecte");
        menu->insertData("24. Afisare n obiecte");
        menu->insertData("25. Creare sablon CatalogIndividual2, o forma diferita a clasei CatalogIndividual");
        menu->insertData("26. Tratarea exceptiilor cu try catch");
        menu->insertData("27. Simulare bad_alloc");

    }

    int optiune;
    Examen **examene;
    int nr_examene=0;
    while(true){
        menu->viewData();
        cin>>optiune;
        switch(optiune){
        case 1:{
            cout<<endl<<"Examen\n";
            Examen *ex = new Examen;
            cout<<*ex;
            cout<<endl<<"Partial\n";
            ex= new Partial;
            cout<<*ex;
            cout<<endl<<"Quizz\n";
            ex= new Quizz;
            cout<<*ex;
            cout<<endl<<"Examen_Final\n";
            ex= new Examen_Final;
            cout<<*ex;
            break;
        }
        case 2:{
            cout<<endl<<"Examen\n";
            Examen *ex = new Examen("Examen Nou",5,999);
            cout<<*ex;
            cout<<endl<<"Partial\n";
            ex= new Partial("Partial Nou",5,6,998);
            Examen *ex1=ex;
            cout<<*ex;
            cout<<endl<<"Quizz\n";
            ex= new Quizz("Quizz Nou",5,997);
            cout<<*ex;
            cout<<endl<<"Examen_Final\n";
            ex= new Examen_Final("Examen_Final Nou",10,3,dynamic_cast<Partial*>(ex1),dynamic_cast<Quizz*>(ex),996);
            cout<<*ex;
            break;
        }
        case 3:{
            Examen examen;
            Partial part;
            Quizz qz;
            Examen_Final ef;
            cout<<endl<<"Examen\n";
            Examen *ex = new Examen(examen);
            cout<<*ex;
            cout<<endl<<"Partial\n";
            ex= new Partial(part);
            cout<<*ex;
            cout<<endl<<"Quizz\n";
            ex= new Quizz(qz);
            cout<<*ex;
            cout<<endl<<"Examen_Final\n";
            ex= new Examen_Final(ef);
            cout<<*ex;
            break;
        }
        case 4:{
            cout<<"Destrctor virtual in cazul unei derivari prin pointer la clasa de baza";

            Examen ex;
            Partial part;
            Quizz qz;
            Examen_Final ef;
            break;
        }
        case 5:{
            Examen ex("Examen Nou",5,999);
            Partial part("Partial Nou",5,6,998);
            Quizz qz("Quizz Nou",5,997);
            Examen_Final ef("Examen_Final Nou",10,3,&part,&qz,996);

            Examen ex1;
            Partial part1;
            Quizz qz1;
            Examen_Final ef1;

            cout<<"\n\nInitial\n\n";
            cout<<ex1<<part1<<qz1<<ef1;

            ex1=ex;
            part1=part;
            qz1=qz;
            ef1=ef;

            cout<<"Dupa\n\n\n"<<ex1<<part1<<qz1<<ef1;
            break;
        }
        case 6:{
            Examen ex1;
            cin>>ex1;
            cout<<ex1;
            break;
        }
        case 7:{
            Examen *ex1;
            ex1=new Partial;
            cin>>*ex1;
            cout<<*ex1;
            ex1=new Quizz;
            cin>>*ex1;
            cout<<*ex1;
            ex1=new Examen_Final;
            cin>>*ex1;
            cout<<*ex1;
            break;
        }
        case 8:{
            CatalogIndividual<Examen> ci1;
            cout<<ci1;
            cin>>ci1;
            cout<<ci1;

            Examen ex1;
            cin>>ex1;
            ci1+=ex1;
            cout<<ci1;

            unordered_map <int,vector<Elev>> new_map;
            cout<<"Numar examene:";
            int input_int_map;
            cin>>input_int_map;
            int id_examen;
            vector <Elev> aux;
            for(int i=0;i<input_int_map;i++){
                cout<<"Introduceti ID Examen: ";
                cin>>id_examen;
                cin>>&aux;
                new_map[id_examen]=aux;
            }

            ci1+=new_map;
            cout<<ci1;
            break;

        }
        case 9:{
            CatalogIndividual<Partial> ci1;
            cout<<ci1;
            cin>>ci1;
            cout<<ci1;

            Partial ex1;
            cin>>ex1;
            ci1+=ex1;
            cout<<ci1;

            unordered_map <int,vector<Elev>> new_map;
            cout<<"Numar examene:";
            int input_int_map;
            cin>>input_int_map;
            int id_examen;
            vector <Elev> aux;
            for(int i=0;i<input_int_map;i++){
                cout<<"Introduceti ID Examen: ";
                cin>>id_examen;
                cin>>&aux;
                new_map[id_examen]=aux;
            }

            ci1+=new_map;
            cout<<ci1;
            break;

        }
        case 10:{
            CatalogIndividual<Examen_Final> ci1;
            cout<<ci1;
            cin>>ci1;
            cout<<ci1;

            Examen_Final ex1;
            cin>>ex1;
            ci1+=ex1;
            cout<<ci1;

            unordered_map <int,vector<Elev>> new_map;
            cout<<"Numar examene:";
            int input_int_map;
            cin>>input_int_map;
            int id_examen;
            vector <Elev> aux;
            for(int i=0;i<input_int_map;i++){
                cout<<"Introduceti ID Examen: ";
                cin>>id_examen;
                cin>>&aux;
                new_map[id_examen]=aux;
            }
            ci1+=new_map;
            cout<<ci1;
            break;
        }
        case 11:{
            CatalogIndividual<Quizz> ci1;
            cout<<ci1;
            cin>>ci1;
            cout<<ci1;

            Quizz ex1;
            cin>>ex1;
            ci1+=ex1;
            cout<<ci1;

            unordered_map <int,vector<Elev>> new_map;
            cout<<"Numar examene:";
            int input_int_map;
            cin>>input_int_map;
            int id_examen;
            vector <Elev> aux;
            for(int i=0;i<input_int_map;i++){
                cout<<"Introduceti ID Examen: ";
                cin>>id_examen;
                cin>>&aux;
                new_map[id_examen]=aux;
            }
            ci1+=new_map;
            cout<<ci1;
            break;
        }
        case 12:{
            CatalogIndividual<Quizz> ci1;
            Elev e1("elev1_nume","elev1_prenume");
            Elev e2("elev2_nume","elev2_prenume");
            Elev e3("elev3_nume","elev3_prenume");

            vector<Elev> v1;
            v1.push_back(e1);
            v1.push_back(e2);
            v1.push_back(e3);

            vector<Elev> v2;
            v2.push_back(e1);
            v2.push_back(e2);

            vector<Elev> v3;
            v3.push_back(e1);

            unordered_map <int,vector<Elev>> new_map;
            new_map[1]=v1;
            new_map[2]=v2;
            new_map[3]=v3;

            ci1+=new_map;
            cout<<ci1;

            ci1.afisare();
            break;
        }
        case 13:{

            cout<<"In proiect am folosit template-uri pentru CatalogIndividual si pentru supraincarcarea citirii si afisarii unui vector din STL.\n";
            cout<<"Am mai creat o clasa template pentru punctul de utilizarea sabloanelor\n";
            cout<<"E mai apropiata de ce credeam ca este clasa CatalogIndividual, studentul si examenele date de acesta.\n";
            cout<<"Pentru ca folosim orice tip de data putem sa o folosim pentru orice alt tip deci nu numai pentru examene\n";
            Template<Examen> CI;
            cin>>CI;
            cout<<CI<<endl<<endl;
            cout<<"Folosesc acelasi template pentru int\n";
            Template<int> CI2;
            cin>>CI2;
            cout<<CI2<<endl<<endl;

            cout<<"Folosesc acelasi tempplate pentru string(constructor neparametrizat)\n";
            Template<string>CI3;
            cin>>CI3;
            cout<<CI3<<endl<<endl;

            cout<<"Am folosit o functie sablon ce apeleaza cout<< pentru variabila primita ca argument indiferent de tipul ei\n";
            cout<<"Pentru Template CI3\n";
            template_afisare(CI3);
            cout<<"Pentru un string\n";
            string str="new_string";
            template_afisare(str);


            cout<<"Citirea si afisare unui vector din STL cu cin si cout\n";
            cout<<"Pentru int\n";
            vector<int> intvector;
            cin>>&intvector;
            cout<<intvector;

            cout<<"Pentru string\n";
            vector<string> intvector2;
            cin>>&intvector2;
            cout<<intvector2;
            break;
        }
        case 14:{
            cout<<"Din STL am folosit vector<> pentru diferite structuri in Quizz, Intrebare si Elev, si am folosit unordered_map pentru structura de date din enunt\n";
            cout<<"Am mai folosit si stack:\nAm implementat si try, catch\n";
            system("pause");
            system("cls");
            stack<int> new_stack;
            int new_option=0;
            while(true){
                try{
                    cout<<"1. Push\n";
                    cout<<"2. Pop\n";
                    cout<<"3. Top\n";
                    cout<<"0. Iesire\n";
                    cout<<"Optiune: ";
                    cin>>new_option;
                    if (new_option==1){
                        int aux;
                        cout<<"Numar: ";
                        cin>>aux;
                        new_stack.push(aux);
                    }
                    if (new_option==2){
                        if(!new_stack.empty())
                            new_stack.pop();
                        else cout<<"Gol\n";

                    }
                    if (new_option==3){
                        if(!new_stack.empty())
                            cout<<new_stack.top()<<endl;
                        else cout<<"Gol\n";
                    }
                    if (new_option==0){
                        break;
                    }
                    if (new_option < 0 || new_option >3)
                        throw new_option;
                }
                catch (int x){
                    cout<<"Input invalid, a fost introdus "<<x<<"\n";
                }
                system("pause");
                system("cls");

            }
            break;
        }
        case 15:{
            cout<<"Exista un Seed pentru Examen in clasa de baza Examen si NrMatricol in clasele template CatalogIndividual si Template\n";
            cout<<"Daca avem doua clase template Template<int> Template<string> ele au seed-uri diferite ( pentru ca sunt clase diferite)\n";
            cout<<"Seed examene: "<<Examen::getSeed()<<endl;
            cout<<"Seed CatalogIndividual<Examen>: "<<CatalogIndividual<Examen>::getSeed()<<endl;
            cout<<"Seed Template<Examen>: "<<Template<Examen>::getSeed()<<endl;
            cout<<"Seed Template<int>: "<<Template<int>::getSeed()<<endl;
            cout<<"Seed Template<string>: "<<Template<string>::getSeed()<<endl;
            break;
        }
        case 16:{
            Examen *ex = new Examen;
            ex=ex;
            cout<<"+1\n";
            break;
        }
        case 17:{
            CatalogIndividual<Examen> *ex = new CatalogIndividual<Examen>;
            ex=ex; // Scap de warning;
            cout<<"+1\n";
            break;
        }
        case 18:{
            Template<Examen> *ex = new Template<Examen>;
            ex=ex; // Scap de warning;
            cout<<"+1\n";
            break;
        }
        case 19:{
            Template<int> *ex = new Template<int>;
            ex=ex; // Scap de warning;
            cout<<"+1\n";
            break;
        }
        case 20:{
            Template<string> *ex = new Template<string>;
            ex=ex; // Scap de warning;
            cout<<"+1\n";
            break;
        }
        case 21:{
            const Examen ex;
            cout<<ex.getNotaScris()<<endl; // Nu da eroare
            //cout<<ex.get_nota_scris(); Eroare - obiect constant are nevoie de functii ce promit ca obiectul nu se schimba;

            Examen ex2;
            cout<<ex2.getNotaScris()<<endl; // Functiile constante pot fi apelate si pentru obiecte normale
            cout<<ex2.get_nota_scris()<<endl;

            // schimba valoarea
            ex2.set_nota_scris(54);
            cout<<ex2.getNotaScris()<<endl;
            cout<<ex2.get_nota_scris()<<endl;
            break;
        }
        case 22:{
            cout<<"Declar in interiorul unui pointer la clasa de baza (Examen) o derivata (Partial)\n";
            cout<<"Partial are o metoda get_nota_oral care nu este valabila in clas de baza\n";
            Examen *ex = new Partial;
            // cout<<ex->get_nota_partial(); Eroare pentru ca clasa de baza nu stie de metodele din derivata
            Partial *part = dynamic_cast<Partial*>(ex); // Merge pentru ca ce se retine in pointer-ul examen coincide cu ce e in memorie
            //Partial *part1 = (Partial*)ex;
            Examen *ex2=dynamic_cast<Examen*>(ex); // Se face upcasting deci merge
            Examen ex3=*(dynamic_cast<Examen*>(ex)); // Upcasting si dupa se copiaza obiectul din dreapta in ex3
            Quizz *qz=dynamic_cast<Quizz*>(ex); // nu merge pentru ca nu sunt compatibile si returneaza pointer nul
            if(part) cout<<*part<<endl;
            if(ex2) cout<<*ex2<<endl;
            if(!qz) cout<<"Nu a mers"<<endl;
            cout<<ex3<<endl;
            cout<<"Downcasting\n";
            Examen aux;
            Partial *aux1=(Partial*)&aux;
            //Nu merge pentru ca in memorie avem Examen si nu putem face dynamic_cast sa avem examen primesc warning: |1193|warning: dynamic_cast of 'Examen aux' to 'class Partial*' can never succeed|
            //Partial *aux3=dynamic_cast<Partial*>(&aux);
            Partial *aux2=static_cast<Partial*>(&aux);
            cout<<aux2->get_nota_oral()<<endl; // primesc ceva random, nu exista verificari ca la dynamic cast
            cout<<endl<<*aux1<<endl;
            cout<<"Primim ceva random pentru ca nu este alocat:"<<aux1->get_nota_oral()<<endl; // la fel cu static_cast
            cout<<"Conversie de tip a unui Examen la integer ( returneaza id_examen ) "<<int(*ex2)<<endl<<endl<<endl;
            Template<string>asd;
            cout<<asd;


            Template<Examen> newtemplate;
            cout<<typeid(newtemplate).name()<<endl;
            cout<<typeid(*ex).name()<<endl;
            cout<<typeid(*part).name()<<endl;
            cout<<typeid(*ex2).name()<<endl;
            cout<<typeid(ex3).name()<<endl;
            try{
                cout<<typeid(*qz).name()<<endl;
            } catch (bad_typeid) {
                cout<<"Se intampla pentru ca in qz este NULL"<<endl;
            }

            Baza<int> b1(15);
            Derivata<int> d1(20,25);

            // Pointer la baza, memoreaza adresa unui obiect baza
            Baza<int> *p1 = &b1;
            cout<<p1->get_valoare()<<endl;

            // Pointer la baza, memoreaza adresa unui obiect derivat
            p1 = &d1;
            cout<<p1->get_valoare()<<endl;

            // Pointer la baza, memoreaza adresa unui obiect baza, merge dynamic_cast
            p1 = dynamic_cast<Baza<int>*>(&b1);
            cout<<p1->get_valoare()<<endl;

            // Pointer la baza, memoreaza adresa unui obiect derivat, merge dynamic_cast (upcast)
            p1 = dynamic_cast<Baza<int>*>(&d1);
            cout<<p1->get_valoare()<<endl;
            // cout<<p1->get_three()<<endl; nu avem  acces

            Derivata<int> *pd1;
            // Pointer la derivata, memoreaza adresa unui obiect derivat din pointer-ul clasa de baza ce are un obiect baza de data in memorie
            // avem acces la metodele din derivata
            pd1 = dynamic_cast<Derivata<int>*>(p1);
            cout<<pd1->get_three()<<endl;

            // Downcast
            try{
                pd1 = dynamic_cast<Derivata<int>*>(&b1);
                if(!pd1)cout<<"Nu a mers pentru ca in memorie avem o referinta la baza ce vrem sa o interpretam ca o derivata"<<endl;
            } catch(bad_cast){
                cout<<"\nbad_cast\n";
            }
            // afiseaza lucruri la intamplare, nu are verificari ca dynamic cast
            pd1 = static_cast<Derivata<int>*>(&b1);
            cout<<pd1->get_three()<<endl;


            Baza<string> b2("asd");
            // Baza<string> *p2 = dynamic_cast<Baza<int>*>(&b2); Intre clasa template diferite nu este compatibil.

            break;
        }
        case 23:{
            if(nr_examene!=0){
                for(int i=0;i<nr_examene;i++)
                    delete examene[i];
                delete [] examene;
            }
            try{
                cout<<"\nIntroduceti numarul de obiecte: ";
                cin>>nr_examene;
                if(nr_examene<=0) throw 10;
            }
            catch(int x){
                cout<<"Nr_examene nu poate fi negativ\n";
            }

            examene=new Examen*[nr_examene];
            for(int i=0;i<nr_examene;){
                string s;
                cout<<"Introduceti tipul obiectului: ";
                cin>>s;
                try{
                    if(s=="Examen"){
                        examene[i]=new Examen;
                        cin>>*examene[i];
                        i++;
                    } else if(s=="Partial"){
                        examene[i]=new Partial;
                        cin>>*examene[i];
                        i++;
                    } else if(s=="Quizz"){
                        examene[i]=new Quizz;
                        cin>>*examene[i];
                        i++;
                    } else if(s=="ExamenFinal"){
                        examene[i]=new Examen_Final;
                        cin>>*examene[i];
                        i++;
                    } else{
                        throw 20;
                    }
                }
                catch(bad_alloc var){
                    cout << "Allocation Failure\n";
                    exit(EXIT_FAILURE);
                }
                catch(int s){
                    cout<<"Incercati Examen || Partial || Quizz || ExamenFinal \n";
                }

            }
            break;
        }
        case 24:{
            int N_examen,N_part,N_quizz,N_examen_final;
            N_examen=N_part=N_quizz=N_examen_final=0;
            if (nr_examene>0){
                for(int i=0;i<nr_examene;i++){
                    if (typeid(*examene[i])==typeid(Examen)){
                        N_examen++;
                    }
                    if (typeid(*examene[i])==typeid(Partial)){
                        N_part++;
                    }
                    if (typeid(*examene[i])==typeid(Quizz)){
                        N_quizz++;
                    }
                    if (typeid(*examene[i])==typeid(Examen_Final)){
                        N_examen_final++;
                    }
                    cout<<*examene[i];
                }
                cout<<"\n\nNumarul de examene: "<<N_examen<<"\n";
                cout<<"Numarul de partiale: "<<N_part<<"\n";
                cout<<"Numarul de quizz-uri: "<<N_quizz<<"\n";
                cout<<"Numarul de examene_final: "<<N_examen_final<<"\n";
            }
            else{
                cout<<"Nu s-au citit obiecte.\n";
            }
        break;
        }
        case 25:{
            CatalogIndividual2<Examen> asd;
            cout<<asd;
            cin>>asd;
            cout<<asd;
            vector<Participant<Examen>> new_vector;
            cin>>&new_vector;
            asd+=new_vector;
            cout<<asd;
            break;
        }
        case 26:{
            cout<<"In proiect am tratat cateva erori cu try-catch.\n";
            cout<<"- bad_alloc -> alocare cu new, cand folosesc .push_back, cand adaug un element in unordered_map\n";
            cout<<"- bad_typeid -> cand folosim typeid pe NULL\n";
            cout<<"- bad_cast -> cand dynamic_cast la referinta da fail la run time\n";
            cout<<"- erori custom: cand nota este <0 || >10 ( in constructor, la citire am preferat sa inlocuiesc cu un while() care citeste pana cand primeste un parametru bun )\n\n";

            Examen *ex = new Partial;
            Quizz *qz=dynamic_cast<Quizz*>(ex); // nu merge pentru ca nu sunt compatibile si returneaza pointer nul
            try{
                cout<<typeid(*qz).name()<<endl;
            } catch (bad_typeid) {
                cout<<"bad_typeid"<<endl;
            }

            double nota;
            cout<<"nota= ";
            cin>>nota;
            try{
                if(nota>10 || nota<0) throw nota;
            }
            catch(double x){
                cout<<"Nota este incorecta\n";
            }

            while(nota<0 || nota>10){
                cout<<"nota= ";
                cin>>nota;
            }

            Examen b;
            try{
                Template<int> &f = dynamic_cast<Template<int>&>(b);
            }
            catch(bad_cast){
                cout<<"bad cast\n";
            }
            break;
        }
        case 27:{
            int a;
            vector<int>aux;
            while(true){
                try{
                    aux.push_back(a);
                }
                catch(bad_alloc){
                    cout<<"Allocation Failure\n";
                    exit(EXIT_FAILURE);
                }
            }
            break;

        }
        default: {
            cout<<"\nNumar incorect.\n";
            break;
        }
        case 0: exit(0);
        }
        cout<<endl<<endl;
        system("pause");
        system("cls");
    }

}
