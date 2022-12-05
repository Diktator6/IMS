#include <iostream>
#include <simlib.h>
#include <string>
#include <string.h>
// Vybaveni dilny
#define POCET_SOUSTRUHU 1
#define POCET_STOLU 2
#define POCET_PIL 2

// Config
int cas_simulace_roky = 5;
int pocet_pracovniku_dilny = 3;
int pole_levelu_pracovniku[10] = {1,1,1,1,1,1,1,1,1,1};
int const Levely[10] = {5, 15, 30, 60, 90, 200, 400, 800, 1600, 3200};
// Profit za stul
int profity[10] = {-100, -50, 0, 50, 200, 400, 800, 1600, 3200, 5000};
int vyplaty[10] = {20000, 22000, 24000, 26000, 28000, 30000, 35000, 40000, 50000, 70000};
// Cas pracovni
#define HODINA 60
#define TYDEN 60 * 40
#define ROK 60 * 40 * 52
// Prumerne casy
#define PRUMER_CAS_SOUSTRUH 15
#define PRUMER_CAS_PILA 30
#define PRUMER_CAS_STUL 90
#define PRUMER_CAS_STUL_DOKONCENI 120
// Delsi cas podle levlu konstanta
#define ZTRATA_LVL 0.1

// Cas poruchy
#define CAS_PORUCHY_SOUSTRUHU 60 * 40 * 2
#define CAS_PORUCHY_PILY 60 * 40 
// Odstraneni poruchy
#define ODSTRANENI_PORUCHY_SOUSTRUHU 30
#define ODSTRANENI_PORUCHY_PILY 30

// Obsluzne linky
Facility Stul[POCET_STOLU];
Facility Soustruh[POCET_SOUSTRUHU];
Facility Pila[POCET_PIL];

// Fronty k obsluznym linkam
Queue Stoly_fronta;
Queue Soustruhy_fronta;
Queue Pily_fronta;

using namespace std;

// Statistiky
class Statistiky: public Event{
    public:
        Statistiky(){
            zamestnanci_lvl[9]++;
            for(int i = 0; i < pocet_pracovniku_dilny - 1; i++){
                zamestnanci_lvl[pole_levelu_pracovniku[i] - 1]++;
            }
            // // Pocitani vyplat
            // for(int i = 0; i < pocet_pracovniku_dilny - 1; i++){
            //     vyplata_celkem += dodelane_stoly_celkem[i] * profity[i];
            // } todo del
        }
        void Prepocet_lvl(int lvl){
           zamestnanci_lvl[lvl]++;
           zamestnanci_lvl[lvl - 1]--;
        }
        void Pridej_stul(int lvl){
            dodelane_stoly[lvl]++;
        }
        void Pridej_ztracene_min(int mins, int lvl){
            ztracene_min_lvl[lvl] += mins;
        }
        void Vypis(){
            cout << "######_Rok: " << ++rok << endl;
            cout << "Zamestnanci urcitych levelu: ";
            for(int i = 0; i < 10; i++){
                if(zamestnanci_lvl[i] != 0)
                    cout << "lvl_" << i + 1 <<": " << zamestnanci_lvl[i] << "  ";
            }
            cout << endl << endl << "Pocet dodelanych stolu: " << endl;
            for(int i = 0; i < 10; i++){
                cout << "lvl_" << i + 1 <<": " << dodelane_stoly[i] << "  ";
            }
            cout << endl << endl;
            cout << "Pocet ztracenych hodin: " << endl;
            for(int i = 0; i < 10; i++){
                cout << "lvl_" << i + 1 <<": " << ztracene_min_lvl[i]/HODINA << "  ";
            }
            cout << endl << endl << endl;
        }
        
        void Vypis_celkem(){
            cout << "############_Celkem_############ ";
            cout << endl << "Pocet dodelanych stolu: " << endl;
            for(int i = 0; i < 10; i++){
                cout << "lvl_" << i + 1 <<": " << dodelane_stoly_celkem[i] << "  ";
            }
            cout << endl << endl;
            cout << "Pocet ztracenych hodin: " << endl;
            for(int i = 0; i < 10; i++){
                cout << "lvl_" << i + 1 <<": " << ztracene_min_lvl_celkem[i]/HODINA << "  ";
            }
            // Pocitani celkoveho profitu
            for(int i = 0; i < 10; i++){
                Profit += dodelane_stoly_celkem[i] * profity[i];
            }
            cout << endl << endl << "Celkovy profit: " << Profit << endl;
        }
        void Prepis(){
            for(int i = 0; i < 10; i++){
                ztracene_min_lvl_celkem[i] += ztracene_min_lvl[i];
                dodelane_stoly_celkem[i] += dodelane_stoly[i];  
                ztracene_min_lvl[i] = 0;
                dodelane_stoly[i] = 0;
            }
        }
        void Behavior(){
            Vypis();
            Prepis();
            if(rok == cas_simulace_roky){
                Vypis_celkem();
            }
            Activate(Time + ROK);
        }
        int rok = 0;
        int Profit = 0;
        int vyplata_celkem = 0;
        int dodelane_stoly[10] = {0,0,0,0,0,0,0,0,0,0};
        int ztracene_min_lvl[10] = {0,0,0,0,0,0,0,0,0,0};
        int zamestnanci_lvl[10] = {0,0,0,0,0,0,0,0,0,0};
        // Celkem
        int ztracene_min_lvl_celkem[10] = {0,0,0,0,0,0,0,0,0,0};
        int dodelane_stoly_celkem[10] = {0,0,0,0,0,0,0,0,0,0};
};

// Dovolene todo
// class Dovolene: public Event{
//     public:
//         void Behavior(){
//             pass;
//         }
// }

// Poruchy Soustruhu
class Porucha_soustruhu: public Process{
    public:
        Porucha_soustruhu(int indx){
            Indx = indx;
        };
        void Behavior(){
            Seize(Soustruh[Indx], 2);
            Wait(Exponential(ODSTRANENI_PORUCHY_SOUSTRUHU));
            Release(Soustruh[Indx]);
        }
        int Indx;
};
class Gen_Poruch_soustruhu: public Event{
    public:
        void Behavior(){
            (new Porucha_soustruhu(fac))->Activate();
            ++fac;
            fac = fac % POCET_SOUSTRUHU;
            Activate(Time + Exponential(CAS_PORUCHY_SOUSTRUHU)/POCET_SOUSTRUHU);
        }
        int fac = 0;
};

// Poruchy Pil
class Porucha_pily: public Process {
    public:
        Porucha_pily(int indx){
            Indx = indx;
        };
        void Behavior(){
            Seize(Pila[Indx], 2);
            Wait(Exponential(ODSTRANENI_PORUCHY_PILY));
            Release(Pila[Indx]);
            
        }
        int Indx;
};
class Gen_Poruch_pily: public Event{
    public:
        void Behavior(){
            (new Porucha_pily(fac))->Activate();
            ++fac;
            fac = fac % POCET_PIL;   
            Activate(Time + Exponential(CAS_PORUCHY_PILY)/POCET_PIL);      
        }
        int fac = 0;
};

// Pracovnici
class Pracovnik : public Process {
    public:
        Pracovnik(int mistr, int level, Statistiky *S){
            Stat = S;
            Mistr = mistr;
            Level = level;
            Pocatecni_pocet_stolu = Levely[Level];
            Pocet_dodelanych_stolu = Pocatecni_pocet_stolu;

            Prepocitani_casu();            
        };
        void Behavior(){
            int cas_ve_fronte = 0;
            int cas_ve_fronte_poc = 0;
            int index = 0;
            
            while(1){
                // Prepocitani levlu po dokonceni stolu
                if(Level != 10){
                    Prepocitani_lvl();
                }
                Priority = Level;

                // Faze 1 stul
                if(Mistr){ Wait(Exponential(cas_stul)); } 
                else{
                    index = 0;
                    for(int i = 0; i < POCET_STOLU; i++){
                        if(Stul[i] > Stul[index])
                            index = i;
                        cas_ve_fronte_poc = Time;
                        Seize(Stul[index]);
                        cas_ve_fronte += Time - cas_ve_fronte_poc;
                        Wait(Exponential(cas_stul));
                        Release(Stul[index]);
                    }
                }

                // Faze 2 soustruh
                index = 0;
                for(int i = 0; i < POCET_SOUSTRUHU; i++){
                    if(Soustruh[i] > Soustruh[index])
                        index = i;
                    cas_ve_fronte_poc = Time;
                    // Mistr neceka
                    Seize(Soustruh[index], Mistr);
                    cas_ve_fronte += Time - cas_ve_fronte_poc;
                    Wait(Exponential(cas_soustruh)); 
                    Release(Soustruh[index]);
                }

                // Faze 3 stul
                if(Mistr){ Wait(Exponential(cas_stul)); } 
                else{
                    index = 0;
                    for(int i = 0; i < POCET_STOLU; i++){
                        if(Stul[i] > Stul[index])
                            index = i;
                        cas_ve_fronte_poc = Time;
                        Seize(Stul[index]);
                        cas_ve_fronte += Time - cas_ve_fronte_poc;
                        Wait(Exponential(cas_stul)); 
                        Release(Stul[index]);
                    }
                }
                // Faze 4 pila
                index = 0;
                for(int i = 0; i < POCET_PIL; i++){
                    if(Pila[i] > Pila[index])
                        index = i;
                    cas_ve_fronte_poc = Time;
                    // Mistr neceka
                    Seize(Pila[index], Mistr);
                    cas_ve_fronte += Time - cas_ve_fronte_poc;
                    Wait(Exponential(cas_pila)); 
                    Release(Pila[index]);
                }

                // Faze 5 stul
                if(Mistr){ Wait(Exponential(cas_stul_dokonceni)); } 
                else{
                    index = 0;
                    for(int i = 0; i < POCET_STOLU; i++){
                        if(Stul[i] > Stul[index])
                            index = i;
                        cas_ve_fronte_poc = Time;
                        Seize(Stul[index]);
                        cas_ve_fronte += Time - cas_ve_fronte_poc;
                        Wait(Exponential(cas_stul_dokonceni)); 
                        Release(Stul[index]);
                    }
                }
                Stat->Pridej_stul(Level - 1);
                Stat->Pridej_ztracene_min(cas_ve_fronte, Level - 1);
                cas_ve_fronte = 0;
                Pocet_dodelanych_stolu++;
            }

        }
        // Prepocitani casu podle levlu
        void Prepocitani_casu(){
            cas_stul = int (PRUMER_CAS_STUL + PRUMER_CAS_STUL * ZTRATA_LVL * (10 - Level));
            cas_stul_dokonceni = int (PRUMER_CAS_STUL_DOKONCENI + PRUMER_CAS_STUL_DOKONCENI * ZTRATA_LVL * (10 - Level));
            cas_pila = int (PRUMER_CAS_PILA + PRUMER_CAS_PILA * ZTRATA_LVL * (10 - Level));
            cas_soustruh = int (PRUMER_CAS_SOUSTRUH + PRUMER_CAS_SOUSTRUH * ZTRATA_LVL * (10 - Level));
        }

        // Prepocitani levlu podle poctu dokoncenych stolu
        void Prepocitani_lvl(){
            if(Pocet_dodelanych_stolu >= Levely[Level]){
                Stat->Prepocet_lvl(Level);
                Level++;
                Prepocitani_casu();
            }          
        }
      
        // Atributy
        Statistiky *Stat;
        int Mistr, Level;
        int Pocatecni_pocet_stolu;
        int Pocet_dodelanych_stolu;
        int cas_stul, cas_stul_dokonceni, cas_pila, cas_soustruh;
};

void parse_args(int argc, char** argv) {
    if (argc == 1) {
        return;
    } else if (argc == 2 &&  !strcmp(argv[1], "--help")) {
        cout << endl << "Pouzivani: `make run <Pocet_roku_simulace> <Pocet_pracovniku> <Level_pracovnika_2> ... <Level_pracovnika_N>" << endl;
        cout << "Maximalni pocet pracovniku dilny je 10." << endl;
        cout << "Levely pracovniku jsou v rozsahu 1 - 10." << endl;
        cout << "V zakladu je pocet let simulace nastaven na 5, pocet pracovniku 2 a levely 1." << endl << endl;
        exit(0);
    } else {
        try {
            cas_simulace_roky = stoi(argv[1]);
            pocet_pracovniku_dilny = stoi(argv[2]);
            // Jestlize pocet pracovniku neodpovida postu levlu pracuje se s levli 1
            if(argc != (pocet_pracovniku_dilny + 2))
                return;
            // Pripsani levlu
            for(int i =  3; i < 2 + pocet_pracovniku_dilny; i++){
                // Jestlize level pracovnika neni v rozsahu 1-10 ponecha se 1
                pole_levelu_pracovniku[i - 3] = stoi(argv[i]);
            }
        }
        catch(const exception& e){
            cerr << e.what() << '\n';
        }
        
    }
}

int main(int argc, char** argv){
    // Parsovani 
    parse_args(argc, argv);
    // Inicializace simulace
    Init(0, cas_simulace_roky * ROK + 1);

    // Statistiky
    Statistiky *S = new Statistiky;
    S->Activate(Time + ROK);

    // Generatory
    (new Gen_Poruch_soustruhu)->Activate(Time + Exponential(CAS_PORUCHY_SOUSTRUHU)/POCET_SOUSTRUHU);
    (new Gen_Poruch_pily)->Activate(Time + Exponential(CAS_PORUCHY_PILY)/POCET_PIL);
    // Vytvoreni mistra
    (new Pracovnik(1, 10, S))->Activate();
    // Vytvoreni radovych pracovniku
    for(int i = 1; i < pocet_pracovniku_dilny; i++){
        (new Pracovnik(0, pole_levelu_pracovniku[i-1], S))->Activate();
    }

    Run();
}