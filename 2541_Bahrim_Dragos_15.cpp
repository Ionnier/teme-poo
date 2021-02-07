#include <iostream>
#include <stdlib.h>
using namespace std;

class matrice{
private:
    double **M;
    int nr_linii;
    int nr_coloane;
public:
    matrice(int,int,double);
    matrice(matrice&);
    ~matrice();
    void citire(istream&);
    void citire();
    void afisare(ostream&);
    void afisare();
    matrice& operator=(matrice&);
    void reactualizare(int,int,double);
    friend matrice& operator+(matrice&,matrice&);
    friend matrice& operator-(matrice&,matrice&);
    friend matrice& operator*(matrice&,matrice&);
    friend matrice& operator*(matrice&,int);
    friend matrice& operator*(int,matrice&);
    friend bool operator==(matrice&,matrice&);
    friend ostream& operator<<(ostream&,matrice&);
    friend istream& operator>>(istream&,matrice&);
    friend int nrlinii(matrice&);
    friend int nrcol(matrice&);
    friend int nrelemente(matrice&);
    friend matrice& inverse(matrice&);
};

matrice::matrice(int nrlin=1,int nrcol=1,double numar=0){
    if(nrlin<=0 || nrcol<=0){
        cout<<"Eroare in constructia matricii. Nr. linii sau nr. coloane este invalid.\n";
        return;
    }
    // Alocam memorie pentru matrice
    this->M= new double*[nrlin];
    for(int i=0;i<nrlin;i++)
        M[i]=new double[nrcol];

    // Initializam datele membre
    this->nr_linii=nrlin;
    this->nr_coloane =nrcol;

    // Punem elementul primit in toate elementele
    for(int i=0;i<nrlin;i++)
        for(int j=0;j<nrcol;j++)
            M[i][j]=numar;
}

matrice::matrice(matrice &M){
    // Alocam memorie
    this->M=new double*[M.nr_linii];
    for(int i=0;i<M.nr_linii;i++)
        this->M[i]=new double[M.nr_coloane];
    // Initialiam datele private
    this->nr_linii=M.nr_linii;
    this->nr_coloane=M.nr_coloane;
    // Copiem matricea
    for(int i=0;i<nr_linii;i++)
        for(int j=0;j<nr_coloane;j++)
            this->M[i][j]=M.M[i][j];
}

matrice::~matrice(){
    // Dezaloc memoria pe fiecare linie pe care sunt retinute coloanele
    for(int i=0;i<this->nr_linii;i++)
        delete [] this->M[i];
    // Dezaloc memorie pentru fiecare linie
    delete [] M;
    // Schimb numarul de linii la 0
    nr_linii=0;
    nr_coloane=0;
}

void matrice::citire(istream &in){
    cout<<"Introduceti numarul de linii: ";
    in>>nr_linii;
    cout<<"Introduceti numarul de coloane: ";
    in>>nr_coloane;
    if(nr_linii<=0 || nr_coloane<=0){
        cout<<"Eroare in citirea matricii. Nr. linii sau nr. coloane este invalid.\n";
        return;
    }

    // Aloc memorie pentru fiecare inceput de linie
    this->M= new double*[nr_linii];

    // Aloc memorie pentru fiecare coloana de la inceputul fiecarei linii
    for(int i=0;i<nr_linii;i++)
        M[i]=new double[nr_coloane];

    // Citesc elementele
    cout<<"Introduceti elementele matricii:"<<endl;
    for(int i=0;i<nr_linii;i++)
        for(int j=0;j<nr_coloane;j++)
            in>>M[i][j];
}

void matrice::citire(){
    cout<<"Introduceti numarul de linii: ";
    cin>>nr_linii;
    cout<<"Introduceti numarul de coloane: ";
    cin>>nr_coloane;

    if(nr_linii<=0 || nr_coloane<=0){
        cout<<"Eroare in citirea matricii. Nr. linii sau nr. coloane este invalid.\n";
        return;
    }
    // Aloc memorie pentru fiecare inceput de linie
    this->M= new double*[nr_linii];

    // Aloc memorie pentru fiecare coloana de la inceputul fiecarei linii
    for(int i=0;i<nr_linii;i++)
        M[i]=new double[nr_coloane];

    // Citesc elementele
    cout<<"Introduceti elementele matricii:"<<endl;
    for(int i=0;i<nr_linii;i++)
        for(int j=0;j<nr_coloane;j++)
            cin>>M[i][j];
}

void matrice::afisare(ostream &out){
    // Afisez matricea
    for(int i=0;i<nr_linii;i++){
        for(int j=0;j<nr_coloane;j++)
            out<<M[i][j]<<" ";
        out<<endl;
    }
    out<<endl;
}

void matrice::afisare(){
    // Afisez matricea
    for(int i=0;i<nr_linii;i++){
        for(int j=0;j<nr_coloane;j++)
            cout<<M[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

matrice& matrice::operator=(matrice& M){
    // Dezaloc memoria pentru matricea care va fi schimbata, apeland destructorul
    this->~matrice();

    // Schimb datele private
    this->nr_linii=M.nr_linii;
    this->nr_coloane=M.nr_coloane;

    // Aloc memorie pentru matrice, liniile si apoi pe fiecare linie pentru coloane
    this->M= new double*[nr_linii];
    for(int i=0;i<nr_linii;i++)
        this->M[i]=new double[nr_coloane];

    // Copiez elementele
    for(int i=0;i<this->nr_linii;i++)
        for(int j=0;j<this->nr_coloane;j++)
            this->M[i][j]=M.M[i][j];

    return *this;
}

void matrice::reactualizare(int nrlin,int nrcol,double elem){

    // Reactualizarea trebuie sa produca o matrice valida
    if(nrlin<=0 || nrcol<=0){
        cout<<"Reactualiazre imposibila.";
        return;
    }

    // Pastrez matricea initiala in cazul in care aceasta mai trebuie folosita pentru ca nu putem folosii realloc daca folosim new [] si delete []. Astfel toate schimbarile le fac intr-o matrice auxiliara
    // Aloc memorie pentru matricea auxiliara
    double **aux= new double*[nrlin];
    for(int i=0;i<nrlin;i++)
        aux[i]=new double[nrcol];

    // Copiez elementele din matricea initiala.
    // Copiez elementele pana la minimul dintre nr. de linii nou  si cel initial
    // Daca nr. de linii nou este mai mare decat cel initial, atunci copiem toate liniile
    // Daca nr. de linii nou este mai mic decat cel initial, atunci copiem doar numarul egal cu nr. de linii nou
    // Acelasi lucru se aplica si pentru coloane
    for(int i=0;i<(nrlin<nr_linii ? nrlin : nr_linii);i++)
        for(int j=0;j<(nrcol<nr_coloane ? nrcol:nr_coloane);j++)
            aux[i][j]=M[i][j];

    // Daca avem mai multe coloane decat initial atunci trebuie sa punem elementele noi
    // Parcurgem toate liniile care le vrem la final, si pornind de la ultima coloana din matricea initiala pana la nr. de coloane cerut
    if(nrcol>nr_coloane){
        for(int i=0;i<nrlin;i++)
            for(int j=nr_coloane;j<nrcol;j++)
                aux[i][j]=elem;
    }

    // Acelasi lucru si pentru linii
    if(nrlin>nr_linii){
        for(int i=nr_linii;i<nrlin;i++)
            for(int j=0;j<nrcol;j++)
                aux[i][j]=elem;
    }

    // Dezaloc matricea initiala prin apelarea destructorului
    this->~matrice();

    // Copiez adresa noua de memorie
    M=aux;

    // Actualizezz nr de coloane si linii
    this->nr_coloane=nrcol;
    this->nr_linii=nrlin;

    /*
    if(nr_linii<nrlin && nr_coloane<nrcol){
        for(int i=0;i<nr_linii;i++)
            for(int j=0;j<nr_coloane;j++)
                aux[i][j]=this->M[i][j];
        for(int i=nr_linii;i<nrlin;i++)
            for(int j=nr_coloane;j<nrcol;j++)
                aux[i][j]=elem;
    }
    if(nr_linii>nrlin && nr_coloane>nrcol){
        for(int i=0;i<nrlin;i++)
            for(int j=0;j<nrcol;j++)
                aux[i][j]=this->M[i][j];
    }

    if(nr_linii>nrlin && nr_coloane<nrcol){
        for(int i=0;i<nrlin;i++)
            for(int j=0;j<nrcol;j++)
                aux[i][j]=this->M[i][j];
    }
    */
}

inline matrice& operator+(matrice& M1,matrice& M2){

    // Daca matricile nu au dimensiuni potrivite
    if(M1.nr_linii != M2.nr_linii || M2.nr_coloane!=M1.nr_coloane ){
        cout<<"Matricile nu pot fi adunate\n";

        // Returnez ceva, insa eroarea nu este rezolvata
        matrice *aux = new matrice;
        return *aux;
    }

    // Creez o matrice noua prin apelarea constructorului alocand memorie pentru numarul de linii si de coloane
    matrice *aux=new matrice(M1.nr_linii,M1.nr_coloane);

    // Adun cele doua matrici in matricea auxiliara
    for(int i=0;i<aux->nr_linii;i++)
        for(int j=0;j<aux->nr_coloane;j++)
            aux->M[i][j]=M1.M[i][j]+M2.M[i][j];

    // Returnez adresa matricii auxiliare
    return *aux;
}

inline matrice& operator-(matrice& M1,matrice& M2){

    // Daca matricile nu au dimensiuni potrivite
    if(M1.nr_linii != M2.nr_linii || M2.nr_coloane!=M1.nr_coloane ){
        cout<<"Matricile nu pot fi scazute\n";
        // Returnez ceva, insa eroarea nu este rezolvata
        matrice *aux= new matrice;
        return *aux;
    }

    // Creez o matrice noua prin apelarea constructorului alocand memorie pentru numarul de linii si de coloane
    matrice *aux= new matrice(M1.nr_linii,M1.nr_coloane);

    // Adun cele doua matrici in matricea auxiliara
    for(int i=0;i<aux->nr_linii;i++)
        for(int j=0;j<aux->nr_coloane;j++)
            aux->M[i][j]=M1.M[i][j]-M2.M[i][j];

     // Returnez adresa matricii auxiliare
    return *aux;
}

inline matrice& operator*(matrice& M1,int x){

    // Adunarea dintre un scalar si o matrice este posibila in orice caz

    // Aloc memorie
    matrice *aux= new matrice(M1.nr_linii,M1.nr_coloane);

    // Inmuletesc toate elementele
    for(int i=0;i<aux->nr_linii;i++)
        for(int j=0;j<aux->nr_coloane;j++)
            aux->M[i][j]=x*M1.M[i][j];

    // Returnez adresa matricii auxiliare
    return *aux;
}

matrice& operator*(int x,matrice& M){

    // Inmultirea unei matrici cu un scalar este comutativa
    return M*x;

};

matrice& operator*(matrice& M1,matrice& M2){

    // Daca matricile nu au dimensiuni potrivite, inmultirea nu este posibila
    if(M2.nr_linii != M1.nr_coloane){

        // Returnez ceva, insa eroarea nu este rezolvata
        cout<<"Matricile nu pot fi inmultite\n";
        matrice *aux=new matrice;
        return *aux;

    }

    // Aloc memorie
    matrice *aux= new matrice(M1.nr_linii,M2.nr_coloane);

    // Parcurg liniile
    for(int i=0;i<aux->nr_linii;i++)
        // Parcurg coloanele
        for(int j=0;j<aux->nr_coloane;j++)
            // Creez suma elementelor
            for(int k=0;k<M1.nr_coloane;k++){
                aux->M[i][j]=aux->M[i][j]+M1.M[i][k]*M2.M[k][j];
            }
    // Returnez adresa matricii auxiliare
    return *aux;
}

bool operator==(matrice& M1, matrice& M2){
    // Daca nu au aceleasi dimensiuni atunci nu pot sa le egalez
    if(M1.nr_linii != M1.nr_linii || M1.nr_coloane!=M2.nr_coloane){
        return false;
    }
    // Egalez fiecare element
    for(int i=0;i<M1.nr_linii;i++)
        for(int j=0;j<M1.nr_coloane;j++)
            if(M1.M[i][j]!=M2.M[i][j])
                return false;
    return true;
}

int nrlinii(matrice& M){
    return M.nr_linii;
}

int nrcol(matrice& M){
    return M.nr_coloane;
}

int nrelemente(matrice& M){
    return M.nr_coloane * M.nr_linii;
}

double*GPT(double**A,double*b,int n){

    // Algoritmul Gauss cu Pivotare Totala pentru rezolvarea unui sistem de ecuatii sub forma unor matrici
    // Pseudocod din cursul de Calcul Numeric al lect.dr Raisa Pascan

    // Cream o matrice si adaugam ca ultima coloana termenii liberi

    // Aloc memorie
    double **M= new double*[n];
    for(int i=0;i<n;i++)
        M[i]=new double[n+1];

    // Copiez matricea
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            M[i][j]=A[i][j];

    // Adaugam coloana termenilor liberii
    for(int i=0;i<n;i++)
        M[i][n]=b[i];

    // Daca schimbam doua coloane intre ele atunci ordinea necunoscutelor se schimba, deci va trebui schimbata la final
    int *index=new int[n];
    for(int i=0;i<n;i++)
        index[i]=i;

    // Parcurg fiecare linie
    for (int k = 0; k < n; k++) {
		double max=-1;
		int p,m;
		double aux;
		// Cautam elementul maxim din submatrice
		for(int i=k;i<n;i++){
			for(int j=k;j<n;j++){
				if(max<abs(M[i][j])){
					max=abs(M[i][j]);
					p=i;
					m=j;
				}
			}
		}
		// Daca elementul maxim este 0 atunci matricea nu poate fi triunghiularizata ( si implicit inversabila )
		if(M[p][m]!=0){

            // Daca elementul maxim nu este pe aceasi linie, interschimbam liniile
			if(p!=k){
				for(int i=0;i<=n;i++){
					aux=M[p][i];
					M[p][i]=M[k][i];
					M[k][i]=aux;
				}
			}

			// Daca elementul maxim nu este pe aceasi coloana, interschimbam coloanele si interschimba si ordinea valorilor in
			if(m!=k){
				for(int j=0;j<n;j++){
					aux=M[j][m];
					M[j][m]=M[j][k];
					M[j][k]=aux;
				}
				aux=index[k];
				index[k]=index[m];
				index[m]=aux;
			}
			// Facem scaderile pentru a obtine 0 sub diagonala
			for(int i=k+1;i<n;i++){
				for(int j=k+1;j<=n;j++)
					M[i][j]=M[i][j]-((M[i][k]*M[k][j])/M[k][k]);
				M[i][k]=0;
			}

			// Afisarea matricii in timpul transformarilor
            /*
			cout<<"Matricea triangularizata este:"<<endl;
			for(int i=0;i<n;i++){
                for(int j=0;j<=n;j++)
                    cout<<M[i][j]<<" ";
                cout<<endl;
            }
            */

		}
		else {
			cout << "Matricea nu este superior trianghiulara" << endl;
			return M[0];
		}
	}

	// Aplicam algoritmul de Substitutie Descendenta
	// Pseudocod din cursul de Calcul Numeric al lect.dr Raisa Pascan

	// Aloc memorie pentru vector auxiliar in care vor fi stocate solutiile
    double *y=new double[n];

    // Ultima necunoscuta este egala cu ultimul termen liber impartit la coeficient
    y[n-1]=M[n-1][n]/M[n-1][n-1];

    // Parcurgem matricea in ordine descrescatoare scazand necunoscutele pe care le-am calculat anterior
    int k=n-2;
    while(k>=0){
        y[k]=M[k][n];
        for(int j=k+1;j<n;j++)
            y[k]=y[k]-(M[k][j]*y[j]);
        y[k]=y[k]*(1/M[k][k]);
        k-=1;
    }
    // Alocam memorie pentru necunoscute
    double *x=new double[n];
    // Daca s-au schimbat coloane atunci necunoscutele trebuie sa fie schimbata ordinea necunoscutelor cu ajutorul vectorului index
    for(int i=0;i<n;i++)
        x[index[i]]=y[i];

    // Dezaloc memorie pentru matrice si necunoscute
    delete []y;
    for(int i=0;i<n;i++)
        delete[] M[i];
    delete[] M;

    // Returnez adresa de memorie a necunoscutelor
    return x;


}

matrice& inverse(matrice&M){

    // Pseudocod din cursul de Calcul Numeric al lect.dr Raisa Pascan


    // Putem afla inversa doar daca este matrice patratica
    if(M.nr_coloane != M.nr_linii){
        cout<<"nu se poate efectua, nu este patratica";
        return M;
    }
    // Aloc memorie  pentru inversa cu ajutorul constructorului de copiere
    matrice *inversa=new matrice(M);

    // Inversa se calculeaza rezolvand multiple sisteme matriceale. Pentru fiecare coloana din matricea identitate, rezolvam un sistem ( prin GPT ) din matricea caruia ii calculam inversa si coloana aceia din matricea idenitate
    double *V=new double[M.nr_coloane];

    // Coloana din matricea identitate
    for(int i=0;i<M.nr_coloane;i++)
        V[i]=0;
    double *solutie;
    V[0]=1;

    // Rezolv primul sistem
    solutie=GPT(M.M,V,M.nr_coloane);

    // Adaug solutia, coloanei respective din inversa
    for(int i=0;i<M.nr_coloane;i++)
            inversa->M[i][0]=solutie[i];
    int k=1;

    // repet acelasi lucru pentru fiecare coloana din matricea identitate
    while(k<M.nr_coloane){
        V[k-1]=0;
        V[k]=1;
        solutie=GPT(M.M,V,M.nr_coloane);
        for(int i=0;i<M.nr_coloane;i++)
            inversa->M[i][k]=solutie[i];
        k++;
    }

    // returnez adresa inversei
    return *inversa;
}

istream& operator>>(istream& in,matrice& M){
    M.citire(in);
    return in;
}

ostream& operator<<(ostream& out,matrice& M){
    M.afisare(out);
    return out;
}

void menu_output(){
    cout<<"\n Bahrim Dragos Grupa 254 - Proiect 1 - Tema 15 D6 Clasa \"matrice\" (matrice de double): \n";
    cout<<"\n\t MENIU:";
    cout<<"\n===========================================\n";
    cout<<"\n";
    cout<<"1. Citirea a n obiecte"; cout<<"\n";
    cout<<"2. Afisarea a n obiecte citite"; cout<<"\n";
    cout<<"3. Initializare matrice prin numar de linii, coloane si un element"; cout<<"\n";
    cout<<"4. Initializare prin copiere"; cout<<"\n";
    cout<<"5. Initializare fara niciun parametru a unei matrici"; cout<<"\n";
    cout<<"6. Citire si afisare matrice cu metode publice"; cout<<"\n";
    cout<<"7. Citire matrice, afisare matrice prin folosirea metodei-operator ="; cout<<"\n";
    cout<<"8. Citire matricea, reactualizare numar de linii si coloane"; cout<<"\n";
    cout<<"9. Adunarea a doua matrici"; cout<<"\n";
    cout<<"10. Diferenta a doua matrici"; cout<<"\n";
    cout<<"11. Produsul unei matrici si a unui numar intreb"; cout<<"\n";
    cout<<"12. Produsul a doua matrici"; cout<<"\n";
    cout<<"13. Citire matrice, afisare numar de linii"; cout<<"\n";
    cout<<"14. Citire matrice, afisare numar de coloane"; cout<<"\n";
    cout<<"15. Citire matrice, afisare numar de elemente"; cout<<"\n";
    cout<<"16. Citire doua matrici, afisare daca sunt egale"; cout<<"\n";
    cout<<"17. Rezolvare sistem A * X = B"; cout<<"\n";
    cout<<"0. Iesire."; cout<<"\n\n";
    cout<<"Varianta aleasa: ";
}

int main(){
    int n;

    // Pentru citirea a n obiecte avem nevoie de pointer in main si nu in optiune pentru a fi valabil si pentru optiunea 1 si 2
    int nr_obiecte=0;
    matrice *obiecte;

    while(true){
        menu_output();
        cin>>n;
        if(n==1){
            if(nr_obiecte==0){
                cout<<"Introduceti numarul de obiecte: ";
                cin>>nr_obiecte;
                obiecte= new matrice[nr_obiecte];
                for(int i=0; i<nr_obiecte; i++)
                    cin>>obiecte[i];
            }
            else{
                for(int i=0; i<nr_obiecte; i++)
                   obiecte[i].~matrice();
                delete [] obiecte;
                cout<<"Introduceti numarul de obiecte: ";
                cin>>nr_obiecte;
                obiecte= new matrice[nr_obiecte];
                for(int i=0; i<nr_obiecte; i++)
                    cin>>obiecte[i];
            }

        }
        else if(n==2){
            if(nr_obiecte==0)
                cout<<"Nu sunt citite obiecte.\n";
            else
                for(int i=0; i<nr_obiecte; i++)
                    cout<<endl<<obiecte[i]<<endl;
        } else if(n==3) {
            int nrlinii,nrcoloane;
            cout<<"Introduceti numarul de linii: ";
            cin>>nrlinii;
            cout<<"Introduceti numarul de coloanne: ";
            cin>>nrcoloane;
            double element;
            cout<<"Introduceti elementul: ";
            cin>>element;
            matrice M(nrlinii,nrcoloane,element);
            cout<<"S-a construit: \n";
            cout<<M;
        } else if(n==4){
            matrice M1;
            cout<<"Introduceti matricea M1:\n";
            cin>>M1;
            matrice M2(M1);
            cout<<"Matricea copiata:\n";
            cout<<M2;
        } else if(n==5){
            matrice M;
            cout<<M;
        } else if(n==6){
            matrice M;
            M.citire();
            M.afisare();
        } else if(n==7){
            matrice M,M2;
            cout<<"Introduceti matricea:\n";
            cin>>M;
            M2=M;
            cout<<"A fost atribuita matricea:\n";
            cout<<M2;

        } else if(n==8){
            matrice M;
            cin>>M;
            int nrlinii,nrcoloane;
            cout<<"\n Introduceti numarul nou de linii:";
            cin>>nrlinii;
            cout<<"Introduceti numarul de coloane: ";;
            cin>>nrcoloane;
            cout<<"Introduceti elementul: ";
            double element;
            cin>>element;
            M.reactualizare(nrlinii,nrcoloane,element);
            cout<<M;

        } else if(n==9){
            matrice M1,M2;
            cin>>M1;
            cin>>M2;
            cout<<M1+M2;

        } else if(n==10){
            matrice M1,M2;
            cin>>M1;
            cin>>M2;
            cout<<M1-M2;
        } else if(n==11){
            matrice M;
            cin>>M;
            int n;
            cout<<"Numar intreg: ";
            cin>>n;
            cout<<M*n;
        } else if(n==12){
            matrice M1,M2;
            cin>>M1;
            cin>>M2;
            cout<<M1*M2;
        } else if(n==13){
            matrice M;
            cin>>M;
            cout<<nrlinii(M);
        } else if(n==14){
            matrice M;
            cin>>M;
            cout<<nrcol(M);
        } else if(n==15){
            matrice M;
            cin>>M;
            cout<<nrelemente(M);
        } else if(n==16){
            matrice M1,M2;
            cin>>M1;
            cin>>M2;
            cout<<(M1==M2);
        } else if(n==17){
            matrice A,B;
            cout<<"Introudceti A\n";
            cin>>A;
            cout<<"Introduceti B\n";
            cin>>B;
            B=(-1)*B;
            cout<<"\nMatricea X\n";
            cout<<inverse(A);
            cout<<endl<<"Verificare\n";
            cout<<A * (inverse(A) * B) - B; // Daca apar numere cu 10^(-16)  de exemplu, este rezultatul corect intrucat pot fi considerate 0, sunt doar erori  de rotunjire
        }
        else if(n==0){
            exit(0);
        }
        cout<<endl<<endl<<endl;
        system("pause");
        system("cls");
    }
}
