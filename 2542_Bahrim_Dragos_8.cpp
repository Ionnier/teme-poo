#include <iostream>
#include <windows.h>
using namespace std;

class Contract{
private:
static int counter;
protected:
    int nrContract;
    int an;
    string beneficiar;
    string furnizor;
    double valoare;

public:
    Contract(int,int,string,string,double);
    Contract(Contract&);
    virtual ~Contract();
    Contract& operator=(Contract&);
    virtual void citire(istream&);  ///citire virtuala - late binding
    virtual void afisare(ostream&); ///afisare virtuala - late binding
    friend istream& operator>>(istream&, Contract&);
    friend ostream& operator<<(ostream&, Contract&);
    void addValoare(double);
    void functie1();
    virtual void functie2();
    static int getCounter(){return counter;};
};
int Contract::counter=0;
Contract::Contract(int nrContract=0,int an=0,string beneficiar="",string furnizor="",double valoare=1){
    counter++;
    this->nrContract=nrContract;
    this->an=an;
    this->beneficiar=beneficiar;
    this->furnizor=furnizor;
    this->valoare=valoare;
}
Contract::~Contract(){
    counter--;
    nrContract=0;
    an=0;
    beneficiar="";
    furnizor="";
    valoare=0;
}
Contract::Contract(Contract& aux){
    this->nrContract=aux.nrContract;
    this->an=aux.an;
    this->beneficiar=aux.beneficiar;
    this->furnizor=aux.furnizor;
    this->valoare=aux.valoare;
}
void Contract::citire(istream &in){
    cout<<"\nIntroducet numarul contractului: ";
    in>>nrContract;
    cout<<"Introduceti anul: ";
    in>>an;
    cout<<"Introduceti beneficiarul: ";
    cin.get();
    cin>>beneficiar;
    cout<<"Introduceti furnizorul: ";
    in>>furnizor;
    cout<<"Introduceti valoarea: ";
    in>>valoare;
}
void Contract::afisare(ostream &out){
    out<<"\nNr. contractului: "<<nrContract<<endl;
    out<<"An: "<<an<<endl;
    out<<"Beneficiar: "<<beneficiar<<endl;
    out<<"Furnizorul: "<<furnizor<<endl;
    out<<"Valoare: "<<valoare<<endl;
}
istream& operator>>(istream& in, Contract& contract){
    contract.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Contract& contract){
    contract.afisare(out);
    return out;
}
Contract& Contract::operator=(Contract& aux){
    this->nrContract=aux.nrContract;
    this->an=aux.an;
    this->beneficiar=aux.beneficiar;
    this->furnizor=aux.furnizor;
    this->valoare=aux.valoare;
    return *this;
}
void Contract::addValoare(double bonus=500){
    this->valoare+=bonus;
}
void Contract::functie1(){
    cout<<"Functie 1 in Baza\n";
}
void Contract::functie2(){
    cout<<"Functie 2 in Baza\n";
}
class ContractInchiriere:public Contract{
private:
    int perioada;
public:
    ContractInchiriere(int,int,string,string,double,int);
    ContractInchiriere(ContractInchiriere&);
    ~ContractInchiriere();
    void citire(istream&);
    void afisare(ostream&);
    void decreaseValoare(int);
    ContractInchiriere& operator=(ContractInchiriere&);
    friend istream& operator>>(istream&, ContractInchiriere&);
    friend ostream& operator<<(ostream&, ContractInchiriere&);
    double getValoareTotala();
    void functie1();
    void functie2();
    void functie3();
};

ContractInchiriere::ContractInchiriere(int nrContract=0,int an=0,string beneficiar="",string furnizor="",double valoare=1,int perioada=0):Contract(nrContract,an,beneficiar,furnizor,valoare){
    this->perioada=perioada;
}
ContractInchiriere::ContractInchiriere(ContractInchiriere&aux):Contract(aux){
    this->perioada=aux.perioada;
}
ContractInchiriere::~ContractInchiriere(){
    perioada=0;
}
void ContractInchiriere::citire(istream &in){
    Contract::citire(in);
    cout<<"Introduceti numarul de luni: ";
    in>>perioada;
}
void ContractInchiriere::afisare(ostream &out){
    out<<"\n";
    Contract::afisare(out);
    out<<"Numarul de luni: "<<perioada<<"\n\n";
}
ContractInchiriere& ContractInchiriere::operator=(ContractInchiriere& aux){
    this->nrContract=aux.nrContract;
    this->an=aux.an;
    this->beneficiar=aux.beneficiar;
    this->furnizor=aux.furnizor;
    this->valoare=aux.valoare;
    this->perioada=aux.perioada;
    return *this;
}
istream& operator>>(istream& in, ContractInchiriere& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, ContractInchiriere& aux){
    aux.afisare(out);
    return out;
}
void ContractInchiriere::decreaseValoare(int valoare){
    this->valoare-=valoare;
}
void ContractInchiriere::functie1(){
    cout<<"Functie 1 in Derivata\n";
}
void ContractInchiriere::functie2(){
    cout<<"Functie 2 in Derivata\n";
}
void ContractInchiriere::functie3(){
    cout<<"Functie 3 in Derivata\n";
}
double ContractInchiriere::getValoareTotala(){
    return valoare*( (double) perioada);
}



class Dosar{
private:
    ContractInchiriere **contracte;
    int numar_dosare;
public:
    Dosar(int);
    Dosar(Dosar&);
    ~Dosar();
    Dosar& operator=(Dosar&);
    void citire(istream&);  ///citire virtuala - late binding
    void afisare(ostream&); ///afisare virtuala - late binding
    friend istream& operator>>(istream&, Dosar&);
    friend ostream& operator<<(ostream&, Dosar&);
    double getValoareTotala();
    void showValoareTotala(ostream&);
};
Dosar::Dosar(int nr=0){
    numar_dosare=nr;
    if(nr!=0){
        contracte=new ContractInchiriere*[nr];
        for(int i=0;i<nr;i++)
            contracte[i]=new ContractInchiriere;
    }
}
Dosar::~Dosar(){
    for(int i=0;i<numar_dosare;i++)
         delete contracte[i];
    delete[] contracte;
    numar_dosare=0;
}
Dosar::Dosar(Dosar &aux){
    this->numar_dosare=aux.numar_dosare;
    if(numar_dosare!=0){
        contracte=new ContractInchiriere*[numar_dosare];
        for(int i=0;i<numar_dosare;i++)
            contracte[i]=new ContractInchiriere(*aux.contracte[i]);
    }
}
Dosar& Dosar::operator=(Dosar&aux){
    this->~Dosar();
    this->numar_dosare=aux.numar_dosare;
    if(numar_dosare!=0){
        this->contracte=new ContractInchiriere*[numar_dosare];
        for(int i=0;i<numar_dosare;i++)
            this->contracte[i]=new ContractInchiriere(*aux.contracte[i]);
    }
    return *this;
}
void Dosar::citire(istream &in){
    cout<<"\nIntroduceti numarul de contracte: ";
    in>>numar_dosare;
    if(numar_dosare!=0){
        contracte=new ContractInchiriere*[numar_dosare];
        for(int i=0;i<numar_dosare;i++){
            contracte[i]=new ContractInchiriere;
            in>>*(contracte[i]);
        }
    }
}
void Dosar::afisare(ostream &out){
    out<<"\n\nIn dosar sunt "<<this->numar_dosare<<" contracte.\n";
    if(this->numar_dosare!=0)
        for(int i=0;i<numar_dosare;i++)
            out<<*contracte[i];
}
istream& operator>>(istream& in, Dosar& aux){
    aux.citire(in);
    return in;
}
ostream& operator<<(ostream& out, Dosar& aux){
    aux.afisare(out);
    return out;
}
double Dosar::getValoareTotala(){
    double suma=0;
    for(int i=0;i<numar_dosare;i++)
        suma+=(*contracte[i]).getValoareTotala();
    return suma;
}
void Dosar::showValoareTotala(ostream &out){
    out<<"\n\nIn dosar sunt "<<this->numar_dosare<<" contracte.\n";
    if(this->numar_dosare!=0)
        for(int i=0;i<numar_dosare;i++){
            out<<*contracte[i];
            out<<"Valoare totala contract: "<<contracte[i]->getValoareTotala()<<endl;
        }
}
int main(){
    string A[]={"Bahrim Dragos Grupa 254 - Proiect 2 - Tema 8. Sa se defineasca clasa Contract:\n","1. Constructor initializare al fiecarei clase","2. Constructor initializare parametrizat al fiecarei clase","3. Constructor de copiere al fiecarei clase","4. Destructor","5. Operator =","6. Operator >> si <<","7. Upcast","8. Downcast","9. Functii virtuale","10. Contorizator al numarului de contracte prin variabila statica","11. Adauga contract pentru a creste contorul","12. Citire n obiecte","13. Afisare n obiecte","14. Citire dosar, rezolvare cerinta"};
    int n;
    Contract **contracte;
    int nr_contracte=0;
    while(true){
        for(unsigned int i=0;i<sizeof(A)/sizeof(string);i++)
            cout<<A[i]<<endl;
        cout<<"\nVarianta: ";
        cin>>n;
        switch(n){
        case 1:{
            Contract *aux = new Contract;
            cout<<*aux;
            ContractInchiriere *aux1 = new ContractInchiriere;
            Dosar *aux2 = new Dosar;
            cout<<*aux1;
            cout<<*aux2;
            delete aux;
            delete aux1;
            delete aux2;
            break;
        }
        case 2:{
            int nrContract=0,an=0;
            string beneficiar="",furnizor="";
            double valoare=1;
            int perioada=0;
            cout<<"Contract";
            cout<<"\nIntroducet numarul contractului: ";
            cin>>nrContract;
            cout<<"Introduceti anul: ";
            cin>>an;
            cout<<"Introduceti beneficiarul: ";
            cin.get();
            cin>>beneficiar;
            cout<<"Introduceti furnizorul: ";
            cin>>furnizor;
            cout<<"Introduceti valoarea: ";
            cin>>valoare;
            Contract *aux = new Contract(nrContract,an,beneficiar,furnizor,valoare);
            cout<<"Contract Inchiriere";
            cout<<"\nIntroducet numarul contractului: ";
            cin>>nrContract;
            cout<<"Introduceti anul: ";
            cin>>an;
            cout<<"Introduceti beneficiarul: ";
            cin.get();
            cin>>beneficiar;
            cout<<"Introduceti furnizorul: ";
            cin>>furnizor;
            cout<<"Introduceti valoarea: ";
            cin>>valoare;
            cout<<"Introduceti perioada: ";
            cin>>perioada;
            ContractInchiriere *aux1 = new ContractInchiriere(nrContract,an,beneficiar,furnizor,valoare,perioada);
            cout<<"Introduceti nr. dosare: ";
            cin>>perioada;
            Dosar *aux2 = new Dosar(perioada);
            cout<<*aux<<*aux1<<*aux2;
            delete aux;
            delete aux1;
            delete aux2;
            break;
        }
        case 3:{
            cout<<"Citesc contractul aux, declar aux2(aux), afisez aux";
            Contract aux;
            cin>>aux;
            Contract aux2(aux);
            cout<<aux2;

            cout<<"Citesc contractul de inchiriere aux3, declar aux4(aux3), afisez aux4";
            ContractInchiriere aux3;
            cin>>aux3;
            ContractInchiriere aux4(aux3);
            cout<<aux4;

            cout<<"Citesc dosarul aux5, declar aux6(aux5), afisez aux6";
            Dosar aux5;
            cin>>aux5;
            Dosar aux6(aux5);
            cout<<aux6;
            break;
        }
        case 4:{
            Contract aux;
            ContractInchiriere aux3;
            Dosar aux5;
            break;
        }
        case 5:{
            cout<<"Intre doua contracte";
            Contract aux1,aux2;
            cin>>aux1;
            aux2=aux1;
            cout<<aux2;

            cout<<"Intre doua contracte inchiriere";
            ContractInchiriere aux3,aux4;
            cin>>aux3;
            aux4=aux3;
            cout<<aux4;

            cout<<"Intre doua dosare";
            Dosar aux5,aux6;
            cin>>aux5;
            aux6=aux5;
            cout<<aux6;
            break;
        }
        case 6:{
            Contract aux;
            ContractInchiriere aux2;
            Dosar aux3;
            cin>>aux;
            cin>>aux2;
            cin>>aux3;
            cout<<aux<<aux2<<aux3;
            break;
        }
        case 7:{
            cout<<"\nDefinesc un pointer catre catre un obiect Contract";
            Contract *pct;
            cout<<"\nDefinesc un obiect ContractInchiriere";
            ContractInchiriere aux;
            cout<<"\nContractInchiriere are o functie3 care afiseaza \"Functie 3 in derivata\".";
            cout<<"\nApelez functia.\n\n";
            aux.functie3();
            cout<<"\n\nIn pointer-ul catre un obiect Contract adaug adresa si etichete obiectul ContractInchiriere: pct=(Contract*)&aux; ";
            cout<<"\nAr trebui sa scriu asta: (*pct).functie3();";
            cout<<"\nAs primi eroare de compilare: |error: 'class Contract' has no member named 'functie3'|";
            cout<<"\nPentru ca se face Upcast in care obiectul ContractInchiriere se comporta ca si Contract care nu are functie3 definit";

            // In pct avem un pointer catre un obiect din clasa derivata
            // Se apeleaza functiile virtuala din clasa virtuala
            // Nu se pot apela functiile din clasa derivata care nu pot fi apelate in clasa de baza
            pct=&aux;
            cin>>*pct;
            cout<<*pct;
            //(*pct).functie3();
            break;
        }

        case 8:{
            // Obiect clasa de baza
            cout<<"\nObiect clasa de baza";
            Contract aux;
            cout<<"\nDowncast: ContractInchiriere *aux1=(ContractInchiriere*)&aux;";
            // Downcast
            ContractInchiriere *aux1=(ContractInchiriere*)&aux;
            cout<<"\nContractInchiriere are o functie decreaseValoare(int) care scade valoarea cu int";
            // Apelez functia membra decreaseValoare valabila doar pentru functia derivata fiind derivata in aceasta
            (*aux1).decreaseValoare(999);
            // Afisez obiectul initial
            cout<<aux;
            cout<<endl<<"Daca incerc sa citesc prin downcast se vor citi doar datele private din clasa de baza";
            cin>>*aux1;
            cout<<*aux1;
            break;
        }
        case 9:{
            // Daca sunt declarate normal se apeleaza cele definite.
            cout<<"Daca sunt declarate normal, in obiect se apeleaza cele definite\n";
            Contract aux1;
            ContractInchiriere aux2;
            aux1.functie1();
            aux2.functie1();
            // Daca se face upcasting se foloseste cea din baza
            cout<<"Daca se face upcasting, in obiect se foloseste cea din baza\n";
            (*((Contract*)&aux2)).functie1();
            cout<<endl<<endl;
            cout<<"Daca sunt declarate virtual se apeleaza cele definite\n";
            // Daca este declarata virtual
            // Daca sunt folosite obiectele direct
            aux1.functie2();
            aux2.functie2();
            // Daca se face upcasting, in obiect se foloseste cea din derivata
            cout<<"Daca se face upcasting, in obiect se foloseste cea din derivata\n";
            (*((Contract*)&aux2)).functie2();

            // Cel mai evident loc unde se observa utilitatea functiilor de citire si afisare este in functiile de citire si afisare. Ce pot fi folosite pentru obiectele etichetate.
            Contract *aux =(Contract *) new ContractInchiriere();
            cin>>*aux;
            cout<<*aux;
            break;
        }
        case 10:{
            // Counter variabila statica
            // getCounter functie statica
            cout<<Contract::getCounter();
            break;
        }
        case 11:{
            Contract *aux = new Contract;
            aux=aux;
            cout<<"\n+1\n";
            break;
        }
        case 12:{
            if(nr_contracte!=0){
                for(int i=0;i<nr_contracte;i++)
                    delete contracte[i];
                delete [] contracte;
            }

            cout<<"\nIntroduceti numarul de obiecte: ";
            cin>>nr_contracte;

            if(nr_contracte<=0){
                cout<<"Nu poate fi negativ";
                break;
            }

            contracte=new Contract*[nr_contracte];
            for(int i=0;i<nr_contracte;){
                string s;
                cout<<"Introduceti tipul obiectului: ";
                cin>>s;
                if(s=="Contract"){
                    contracte[i]=new Contract;
                    cin>>*contracte[i];
                    i++;
                } else if(s=="ContractInchiriere"){
                    contracte[i]=new ContractInchiriere;
                    cin>>*contracte[i];
                    i++;
                } else{
                cout<<"Incercati Contract sau ContractInchiriere \n";
                }
            }
            break;
        }
        case 13:{
            if(nr_contracte==0){
                cout<<"Nu sunt obiecte\n";
                break;
            }
            for(int i=0;i<nr_contracte;i++)
                cout<<*contracte[i];
            break;
        }
        case 14:{
        Dosar dos;
        cin>>dos;
        dos.showValoareTotala(cout);
        cout<<"\n\nValoare totala dosar: "<<dos.getValoareTotala();
        break;
        }
        case 0: exit(0);
        }
        cout<<"\n\n\n";
        system("pause");
        system("cls");
    }
}
