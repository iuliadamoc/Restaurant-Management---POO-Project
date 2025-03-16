#define _CRT_SECURE_NO_WARNINGS_
#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<sstream>
#include<list>
#include<set>
#include<map>
using namespace std;

/*    FISIERE UTILIZATE:
ingrediente.txt -- popune stocul initial din bucatarie (cerinta 1)
meniu.txt -- popune meniul initial (cerinta 1)

camara.bin -- salveaza datele despre stoc in fisier binar (cerinta 2)
meniu.bin -- salveaza datele despre meniu in fisier binar (cerinta 2)
comanda.bin -- salveaza datele despre comanda in fisier binar (cerinta 2)
stocFinal.bin -- salveaza datele despre stoc dupa preluarea comenzilor in fisier binar (cerinta 2)

raportStocRamas.txt -- raportul 1 -> stocul ramas (cerinta 3)
raport_nume_retete.txt -- raportul 2 -> nume retete disponibile (cerinta 3)
raportIncasari.txt -- raportul 3 -> sume incasate astazi (cerinta 3)
raportProduseComandate.txt -- raportul 4 -> lista produse comandate astazi (cerinta 3)

camaraCSV.csv -- camara obtinuta din fisiere CSV (cerinta 4)

ingredientVirtualizare.bin -- ingredient afisat si citit din fisier binar ~virtualizarea (cerinta 5)
lista_ingredienteVirtualizare.bin -- stoc afisat si citit din fisier binar ~virtualizarea (cerinta 5)
retetaVirtualizare.bin -- reteta afisata si citita din fisier binar ~virtualizarea (cerinta 5)
meniuVirtualizare.bin -- meniu afisat si citit din fisier binar ~virtualizarea (cerinta 5)

raportComenzileUtilizatorului.txt -- raportul 1 -> lista cu comenzile/operatiunile efectuate de utilizator (cerinta 6)
raportOperatiaCelmaiDesUtilizata.txt -- raportul 2 -> comanda/operatiunea efectcel mai des efectuata de utilizator (cerinta 6)

ing.txt -- fisier out pentru verificari

*/

class ExceptieInput {
    string mesaj;
public:
    ExceptieInput(string mesaj) : mesaj(mesaj) {}

    string getMesaj() { return this->mesaj; }
};

class IReadableaAndWritable {
public:
    virtual void writeToFile(fstream& f) const = 0;
    virtual void restoreFromFile(fstream& f) = 0;
    virtual ~IReadableaAndWritable() = default;
};

class Ingredient : public IReadableaAndWritable {
private:
    string nume = "-";
    int grame = 0;

public:
    Ingredient() {
        string nume = "-";
        int grame = 0;
    }

    Ingredient(string nume, int grame) {
        if (nume != "") {
            this->nume = nume;
        }
        else {
            this->nume = "-";
        }

        if (grame > 0) {
            this->grame = grame;
        }
        else {
            this->grame = 0;
        }
    }

    Ingredient(const Ingredient& ing) {
        if (ing.nume != "") {
            this->nume = ing.nume;
        }
        else {
            this->nume = "-";
        }

        if (ing.grame > 0) {
            this->grame = ing.grame;
        }
        else {
            this->grame = 0;
        }
    }

    Ingredient& operator=(const Ingredient& ing) {
        if (this != &ing) {

            if (ing.nume != "") {
                this->nume = ing.nume;
            }
            else {
                this->nume = "-";
            }

            if (ing.grame > 0) {
                this->grame = ing.grame;
            }
            else {
                this->grame = 0;
            }
        }
        return *this;
    }

    string getNumeIngredient() const {
        return nume;
    }

    int getGrame() const {
        return grame;
    }

    void setNumeIngredient(char* numeNou) {
        if (numeNou != " ")
            this->nume = numeNou;
        else {
            throw ExceptieInput("Input invalid");
        }
    };

    void setGrame(int gramajNou) {
        if (gramajNou >= 0) {
            this->grame = gramajNou;
        }
        else {
            throw ExceptieInput("Input invalid");
        }
    }

    friend ostream& operator<< (ostream& out, const Ingredient& ing) {
        if (ing.nume != "") {
            out << "\nNume ingredient: " << ing.nume << endl;
        }
        else out << "\nIngredientul nu are nume!" << endl;

        if (ing.grame > 0) {
            out << "Grame/Cantitate: " << ing.grame << endl;
        }
        else out << "\nIngredientul nu are inregistrat gramaj!" << endl;

        return out;
    }

    friend ofstream& operator<< (ofstream& out, const Ingredient& ing) {
        out << ing.nume << " ";
        out << ing.grame << endl;

        return out;
    }

    void writeToFile(fstream& f) const override {
        // Calculăm lungimea șirului de caractere "nume" + 1 
        int lg = nume.size() + 1;
        // Scriem în fișier lungimea șirului de caractere
        f.write(reinterpret_cast<const char*>(&lg), sizeof(int));  // converteste adresa de memorie a variabilei lg la un pointer catre un șir de caractere 
        // Scriem în fișier conținutul șirului de caractere
        f.write(nume.data(), lg);
        // Scriem în fișier valoarea variabilei "grame"
        f.write(reinterpret_cast<const char*>(&grame), sizeof(int));
    }

    void restoreFromFile(fstream& f) override {
        int lg;
        // Citim din fișier lungimea șirului de caractere
        f.read(reinterpret_cast<char*>(&lg), sizeof(int));
        // Alocăm memorie pentru bufferul în care vom citi șirul de caractere
        char* buffer = new char[lg];
        // Citim șirul de caractere din fișier în buffer
        f.read(buffer, lg);
        nume = buffer;
        delete[] buffer;
        // Citim din fișier valoarea variabilei "grame"
        f.read(reinterpret_cast<char*>(&grame), sizeof(int));
    }


    friend istream& operator>>(istream& in, Ingredient& ing) {
        cout << "\nNumele ingredienetului: ";
        string buffer;
        in >> buffer;
        if (buffer != "" && buffer.size() > 0) {
            ing.nume = buffer;
        }
        else {
            ing.nume = "";
        }
        cout << "\nGramajul este: ";
        in >> ing.grame;
        if (ing.grame < 0)
            ing.grame = 0;

        return in;
    }


    friend ifstream& operator>>(ifstream& in, Ingredient& ing) {  
        in >> ing.nume;
        in >> ing.grame;
        if (ing.grame < 0)
            ing.grame = 0;

        return in;
    }

    explicit operator int() {
        return this->grame;
    }

    bool operator==(const Ingredient& ing) const {
        if (this->nume != ing.nume) {
            return false;
        }
        if (this->grame != ing.grame) {
            return false;
        }
        return true;
    }

    bool operator!=(Ingredient& ing) {
        if (this->nume == ing.nume) {
            return false;
        }
        if (this->grame != ing.grame) {
            return true;
        }
        return false;
    }

    friend Ingredient& operator+(int nrGrame, Ingredient& ing) {
        if (nrGrame < 0) {
            throw ExceptieInput("Nr de grame nu poate fi negativ!");
        }

        return ing;
    }

    bool operator<(int nrGrame) {
        if ((int)*this < nrGrame) {
            return true;
        }
        return false;
    }

    bool operator>(int nrGrame) {
        if ((int)*this > nrGrame) {
            return true;
        }
        return false;
    }

    Ingredient operator++() {
        if (this->grame > 0) {
            this->grame++;
        }
        return *this;
    }

    Ingredient operator++(int) {
        Ingredient temp = *this;
        if (this->grame > 0) {
            this->grame++;
        }
        return temp;
    }

    Ingredient operator--() {
        if (this->grame > 0) {
            this->grame--;
        }
        return *this;
    }

    Ingredient operator--(int) {
        Ingredient temp = *this;
        if (this->grame > 0) {
            this->grame--;
        }
        return temp;
    }

    ~Ingredient() {

    }
};

class Camara : public IReadableaAndWritable {
private:
    vector<Ingredient> ingredienteCamara;
public:
    Camara() {
    }

    Camara(const vector<Ingredient>& ingrediente) : ingredienteCamara(ingrediente) {
    }


    Camara(const Camara& c) : ingredienteCamara(c.ingredienteCamara) {
    }

    Camara& operator=(const Camara& c) {
        if (this != &c) {
            ingredienteCamara = c.ingredienteCamara;
        }
        return *this;
    }

    ~Camara() {

    }

    friend ostream& operator<<(ostream& out, const Camara& c) {
        if (!c.ingredienteCamara.empty()) {
            out << "\nIngrediente camara: ";
            for (const auto& ingredient : c.ingredienteCamara) {
                out << ingredient; // Apelul operatorului << din clasa Ingredient
                out << " ";
            }
        }
        else {
            out << "\nNu exista ingrediente in camara!";
        }
        return out;
    }

    friend ofstream& operator<<(ofstream& out, const Camara& c) {
        if (!c.ingredienteCamara.empty()) {
            out << c.ingredienteCamara.size() << endl;
            for (const auto& ingredient : c.ingredienteCamara) {
                out << ingredient; // Apelul operatorului << din clasa Ingredient
            }
        }
        return out;
    }

    void writeToFile(fstream& f) const override {
        // Salvare dimensiune vector (size_t)
        size_t size = ingredienteCamara.size();
        f.write((char*)&size, sizeof(size_t));

        // Salvare fiecare ingredient
        for (const auto& ingredient : ingredienteCamara) {
            ingredient.writeToFile(f);
        }
    }

    void restoreFromFile(fstream& f) override {
        // Citire dimensiune vector (size_t)
        size_t size;
        f.read((char*)&size, sizeof(size_t));

        // Dezalocare memorie existentă
        ingredienteCamara.clear();

        // Alocare memorie pentru vectorul de ingrediente
        ingredienteCamara.resize(size);

        // Citire fiecare ingredient
        for (auto& ingredient : ingredienteCamara) {
            ingredient.restoreFromFile(f);
        }
    }

    friend istream& operator>>(istream& in, Camara& c) {
        cout << "\nNr de ingrediente: ";
        size_t size;
        in >> size;

        // Dezalocare memorie existentă
        c.ingredienteCamara.clear();

        if (size > 0) {
            // Alocare memorie pentru vectorul de ingrediente
            c.ingredienteCamara.resize(size);

            cout << "\nIngrediente: ";
            for (auto& ingredient : c.ingredienteCamara) {
                in >> ingredient;
            }
        }

        return in;
    }

    friend ifstream& operator>>(ifstream& in, Camara& c) {
        size_t size;
        in >> size;

        // Dezalocare memorie existentă
        c.ingredienteCamara.clear();

        if (size > 0) {
            // Alocare memorie pentru vectorul de ingrediente
            c.ingredienteCamara.resize(size);

            for (auto& ingredient : c.ingredienteCamara) {
                in >> ingredient;
            }
        }

        return in;
    }

    void readFromCSV(const string& filename, char delimitator) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) { // Iterează prin fiecare linie a fișierului
            istringstream iss(line);

            string nume;
            if (getline(iss, nume, delimitator)) {                //istringstream --descompunerea unui șir de caractere în cuvinte separate
                string cantitateString;
                if (getline(iss, cantitateString, delimitator)) {
                    try {
                        int quantity = stoi(cantitateString);
                        ingredienteCamara.emplace_back(nume, quantity);
                    }
                    catch (const invalid_argument& e) {
                        cerr << "Eroare la convertirea la intreg la linia: " << line << endl;
                    }
                }
                else {
                    cerr << "Eroare la linia: " << line << endl;
                }
            }
            else {
                cerr << "Eroare la linia: " << line << endl;
            }
        }

        file.close();
    }


    void adaugaIngredient(const Ingredient& nou) {
        ingredienteCamara.push_back(nou);
    }

    int gasesteIngredient(const Ingredient& cautat) const {
        for (size_t i = 0; i < ingredienteCamara.size(); i++) {
            if (ingredienteCamara[i] == cautat) {
                return static_cast<int>(i);  // Convertim size_t la int
            }
        }
    }

    void scadeGramaj(const Ingredient& scade) {
        if (scade.getNumeIngredient() != "" && scade.getGrame() > 0) {
            for (size_t i = 0; i < ingredienteCamara.size(); i++) {
                if (ingredienteCamara[i].getNumeIngredient() == scade.getNumeIngredient()) {
                    if (ingredienteCamara[i].getGrame() >= scade.getGrame()) {
                        ingredienteCamara[i].setGrame(ingredienteCamara[i].getGrame() - scade.getGrame());
                        return;
                    }
                    else {
                        throw ExceptieInput("Nu exista suficient ingredient in camara");
                    }
                }
            }
            throw ExceptieInput("Ingredientul cautat nu se afla in camara");
        }
        else {
            throw ExceptieInput("Ingredientul cautat este invalid.");
        }
    }

    void afiseazaIngrediente() const {
        for (const auto& ing : ingredienteCamara) {
            std::cout << ing << "\n";
        }
    }
};

class Retete : public IReadableaAndWritable {
private:
    string numeReteta = "";
    float pret = 0;
    int nrIngrediente = 0;
    Ingredient* ingredienteReteta = nullptr;
public:
    Retete() {
        this->ingredienteReteta = nullptr;
        this->nrIngrediente = 0;
        this->numeReteta = " ";
        this->pret = 0;
    }

    Retete(string numeReteta, float pret, int nrIngrediente, Ingredient* ingredienteReteta) {
        if (numeReteta != "") {
            this->numeReteta = numeReteta;
        }
        else {
            this->numeReteta = "";
        }

        if (pret > 0) {
            this->pret = pret;
        }
        else {
            this->pret = 0;
        }

        if (ingredienteReteta != nullptr && nrIngrediente > 0) {
            this->nrIngrediente = nrIngrediente;
            this->ingredienteReteta = new Ingredient[nrIngrediente];
            for (int i = 0; i < nrIngrediente; i++) {
                this->ingredienteReteta[i] = ingredienteReteta[i];
            }
        }
        else {
            this->ingredienteReteta = nullptr;
            this->nrIngrediente = 0;
        }
    }

    Retete(const Retete& reteta) {
        if (reteta.numeReteta != "") {
            this->numeReteta = reteta.numeReteta;
        }
        else {
            this->numeReteta = "";
        }
        if (reteta.pret > 0) {
            this->pret = reteta.pret;
        }
        else {
            this->pret = 0;
        }
        if (reteta.ingredienteReteta != nullptr && reteta.nrIngrediente > 0) {
            this->nrIngrediente = reteta.nrIngrediente;
            this->ingredienteReteta = new Ingredient[reteta.nrIngrediente];
            for (int i = 0; i < reteta.nrIngrediente; i++) {
                this->ingredienteReteta[i] = reteta.ingredienteReteta[i];
            }
        }
        else {
            this->ingredienteReteta = nullptr;
            this->nrIngrediente = 0;
        }
    }

    Retete& operator=(const Retete& reteta) {
        if (this != &reteta) {
            delete[] this->ingredienteReteta;
            this->ingredienteReteta = nullptr;

            if (reteta.numeReteta != "") {
                this->numeReteta = reteta.numeReteta;
            }
            else {
                this->numeReteta = "";
            }

            if (reteta.pret > 0) {
                this->pret = reteta.pret;
            }
            else {
                this->pret = 0;
            }

            if (reteta.ingredienteReteta != nullptr && reteta.nrIngrediente > 0) {
                this->nrIngrediente = reteta.nrIngrediente;
                this->ingredienteReteta = new Ingredient[reteta.nrIngrediente];
                for (int i = 0; i < reteta.nrIngrediente; i++) {
                    this->ingredienteReteta[i] = reteta.ingredienteReteta[i];
                }
            }
            else {
                this->ingredienteReteta = nullptr;
                this->nrIngrediente = 0;
            }
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Retete& reteta) {
        if (reteta.ingredienteReteta != nullptr && reteta.nrIngrediente > 0) {
            out << "\nNumele retetei: ";
            out << reteta.numeReteta;
            out << "\nPretul retetei: ";
            out << reteta.pret;
            out << "\nLista ingrediente: ";
            for (int i = 0; i < reteta.nrIngrediente; i++) {
                out << reteta.ingredienteReteta[i] << " ";
            }
        }
        else {
            out << "\nNu exista suficiente date pentru aceasta reteta!";
        }

        return out;
    }


    friend ofstream& operator<<(ofstream& out, const Retete& reteta) {
        if (reteta.ingredienteReteta != nullptr && reteta.nrIngrediente > 0) {
            out << endl << reteta.numeReteta;
            out << endl << reteta.pret << endl;
            out << reteta.nrIngrediente << endl;
            for (int i = 0; i < reteta.nrIngrediente; i++) {
                out << reteta.ingredienteReteta[i];
            }
        }
        return out;
    }

    void writeToFile(fstream& f) const override {
        //scriere nume reteta(string)
        //1.scriere lungime sir
        int lg = this->numeReteta.size() + 1;
        f.write((char*)&lg, sizeof(int));
        //2.scriere sir de caractere
        f.write(this->numeReteta.data(), lg);

        //scriere pret (float)
        f.write((char*)&this->pret, sizeof(float));

        if (this->ingredienteReteta != nullptr && this->nrIngrediente > 0) {
            f.write((char*)&this->nrIngrediente, sizeof(int));
            for (int i = 0; i < this->nrIngrediente; i++) {
                this->ingredienteReteta[i].writeToFile(f);
            }
        }
    }

    void restoreFromFile(fstream& f) override {
        //citire nume retata(string)
        //1.citire lg sir
        int lg;
        f.read((char*)&lg, sizeof(int));
        //2.citire sir de caractere
        char* buffer = new char[lg];
        f.read(buffer, lg);
        this->numeReteta = buffer;
        delete[] buffer;

        //citire pret reteta(float)
        f.read((char*)&this->pret, sizeof(float));

        // Citire nr ingrediente (int)
        f.read((char*)&this->nrIngrediente, sizeof(int));

        // Dezalocare memorie existentă, dacă este cazul
        delete[] this->ingredienteReteta;

        // Alocare memorie pentru noul array de ingrediente
        this->ingredienteReteta = new Ingredient[this->nrIngrediente];

        // Citire fiecare ingredient
        for (int i = 0; i < this->nrIngrediente; i++) {
            this->ingredienteReteta[i].restoreFromFile(f);
        }
    }

    friend istream& operator>>(istream& in, Retete& reteta) {
        cout << "\nNumele retetei: ";
        string buffer;
        in >> buffer;
        if (buffer != "" && buffer.size() > 0) {
            reteta.numeReteta = buffer;
        }
        else {
            reteta.numeReteta = "";
        }

        cout << "\nPretul retetei: ";
        in >> reteta.pret;
        if (reteta.pret < 0) {
            reteta.pret = 0;
        }

        cout << "\nNr de ingrediente: ";
        in >> reteta.nrIngrediente;
        if (reteta.nrIngrediente <= 0) {
            reteta.nrIngrediente = 0;
            reteta.ingredienteReteta = nullptr;
            cout << "Atentie : ingredientele retetei au fost eliminate";
        }
        else {
            reteta.ingredienteReteta = new Ingredient[reteta.nrIngrediente];
            cout << "\nIngrediente: ";
            for (int i = 0; i < reteta.nrIngrediente; i++) {
                in >> reteta.ingredienteReteta[i];
            }
        }

        return in;
    }

    friend ifstream& operator>>(ifstream& in, Retete& reteta) {
        string buffer;
        in >> buffer;
        if (buffer != "" && buffer.size() > 0) {
            reteta.numeReteta = buffer;
        }
        else {
            reteta.numeReteta = "";
        }

        in >> reteta.pret;
        if (reteta.pret < 0) {
            reteta.pret = 0;
        }

        in >> reteta.nrIngrediente;
        if (reteta.nrIngrediente <= 0) {
            reteta.nrIngrediente = 0;
            reteta.ingredienteReteta = nullptr;
        }
        else {
            reteta.ingredienteReteta = new Ingredient[reteta.nrIngrediente];
            for (int i = 0; i < reteta.nrIngrediente; i++) {
                in >> reteta.ingredienteReteta[i];
            }
        }

        return in;
    }

    const string getNumeReteta() {
        return this->numeReteta;
    }

    const int getNrIngredienteReteta() {
        return this->nrIngrediente;
    }

    const Ingredient* getIngredienteReteta() {
        return this->ingredienteReteta;
    }

    void setNumeReteta(string numeNouReteta) {
        if (numeNouReteta != " ")
            this->numeReteta = numeNouReteta;
        else
            throw ExceptieInput("\nInvalid input.");
    }

    void setIngredienteReteta(Ingredient* ingredienteNoi, int nrIngredienteNoi) {
        if (ingredienteNoi != nullptr && nrIngredienteNoi > 0) {
            delete[] this->ingredienteReteta;
            this->nrIngrediente = nrIngredienteNoi;
            this->ingredienteReteta = new Ingredient[nrIngredienteNoi];
            for (int i = 0; i < nrIngredienteNoi; i++) {
                this->ingredienteReteta[i] = ingredienteNoi[i];
            }
        }
        else {
            throw ExceptieInput("Nu putem inlocui ingredientele cu nimic.");
        }
    }

    float getPretReteta() {
        return this->pret;
    }

    void setPretReteta(float pretNouReteta) {
        if (pretNouReteta > 0)
            this->pret = pretNouReteta;
        else
            throw ExceptieInput("Pretul nu poate fi negativ.");
    }

    explicit operator int() {
        return this->nrIngrediente;
    }

    ~Retete() {
        delete[] this->ingredienteReteta;
        this->ingredienteReteta = nullptr;
    }

};

class Meniu : public IReadableaAndWritable {
private:
    Retete* reteteMeniu = nullptr;
    int nrRetete = 0;

public:
    Meniu() {
        this->reteteMeniu = nullptr;
        this->nrRetete = 0;
    }

    Meniu(Retete* reteteMeniu, int nrRetete) {
        if (reteteMeniu != nullptr && nrRetete > 0) {
            this->nrRetete = nrRetete;
            this->reteteMeniu = new Retete[nrRetete];
            for (int i = 0; i < nrRetete; i++)
                this->reteteMeniu[i] = reteteMeniu[i];
        }
        else {
            this->nrRetete = 0;
            this->reteteMeniu = nullptr;
        }
    }

    Meniu(const Meniu& componenta) {
        if (componenta.reteteMeniu != nullptr && componenta.nrRetete > 0) {
            this->nrRetete = componenta.nrRetete;
            this->reteteMeniu = new Retete[componenta.nrRetete];
            for (int i = 0; i < componenta.nrRetete; i++)
                this->reteteMeniu[i] = componenta.reteteMeniu[i];
        }
        else {
            this->nrRetete = 0;
            this->reteteMeniu = nullptr;
        }
    }

    Meniu& operator=(const Meniu& componenta) {
        if (this != &componenta) {
            delete[] this->reteteMeniu;
            this->reteteMeniu = nullptr;

            if (componenta.reteteMeniu != nullptr && componenta.nrRetete > 0) {
                this->nrRetete = componenta.nrRetete;
                this->reteteMeniu = new Retete[componenta.nrRetete];
                for (int i = 0; i < componenta.nrRetete; i++)
                    this->reteteMeniu[i] = componenta.reteteMeniu[i];
            }
            else {
                this->nrRetete = 0;
                this->reteteMeniu = nullptr;
            }
        }
        return *this;
    }

    int getNrRetete() {
        return this->nrRetete;
    }

    const Retete* getReteteMeniu() {
        return this->reteteMeniu;
    }

    void setReteteMeniu(Retete* retetaNouaMeniu, int nrNou) {
        if (retetaNouaMeniu != nullptr && nrNou > 0) {
            delete[] this->reteteMeniu;
            this->nrRetete = nrNou;
            this->reteteMeniu = new Retete[nrNou];
            for (int i = 0; i < nrNou; i++) {
                this->reteteMeniu[i] = retetaNouaMeniu[i];
            }
        }
        else {
            throw ExceptieInput("Nu putem inlocui retetele cu nimic");
        }
    }

    friend ostream& operator<<(ostream& out, const Meniu& meniu) {
        if (meniu.nrRetete > 0 && meniu.reteteMeniu != nullptr) {
            out << "Avem " << meniu.nrRetete << " retete in meniu\n";
            for (int i = 0; i < meniu.nrRetete; i++) {
                out << meniu.reteteMeniu[i] << " ";
            }
            out << "\n";
        }
        else out << "Meniul nu contine nicio reteta\n";
        return out;
    }

    friend ofstream& operator<<(ofstream& out, const Meniu& meniu) {
        if (meniu.nrRetete > 0 && meniu.reteteMeniu != nullptr) {
            out << meniu.nrRetete << endl;
            for (int i = 0; i < meniu.nrRetete; i++) {
                out << meniu.reteteMeniu[i] << " ";
            }
            out << "\n";
        }
        return out;
    }

    friend istream& operator>>(istream& in, Meniu& meniu) {
        cout << "Introduceti numarul de retete: ";
        in >> meniu.nrRetete;
        if (meniu.nrRetete <= 0) {
            throw ExceptieInput("Nu putem inlocui retetele cu nimic");
        }
        else {
            delete[] meniu.reteteMeniu;
            meniu.reteteMeniu = new Retete[meniu.nrRetete];
            cout << "Introduceti retele";
            for (int i = 0; i < meniu.nrRetete; i++) {
                in >> meniu.reteteMeniu[i];
            }
        }
        return in;
    }

    friend ifstream& operator>>(ifstream& in, Meniu& meniu) {
        in >> meniu.nrRetete;
        delete[] meniu.reteteMeniu;
        meniu.reteteMeniu = new Retete[meniu.nrRetete];
        for (int i = 0; i < meniu.nrRetete; i++) {
            in >> meniu.reteteMeniu[i];
        }
        return in;
    }

    void writeToFile(std::fstream& f) const override {
        if (reteteMeniu != nullptr && nrRetete > 0) {
            f.write((char*)&nrRetete, sizeof(int));
            for (int i = 0; i < nrRetete; i++) {
                reteteMeniu[i].writeToFile(f);
            }
        }
    }

    void restoreFromFile(std::fstream& f) override {
        // Citire nr retete (int)
        f.read((char*)&nrRetete, sizeof(int));

        // Dezalocare memorie existentă, dacă este cazul
        delete[] reteteMeniu;

        // Alocare memorie pentru noul array de retete
        reteteMeniu = new Retete[nrRetete];

        // Citire fiecare reteta
        for (int i = 0; i < nrRetete; i++) {
            reteteMeniu[i].restoreFromFile(f);
        }
    }


    Retete getReteta(int pozitie) const {
        if (pozitie >= 0 && pozitie < nrRetete) {
            return reteteMeniu[pozitie];
        }
        else {
            throw out_of_range("Pozitie invalida in meniu");
        }
    }


    ~Meniu() {
        delete[] this->reteteMeniu;
        this->reteteMeniu = nullptr;
    }

    int gasesteReteta(string cauta) {
        if (cauta != "") {
            for (int i = 0; i < this->nrRetete; i++) {
                if (this->reteteMeniu[i].getNumeReteta() == cauta) {
                    return i;
                }
            }
        }
        return -1;
    }

    void adaugaReteta(Retete nou) {
        if (nou.getNumeReteta() != "" && nou.getNrIngredienteReteta() > 0) {
            Retete* temp = new Retete[this->nrRetete + 1];
            for (int i = 0; i < this->nrRetete; i++) {
                temp[i] = this->reteteMeniu[i];
            }
            temp[this->nrRetete] = nou;
            delete[] this->reteteMeniu;
            this->reteteMeniu = temp;
            this->nrRetete++;
        }
        else throw ExceptieInput("Nu se poate adauga reteta!");
    }

    void generareRaportNumeRetete() const {
        ofstream raportNumeRetete("raport_nume_retete.txt");

        if (!raportNumeRetete.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru raport.\n";
            return;
        }

        cout << "\n********** Numele Retetelor Disponibile-Raport 2**********\n";

        for (int i = 0; i < nrRetete; ++i) {
            raportNumeRetete << reteteMeniu[i].getNumeReteta() << "\n";
        }

        for (int i = 0; i < nrRetete; ++i) {
            cout << reteteMeniu[i].getNumeReteta() << "\n";
        }

        raportNumeRetete.close();
    }
};

class ComandaCurenta {
private:
    Retete* retete;
    int nrBucati = 0;
public:
    ComandaCurenta() {
        this->retete = nullptr;
        this->nrBucati = 0;
    }
    ComandaCurenta(Retete* retete, int nrBucati) {
        if (retete != nullptr && nrBucati > 0) {
            this->nrBucati = nrBucati;
            this->retete = new Retete[nrBucati];
            for (int i = 0; i < nrBucati; i++) {
                this->retete[i] = retete[i];
            }
        }
        else {
            this->retete = nullptr;
            this->nrBucati = 0;
        }
    }
    ComandaCurenta(const ComandaCurenta& comanda) {
        if (comanda.retete != nullptr && comanda.nrBucati > 0) {
            this->nrBucati = comanda.nrBucati;
            this->retete = new Retete[comanda.nrBucati];
            for (int i = 0; i < comanda.nrBucati; i++) {
                this->retete[i] = comanda.retete[i];
            }
        }
        else {
            this->retete = nullptr;
            this->nrBucati = 0;
        }
    }
    ComandaCurenta& operator=(const ComandaCurenta& comanda) {
        if (this != &comanda) {
            delete[] this->retete;
            this->retete = nullptr;

            if (comanda.retete != nullptr && comanda.nrBucati > 0) {
                this->nrBucati = comanda.nrBucati;
                this->retete = new Retete[comanda.nrBucati];
                for (int i = 0; i < comanda.nrBucati; i++) {
                    this->retete[i] = comanda.retete[i];
                }
            }
            else {
                this->retete = nullptr;
                this->nrBucati = 0;
            }
        }
        return *this;
    }
    ~ComandaCurenta() {
        delete[] this->retete;
        this->retete = nullptr;
    }

    int getNrBucati() const {
        return this->nrBucati;
    }

    void setNrBucati(int nrBucati) {
        if (nrBucati > 0) {
            this->nrBucati = nrBucati;
        }
        else {
            throw ExceptieInput("Numarul de bucati nu poate fi negativ!");
        }
    }

    Retete* getRetete() const {
        return this->retete;
    }

    void adaugaReteta(Retete nou) {
        if (nou.getNumeReteta() != "" && nou.getNrIngredienteReteta() > 0) {
            Retete* temp = new Retete[this->nrBucati + 1];
            for (int i = 0; i < this->nrBucati; i++) {
                temp[i] = this->retete[i];
            }
            temp[this->nrBucati] = nou;
            delete[] this->retete;
            this->retete = temp;
            this->nrBucati++;
        }
        else throw ExceptieInput("Nu se poate adauga reteta!");
    }

    friend ostream& operator<<(ostream& out, const ComandaCurenta& comanda) {
        if (comanda.retete != nullptr && comanda.nrBucati > 0) {
            //out << "\nAvem " << comanda.nrBucati << " retete in comanda\n";
            for (int i = 0; i < comanda.nrBucati; i++) {
                out << comanda.retete[i] << " ";
            }
            out << "\n";
        }
        else out << "\nComanda nu contine nicio reteta\n";
        return out;
    }

    friend ofstream& operator<<(ofstream& out, const ComandaCurenta& comanda) {
        if (comanda.retete != nullptr && comanda.nrBucati > 0) {
            out << comanda.nrBucati;
            for (int i = 0; i < comanda.nrBucati; i++) {
                out << comanda.retete[i];
            }
            out << "\n";
        }
    }

    friend istream& operator>>(istream& in, ComandaCurenta& comanda) {
        cout << "\nIntroduceti numarul de retete: ";
        in >> comanda.nrBucati;
        if (comanda.nrBucati <= 0) {
            throw ExceptieInput("Nu putem inlocui retetele cu nimic");
        }
        else {
            delete[] comanda.retete;
            comanda.retete = new Retete[comanda.nrBucati];
            cout << "\nIntroduceti retetele.";
            for (int i = 0; i < comanda.nrBucati; i++) {
                in >> comanda.retete[i];
            }
        }
        return in;
    }

    friend ifstream& operator>>(ifstream& in, ComandaCurenta& comanda) {
        in >> comanda.nrBucati;
        delete[] comanda.retete;
        comanda.retete = new Retete[comanda.nrBucati];
        for (int i = 0; i < comanda.nrBucati; i++) {
            in >> comanda.retete[i];
        }
        return in;
    }

    void writeToFile(fstream& f) {
        if (this->retete != nullptr && this->nrBucati > 0) {
            f.write((char*)&this->nrBucati, sizeof(int));
            for (int i = 0; i < this->nrBucati; i++) {
                f.write((char*)&this->retete[i], sizeof(Retete));
            }
        }
    }

    void restoreFromFile(fstream& f) {
        // Citire nr ingrediente (int)
        f.read((char*)&this->nrBucati, sizeof(int));

        // Dezalocare memorie existentă, dacă este cazul
        delete[] this->retete;

        // Alocare memorie pentru noul array de ingrediente
        this->retete = new Retete[this->nrBucati];

        // Citire fiecare ingredient
        for (int i = 0; i < this->nrBucati; i++) {
            f.read((char*)&this->retete[i], sizeof(Retete));
        }
    }

    bool operator<(const ComandaCurenta& c) const {
        return nrBucati < c.nrBucati;
    }

    void golesteComanda() {
        delete[] this->retete;
        this->retete = nullptr;
        this->nrBucati = 0;
    }
};

class Restaurant {
private:
    static string numeRestaurant;
    static const string comenziConsola;
    list<string> numeAngajati;
    vector<float> pretComenzi;
    vector<string> produseComandateAzi;
    vector<string> comenzileUtilizatorului;
public:


    Restaurant() :numeAngajati() {
    }
    Restaurant(const list<string>& angajati) : numeAngajati(numeAngajati) {
        if (angajati.empty()) {
            throw ExceptieInput("Lista de angajati este vida!");
        }
    }

    Restaurant(const Restaurant& restaurant) : numeAngajati(restaurant.numeAngajati) {
    }

    Restaurant& operator=(const Restaurant& restaurant) {
        if (this != &restaurant) {
            numeAngajati = restaurant.numeAngajati;
        }
        return *this;
    }

    ~Restaurant() {
    }

    friend ostream& operator<<(ostream& out, const Restaurant& restaurant) {
        out << "\nNumele restaurantului: " << Restaurant::numeRestaurant;
        if (!restaurant.numeAngajati.empty()) {
            out << "\nNumele angajatilor: ";
            for (const string& nume : restaurant.numeAngajati) {
                out << nume << " ";
            }
        }
        else {
            out << "\nNu exista angajati in restaurant!";
        }

        return out;
    }

    friend istream& operator>>(istream& in, Restaurant& restaurant) {
        cout << "\nNr de angajati: ";
        int nrAngajati;
        in >> nrAngajati;

        if (nrAngajati <= 0) {
            throw ExceptieInput("Numarul de angajati trebuie sa fie mai mare decat zero");
        }

        restaurant.numeAngajati.clear(); // Curățăm lista curentă

        cout << "\nNumele angajatilor: ";
        for (int i = 0; i < nrAngajati; i++) {
            string nume;
            in >> nume;
            restaurant.numeAngajati.push_back(nume);
        }

        return in;
    }

    void adaugaAngajat(const string& nume) {
        numeAngajati.push_back(nume);
    }

    int getNrAngajati() {
        return static_cast<int>(numeAngajati.size()); //functia returneaza size_t  si prin static_cast se face conversia explicită la int
    }

    const list<string>& getNumeAngajati() const {  
        return numeAngajati;
    }

    void setAngajati(const list<string>& angajati) {
        if (!angajati.empty()) {
            numeAngajati = angajati;
        }
        else {
            throw ExceptieInput("Lista de angajati este vida!");
        }
    }

    void scrieComenziConsola() {
        cout << Restaurant::comenziConsola << "\n";
    }

    void adaugaPretComanda(float pretTotal) {
        pretComenzi.push_back(pretTotal);
    }

    void adaugaProdusComandat(string numeReteta) {
        produseComandateAzi.push_back(numeReteta);
    }

    void adaugaComandaUtilizator(string comandaUtilizator) {
        comenzileUtilizatorului.push_back(comandaUtilizator);
    }

    void generareRaportIncasari() {
        ofstream incasariOut("raportIncasari.txt");
        cout << "\n*************************Incasarile din ziua de azi - Raport 3**********************\n";


        for (float pret : pretComenzi) {
            cout << pret << "\n";
            incasariOut << pret << endl;
        }
        incasariOut.close();
    }

    void generareRaportProduseComandate() {
        ofstream produseComandate("raportProduseComandate.txt");
        cout << "\n**********************Produsele comandate in ziua de azi - Raport 4**********************\n";

        for (string nume : produseComandateAzi) {
            cout << nume << "\n";
            produseComandate << nume << endl;
        }
        produseComandate.close();
    }

    void generareRaportComenzileUtilizatorului() {
        ofstream comenziUtilizatorFile("raportComenzileUtilizatorului.txt");
        cout << "\n****************Comenzile utilizatorului in ziua de azi - Raport 1**********************\n";

        for (string numeComanda : comenzileUtilizatorului) {
            cout << numeComanda << "\n";
            comenziUtilizatorFile << numeComanda << endl;
        }
        comenziUtilizatorFile.close();
    }

    void generareRaportOperatiuneaCeaMaiDesFolosita() {
        ofstream operatiuneaCeaMaiFolositaFile("raportOperatiaCelmaiDesUtilizata.txt");
        cout << "\n*************Operatia cea mai des utilizata in ziua de azi - Raport 2**********************\n";

        map<string, int> numarOperatii;

        vector<string> toateOperatiunile = comenzileUtilizatorului;

        // Numără de câte ori apare fiecare operațiune
        for (const string& operatiune : toateOperatiunile) {
            numarOperatii[operatiune]++;
        }

        // Găsește operațiunea cu cel mai mare număr de apariții
        int maxAparitii = 0;
        string operatiuneaCeaMaiDesFolosita;

        for (const auto& pair : numarOperatii) {
            if (pair.second > maxAparitii) {
                maxAparitii = pair.second;
                operatiuneaCeaMaiDesFolosita = pair.first;
            }
        }

        // Afisează operațiunea cu cel mai mare număr de apariții
        if (!operatiuneaCeaMaiDesFolosita.empty()) {
            cout << operatiuneaCeaMaiDesFolosita << "\n";
            operatiuneaCeaMaiFolositaFile << operatiuneaCeaMaiDesFolosita;
        }
        else {
            cout << "Nu exista date pentru a determina operatiunea cea mai des folosita.\n";
        }
        operatiuneaCeaMaiFolositaFile.close();
    }
};

string Restaurant::numeRestaurant = "Restaurant Royal";
const string Restaurant::comenziConsola = "\nScrieti vezi_stoc pentru a consulta stocul\nScrieti adauga_reteta pentru a adauga un produs in comada\nScrieti vezi_comanda pentru a vedea comanda curenta\nScrieti vezi_meniu pentru a vedea toate preparatele din meniu\nScrieti paraseste_restaurant pentru a parasi restaurantul\nScrieti goleste_comanda pentru a goli comanda curenta\nScrieti finalizeaza_comanda pentru a finaliza comanda\n\n";

int main(int argc, char* argv[])
{
    string camaraFileName;
    string meniuFileName;
    string camaraFileBinar;
    string meniuFileBinar;
    string comandaFileBinar;
    string camaraFileBinarFinal;

    //CERINTA 1
    if (argc == 7) {
        cout << "***********************************";
        cout << "\nVom folosi: " << argv[0];
        cout << "\nVom utiliza fisierele:\n";
        for (int i = 1; i < 7; i++) {
            cout << argv[i] << endl;
        }
        cout << "***********************************\n";

        camaraFileName = argv[1];
        meniuFileName = argv[2];
        camaraFileBinar = argv[3];
        meniuFileBinar = argv[4];
        comandaFileBinar = argv[5];
        camaraFileBinarFinal = argv[6];
    }
    else {
        cout << "***********************************";
        cout << "\nNu s-au furnizat suficiente date asadar vom folosi: " << argv[0];
        cout << "\nVom utiliza fisierele:\ningrediente.txt\nmeniu.txt\ncamara.bin\nmeniu.bin\ncomanda.bin\nstocfinal.bin\n";
        cout << "***********************************\n";


        camaraFileName = "ingrediente.txt";
        meniuFileName = "meniu.txt";
        camaraFileBinar = "camara.bin";
        meniuFileBinar = "meniu.bin";
        comandaFileBinar = "comanda.bin";
        camaraFileBinarFinal = "stocfinal.bin";
    }

    // Deschide fișierele text pentru citire
    ifstream camaraFile(camaraFileName);
    ifstream meniuFile(meniuFileName);

    // Deschide fișierele text pentru afisare
    ofstream g("ing.txt");

    Camara c;  //creare stoc
    camaraFile >> c; //luare stoc din fisier
    //g<< c;   //afisare stoc in fisier output

    Meniu m;   //creare meniu
    meniuFile >> m; //luare meniu din fisier
    //g << m;  //afisare meniu

    //CERINTA 2 --SALVREA DATELOR IN FISIERE BINARE
    fstream camaraBinar(camaraFileBinar, ios::out | ios::binary);   //scrierea stocului in fis binar
    c.writeToFile(camaraBinar);
    camaraBinar.close();

    fstream meniuBinar(meniuFileBinar, ios::out | ios::binary);     //scrierea meniului in fis binar
    m.writeToFile(meniuBinar);
    meniuBinar.close();

    Restaurant rest;
    rest.adaugaAngajat("Mihai");
    rest.adaugaAngajat("Adriana");
    rest.adaugaAngajat("George");
    rest.adaugaAngajat("Darius");

    string angajatAles = "";
    string buffer;

    ComandaCurenta comandaUtilizator;
    cout << "*************************BINE ATI VENIT LA RESTAURANT ROYAL!***************************";
    cout << "\nCe angajat doriti sa va serveasca masa? ";
    cout << "\nAngajatii disponibili sunt: ";
    for (const auto& angajat : rest.getNumeAngajati()) {  //parcurge fiecare element al listei și îl stochează temporar în variabila angajat
        cout << angajat << " ";
    }

    //AUTO este utilizat pentru a declara o variabilă fără a specifica tipul exact. 
    //CONST indică că elementele din listă nu pot fi modificate în interiorul buclei.
    //& indică că variabila angajat este o referință către elementul din listă, evitând astfel copierea fiecărui element

    cout << endl;
    getline(cin, buffer);
    for (const auto& angajat : rest.getNumeAngajati()) {
        if (buffer == angajat) {
            cout << angajat << " este angajatul ales.\n" << endl;
            angajatAles = angajat;
        }
    }
    if (angajatAles == "") {
        angajatAles = rest.getNumeAngajati().front();
        cout << "Angajatul ales a fost invalid, va fi ales automat primul angajat: " << angajatAles << endl;
    }

    bool deschis = true;

    while (deschis) {
        rest.scrieComenziConsola();

        string comanda;
        getline(cin, comanda);

        rest.adaugaComandaUtilizator(comanda);

        if (comanda == "paraseste_restaurant") {

            fstream camaraBinarFinal(camaraFileBinarFinal, ios::out | ios::binary);   //scrierea stocului final in fis binar
            c.writeToFile(camaraBinarFinal);
            camaraBinarFinal.close();

            cout << "Va multumim ca ati ales restaurantul nostru!\n";
            deschis = false;
        }
        else if (comanda == "goleste_comanda") {
            comandaUtilizator.golesteComanda();
            cout << "Comanda a fost golita!\n";
        }
        else if (comanda == "vezi_stoc") {
            cout << c << "\n";
        }
        else if (comanda == "adauga_reteta") {
            cout << "Ce reteta doriti sa adaugati? ";
            getline(cin, buffer);
            if (m.gasesteReteta(buffer) != -1) {
                cout << "Introduceti cantitatea: ";
                int cantitate;
                cin >> cantitate;
                getchar();
                if (cantitate <= 0) {
                    cout << "Cantitatea nu poate fi negativa sau 0!\n";
                }
                else {
                    cout << "Reteta a fost adaugata!\n";
                    for (int i = 0; i < cantitate; i++) {
                        comandaUtilizator.adaugaReteta(m.getReteteMeniu()[m.gasesteReteta(buffer)]);
                    }
                }
            }
            else {
                cout << "Reteta nu exista in meniu!\n";
            }
        }
        else if (comanda == "finalizeaza_comanda") {
            if (comandaUtilizator.getNrBucati() == 0) {
                cout << "Comanda este goala!\n";
                continue;
            }
            fstream comandaBinar(comandaFileBinar, ios::out | ios::binary);   //scrierea comenzii in fis binar
            comandaUtilizator.writeToFile(comandaBinar);
            comandaBinar.close();

            cout << "Angajatul " << angajatAles << " va aduce comanda!\n";
            Camara copie = c;
            bool succes = true;
            for (int i = 0; i < comandaUtilizator.getNrBucati(); i++) {
                for (int j = 0; j < comandaUtilizator.getRetete()[i].getNrIngredienteReteta(); j++) {
                    try {
                        copie.scadeGramaj(comandaUtilizator.getRetete()[i].getIngredienteReteta()[j]);
                    }
                    catch (ExceptieInput& e) {
                        cout << e.getMesaj() << "\n";
                        cout << "Comanda nu poate fi finalizata!\n";
                        succes = false;
                        break;
                    }
                }
            }
            if (succes) {
                c = copie;
                float pretTotal = 0;
                for (int i = 0; i < comandaUtilizator.getNrBucati(); i++) {
                    rest.adaugaProdusComandat(comandaUtilizator.getRetete()[i].getNumeReteta());
                    pretTotal += comandaUtilizator.getRetete()[i].getPretReteta();
                }
                cout << "Pretul comenzii este: " << pretTotal << "\n";
                cout << "Doriti sa lasati bacis? Suma maxima este 20% din comanda totala (introduceti 0 pt nu).\n";
                float bacsis;
                cin >> bacsis;
                getchar();
                if (bacsis <= 0) bacsis = 0;
                else {
                    if (bacsis > 0.2 * pretTotal) {
                        cout << "Bacsisul dumneavoastra a fost redus automat la 20% din comanda totala: " << 0.2 * pretTotal << " lei.\n";
                        cout << "Veti primi " << bacsis - (0.2 * pretTotal) << " lei inapoi.\n";
                        bacsis = 0.2 * pretTotal;
                    }
                }
                cout << "Total de plata: " << pretTotal + bacsis << "\n";
                rest.adaugaPretComanda(pretTotal + bacsis);
                comandaUtilizator.golesteComanda();
                cout << "\n";
            }
        }
        else if (comanda == "vezi_comanda") {
            cout << "Comanda curenta este: " << comandaUtilizator;
            cout << "\n";
        }
        else if (comanda == "vezi_meniu") {
            cout << "Produsele disponibile sunt: " << m;
            cout << "\n";
        }
    }

    //CERINTA 7--MAP
    cout << "\n***********************COMANDA PLASATA FOLOSIND MAP*********************\n";
    map <int, ComandaCurenta> comandaMea;

    Retete reteta2 = m.getReteta(1); // A doua reteta din meniu
    Retete reteta3 = m.getReteta(2); // A treia reteta din meniu
    Retete reteta4 = m.getReteta(3); // A patra reteta din meniu

    ComandaCurenta c1(&reteta2, 1);
    ComandaCurenta c2(&reteta3, 1);
    ComandaCurenta c3(&reteta4, 1);

    comandaMea[0] = c1;
    comandaMea[1] = c2;
    comandaMea[2] = c3;

    for (const auto& pair : comandaMea) {
        int idComanda = pair.first;  // Cheia map-ului
        const ComandaCurenta& comanda = pair.second;  // Valoarea map-ului  || Creează o referință constantă către obiectul ComandaCurenta din map(nu face o copie)

        // Afiseaza detaliile comenzii folosind metodele din clasa ComandaCurenta
        cout << "Comanda cu ID " << idComanda << ":\n";
        cout << "Numar de bucati: " << comanda.getNrBucati() << "\n";

        // Parcurge fiecare reteta din comanda
        for (int j = 0; j < comanda.getNrBucati(); j++) {
            const Retete& reteta = comanda.getRetete()[j];
            cout << reteta << "\n";
        }
    }

    //CERINTA 7--SET
    set <ComandaCurenta> comandaMeaSet;

    comandaMeaSet.insert(c1); 
    comandaMeaSet.insert(c2);
    comandaMeaSet.insert(c3);

    cout << "\n***********************COMANDA PLASATA FOLOSIND SET*********************\n";

    set<ComandaCurenta>::iterator itSet;
    for (itSet = comandaMeaSet.begin(); itSet != comandaMeaSet.end(); itSet++)
        cout << *itSet << " ";

    //CERINTA 7--MULTISET
    multiset <ComandaCurenta> comandaMeaMultiset;

    comandaMeaMultiset.insert(c1);
    comandaMeaMultiset.insert(c2);
    comandaMeaMultiset.insert(c3);

    cout << "\n***********************COMANDA PLASATA FOLOSIND MULTISET*********************\n";

    multiset<ComandaCurenta>::iterator itMultiset;
    for (itMultiset = comandaMeaMultiset.begin(); itMultiset != comandaMeaMultiset.end(); itMultiset++)
        cout << *itMultiset << " ";


    //GENERARE RAPOARTE - CERINTA 3
    //Raport 1 
    ofstream stocOut("raportStocRamas.txt");
    stocOut << c;
    cout << "\n**********************Stocul ramas in camara-Raport 1**********************\n";
    cout << c;
    stocOut.close();

    //Raport 2 
    m.generareRaportNumeRetete();

    //Raport 3 
    rest.generareRaportIncasari();

    //Raport 4
    rest.generareRaportProduseComandate();

    //GENERARE RAPOARTE DATA MINING- CERINTA 6
    //Raport 1
    rest.generareRaportComenzileUtilizatorului();

    //Raport 2
    rest.generareRaportOperatiuneaCeaMaiDesFolosita();

    //FISIERE CSV - CERINTA 4
    cout << "\n***********************Camara obtinuta din fisiere CSV:*************************************\n";
    Camara camaraDinCSV;
    camaraDinCSV.readFromCSV("camaraCSV.csv", ';');
    cout << camaraDinCSV;

    //CERINTA 5-VIRTUALIZAREA
    cout << "\n***********************VIRTUALIZAREA*************************************\n";
    cout << "\n***********************pentru Ingrediente*************************************\n";
    Ingredient ing1("Faina", 500);

    fstream fileOutIngredient("ingredientVirtualizare.bin", ios::binary | ios::out);
    ing1.writeToFile(fileOutIngredient);
    fileOutIngredient.close();

    Ingredient ing2;
    fstream fileInIngredient("ingredientVirtualizare.bin", ios::binary | ios::in);
    ing2.restoreFromFile(fileInIngredient);
    fileInIngredient.close();

    cout << "\nIngredient citit din fisier: \n" << ing2 << "\n";

    cout << "\n***********************pentru Camara/Stoc*************************************\n";
    Camara lista;
    Ingredient ing3("Zahar", 300);

    lista.adaugaIngredient(ing1);
    lista.adaugaIngredient(ing3);

    fstream fileOutCamara("lista_ingredienteVirtualizare.bin", ios::binary | ios::out);
    lista.writeToFile(fileOutCamara);
    fileOutCamara.close();

    Camara listaCitita;
    fstream fileInCamara("lista_ingredienteVirtualizare.bin", ios::binary | ios::in);
    listaCitita.restoreFromFile(fileInCamara);
    fileInCamara.close();

    cout << "Stoc ingrediente citit din fisier:\n";
    listaCitita.afiseazaIngrediente();

    cout << "\n***********************pentru Retete*************************************\n";
    fstream fileOutReteta("retetaVirtualizare.bin", ios::out | ios::binary);

    Retete retetaClatite("Clatite", 10.5f, 4, new Ingredient[4]{
        Ingredient("Lapte", 50),
        Ingredient("Zahar", 30),
        Ingredient("Oua", 20),
        Ingredient("Faina", 100)
        });

    retetaClatite.writeToFile(fileOutReteta);
    fileOutReteta.close();

    fstream fileInReteta("retetaVirtualizare.bin", ios::in | ios::binary);

    Retete retetaCitita;
    retetaCitita.restoreFromFile(fileInReteta);
    fileInReteta.close();

    cout << "Reteta citita: " << retetaCitita << endl;

    cout << "\n***********************pentru Meniu*************************************\n";
    Retete retetaCarbonara("Paste carbonara", 25, 2, new Ingredient[2]{
        Ingredient("Paste",100),
        Ingredient("Sos carbonara",40)
        });

    Retete retetaSalata("Salata Caesar", 20, 3, new Ingredient[3]{
        Ingredient("Salata verde",100),
        Ingredient("Crutoane",30),
        Ingredient("Pui",100)
        });

    Retete reteteMeniuVirtualizare[] = { retetaCarbonara, retetaSalata };
    Meniu meniuVirtualizare(reteteMeniuVirtualizare, 2);

    meniuVirtualizare.adaugaReteta(retetaClatite);

    fstream fileOutMeniu("meniuVirtualizare.bin", ios::out | ios::binary);
    if (fileOutMeniu.is_open()) {
        meniuVirtualizare.writeToFile(fileOutMeniu);
        fileOutMeniu.close();
    }
    else {
        cerr << "Eroare la deschiderea fisierului pentru scriere.\n";
    }

    Meniu meniuRestaurat;
    std::fstream fileInMeniu("meniuVirtualizare.bin", ios::in | ios::binary);
    if (fileInMeniu.is_open()) {
        meniuRestaurat.restoreFromFile(fileInMeniu);
        fileInMeniu.close();
    }
    else {
        cerr << "Eroare la deschiderea fisierului pentru citire.\n";
    }

    cout << "Meniu restaurant:\n" << meniuRestaurat;


    camaraFile.close();
    meniuFile.close();
    g.close();

    return 0;
}
