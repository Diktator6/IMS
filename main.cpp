#include <iostream>
#include <simlib.h>

// Vybaveni dilny
#define POCET_SOUSTRUHU 1
#define POCET_STOLU 2
#define POCET_PIL 2

// Config
#define POCET_PRACOVNIKU_DILNY 1
enum Levely { 10, 20, 30, 60, 90, 120, 240, 480, 960}
// Prumerne casy
#define PRUMER_CAS_SOUSTRUH 15
#define PRUMER_CAS_PILA 30
#define PRUMER_CAS_STUL 90
#define PRUMER_CAS_STUL_DOKONCENI 120

#define ZTRATA_LVL 0.1


Facility Soustruh[POCET_SOUSTRUHU];
Facility pila[POCET_PIL];
Facility stul[POCET_STOLU];
Facility stul_mistr;




using namespace std;


/*
class Trans: public Process{
    public:
        Trans(): Process(){
            tid++;
            id = tid;
        };
        
        void Behavior(){
            int kt = -1;
            int cre_time = Time;
            
            zpet:
            for(int a = 0; a < KOLIK; a++){
                if(!Fac[a].Busy()) {kt = a; break;}
            }
            cout << "Here "<< kt << endl;

            if(kt == -1){
                cout << "PASSSSSSSSSSSSSSS "<< kt << endl;
                Into(Q);
                Passivate();
                goto zpet;
            }

            Seize(Fac[kt]);
            Wait(Exponential(30));
            Release(Fac[kt]);
            if(Q.Length() > 0){
                (Q.GetFirst())->Activate();
            }

            cout << "Cas procesu " << id << " v simulaci byl " << Time - cre_time << "." << endl;
        }

        int id;
        int cre_time;
        int = pocet_dodelanych_stolu;
};

class Obj: public Event {
    void Behavior(){
        (new Trans)->Activate();
        Activate(Time + Exponential(3));
        cout << "Hello in time " << Time << endl;
    }
};
*/

class Pracovnik: public Process{
    public:
        Pracovnik(int mistr, int level):public Process(){
            Mistr = mistr;
            Level = level;
        };
        Behabior{
            // todo prepocitani levlu po dokonceni stolu
            if(mistr){ Wait(Exponential(PRUMER_CAS_STUL))}
            else{
                // todo
                Seize(Stul)
                Wait()
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
            
        }

        // Atributy
        int Mistr, Level;
        int pocet_dodelanych_stolu = 0;
        int cas_stul, cas_stul_dokonceni, cas_pila, cas_soustruh;
}

int main(int argc, char *argv[]){
    // Inicializace simulace
    Init(0, 50);

    // Generatory
    // Vytvoreni mistra
    (new Pracovnik(1, 10));
    // Vytvoreni radovych pracovniku
    for(int i = 1; i < POCET_PRACOVNIKU_DILNY; i++){
        (new Pracovnik(0,3))
    }

    Run();
}