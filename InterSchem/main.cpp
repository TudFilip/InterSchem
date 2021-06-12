#include <iostream>
#include <string.h>
#include <graphics.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#define infinit INT_MAX
#define epsi 0.0001
#define  MAX 100
#define MAX1 20

using namespace std;

int inaltime_fereastra = GetSystemMetrics(SM_CYFULLSCREEN);

struct pct
{
    int x,y;
};
FILE* fisi;
struct bloc
{
    pct stup,drdown;
    char tip,text[50],v[10];
    float val;
    int pLeg[5];
} VectorBlocuri[101];
int contor_start = 0;
int x, y; // Retin valorile coordonatelor mouse-ului in diferite functii
int i=2;  // Retine numarul de blocuri pe care le avem introduse si functioneaza pe post de contor
int contor_blocuri = 100;
bool afis_linie_titlu_meniu = true;
int saves[100]; // vector ce retine pasii facuti
int scontor=0; // contor pentru pasii facuti inainte si pentru care se poate da "undo"

//// EVAL - INCEPUT
int top1,top2; // varfurile celor doua stive
const int max_stiva=100;

double Opd[max_stiva]; // stiva operanzilor
char Op[max_stiva]; // stiva operatorilor

char OperatiiBinare[200]="+-*/^<>=#";
char OperatiiUnare[200]="scarel";
char Operatii[200]="+-*/^<>=#scarel";

int Prioritate(char c)  // prioritate operatorilor
{
    if(c=='(' || c==')')
        return 0;
    if(c=='+' || c=='-')
        return 1;
    if(c=='*' || c=='/')
        return 2;
    if(c=='^')
        return 3;
    if(c=='=' || c=='#' || c=='<' || c=='>')
        return 4;

}

const float pi=3.1415926; // se da sub forma literei q

bool DifInf(float x)
{
    return fabs(infinit-fabs(x)) > infinit / 2.0;
}

float Logaritm(float x)
{
    if (x>epsi && DifInf(x))
        return log(x);
    else
        return infinit;
}

float Exponential(float x)
{
    if (DifInf(x))
        return exp(x);
    else
        return infinit;
}

float Inmultit(float x, float y)
{
    if (fabs(x < epsi) || fabs(y) < epsi)
        return 0;
    else if (DifInf(x) && DifInf(y))
        return x*y;
    else
        return infinit;
}

float Putere(float x, float y)
{
    // float p;
    if (x==0)
        return 0;
    else if (y==0)
        return 1;
    else if (x==infinit || y==infinit)
        return infinit;
    else
        return pow(x,y);
}

float Egal(float x, float y)
{
    return x==y;
}

float Diferit(float x, float y)
{
    return x!=y;
}

float MaiMic(float x, float y)
{
    return x < y;
}

bool MaiMare(float x, float y)
{
    return x > y;
}

float Plus(float x, float y)
{
    if (DifInf(x) && DifInf(y))
        return x+y;
    else
        return infinit;
}
float Minus(float x, float y)
{
    if (DifInf(x) && DifInf(y))
        return x-y;
    else
        return infinit;
}
float Impartit(float x, float y)
{
    if (fabs(y)>epsi)
        return x/y;
    else
        return infinit;
}
float Modul(float x)
{
    if (DifInf(x))
        return fabs(x);
    else
        return infinit;
}
bool EsteNumar(char sir[MAX1])
{
    return (atof(sir)!=0.0 &&
            strchr("0123456789",sir[0]));
}

struct functie
{
    char expresie[300];
    char vect[MAX+1][MAX1];// vectorul cuprinzand “cuvintele” ce formeaza expresia
    int lung; // lungimea efectiva a vectorului
    float a,b; // intervalul de evaluare a functiei
    int n; //numarul de puncte de evaluare
};
double ValoareFunctie(functie E, double x)
{
    int valoare;
    // returneaza valoarea functiei E in punctul x
    double valo,x_1,x_2;
    //calculul expresiei
    for (valoare=1; valoare<=max_stiva; valoare++)
    {
        Opd[valoare]=0;
        Op[valoare]='@';
    }
    top1=0;
    top2=1;
    Op[top2]='(';
    valoare=0;
    while (valoare<=E.lung && top2>0)
    {
        valoare++;
        if (EsteNumar(E.vect[valoare]))
        {
            top1++;
            valo=atof(E.vect[valoare]);
            Opd[top1]=valo;
        }
        else
            switch (E.vect[valoare][0])
            {
            /* constanta pi=3.1415926 se da sub forma literei q */
            case 'q':
                top1++;
                Opd[top1]=pi;
                break;
            case 'X': /* variabila x */
                top1++;
                Opd[top1]=x;
                break;
            case '(': /* inceput de bloc */
                top2++;
                Op[top2]='(';
                break;
            default:
                /* operatii binare si unare */
                while ((top2>0) && !(strchr("()",Op[top2])) &&
                        Prioritate(Op[top2])>=Prioritate(E.vect[valoare][0]))
                {
                    if (top1>1)
                        x_1=Opd[top1-1];
                    x_2=Opd[top1];
                    /* functii scrise in OPERATII */
                    switch (Op[top2])
                    {
                    case '=':
                        valo=Egal(x_1,x_2);
                        break;
                    case '#':
                        valo=Diferit(x_1,x_2);
                        break;
                    case '<':
                        valo=MaiMic(x_1,x_2);
                        break;
                    case '>':
                        valo=MaiMare(x_1,x_2);
                        break;
                    case '+':
                        valo=Plus(x_1,x_2);
                        break;
                    case '-':
                        valo=Minus(x_1,x_2);
                        break;
                    case '*':
                        valo=Inmultit(x_1,x_2);
                        break;
                    case '/':
                        valo=Impartit(x_1,x_2);
                        break;
                    case '^':
                        valo=Putere(x_1,x_2);
                        break;
                    } //operator[top2]
                    if (strchr(OperatiiBinare,Op[top2]))
                        top1--;
                    if (strchr(Operatii,Op[top2]))
                        Opd[top1]=valo;
                    top2--;
                }
                if (top2>0)
                    if ((Op[top2]!='(') || (strcmp(E.vect[valoare],")")))
                    {
                        top2++;
                        Op[top2] = E.vect[valoare][0];
                    }
                    else
                        top2--;
            }
    }
    if (top2==0)
        return Opd[1];
    else
        return infinit;
}
//// EVAL - FINAL


//Functii pentru Interfata Grafica

// CULORI FOLOSITE //
// Culoare butoane meniu principal: Umplere - 165, 186, 199; Margini - 49, 52, 54


//Functia care creeaza interfata pentru pagina initiala/principala
void Meniu()
{
    setlinestyle(SOLID_LINE,1,2);
    readimagefile("fundal_meniu.bmp",0,0,1500,inaltime_fereastra);

    settextstyle(0, HORIZ_DIR, 2);
    setbkcolor(COLOR(165, 186, 199));
    setcolor(COLOR(49, 52, 54));
    setfillstyle(1, COLOR(165, 186, 199));

    rectangle(430,inaltime_fereastra/2-30,1075,inaltime_fereastra/2+30);
    floodfill(433,inaltime_fereastra/2-13,COLOR(49, 52, 54));

    setcolor(BLACK);
    outtextxy(431,inaltime_fereastra/2-8," Deseneaza schema  SchemaSalv  Oprire program ");
    setcolor(COLOR(49, 52, 54));

    rectangle(430,inaltime_fereastra/2-30,1075,inaltime_fereastra/2+30);
    rectangle(681,inaltime_fereastra/2-30,849,inaltime_fereastra/2+30);
}

//Functia care creeaza interfata pentru pagina in care se vor desena schemele logice
void Meniu_Desen()
{
    setlinestyle(SOLID_LINE,1,2);
    setbkcolor(COLOR(219, 218, 232));
    clearviewport();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setbkcolor(COLOR(0,255,85));
    setfillstyle(1, COLOR(0,255,85));
    setcolor(BLACK);

    setcolor(COLOR(0,255,85));
    line(0,70,1200,70);
    setfillstyle(1, COLOR(0,255,85));
    bar(246,20,956,50);

    setcolor(BLACK);
    outtextxy(250,27," START  STOP  INTRARE  IESIRE  DECIZIE  ATRIBUIRE ");

    rectangle(246,20,956,50);
    rectangle(346,20,434,50);
    rectangle(434,20,560,50);
    rectangle(672,20,798,50);

    if(scontor != 0)
    {
        setcolor(BLACK);
        bar(1000,20,1100,50);
        outtextxy(1022,27,"UNDO");
        rectangle(1000,20,1100,50);
    }

    setfillstyle(1, COLOR(0,255,85));
    bar(1200,0,1500,inaltime_fereastra);

    char text_buton[200];
    setfillstyle(1, COLOR(128, 255, 212));
    setbkcolor(COLOR(128, 255, 212));

    strcpy(text_buton, "EXECUTA SCHEMA");
    bar(1220,20,1480,80);
    rectangle(1220,20,1480,80);
    outtextxy(1252,40,text_buton);

    strcpy(text_buton, "UNESTE BLOCURI");
    bar(1220,100,1480,160);
    rectangle(1220,100,1480,160);
    outtextxy(1252,120,text_buton);

    strcpy(text_buton, "SALVEAZA SCHEMA");
    bar(1220,180,1480,240);
    rectangle(1220,180,1480,240);
    outtextxy(1244,200,text_buton);

    strcpy(text_buton, "MUTA BLOC");
    bar(1220,260,1480,320);
    rectangle(1220,260,1480,320);
    outtextxy(1282,280,text_buton);

    strcpy(text_buton, "EDITEAZA NOD");
    bar(1220,inaltime_fereastra-240,1480,inaltime_fereastra-180);
    rectangle(1220,inaltime_fereastra-240,1480,inaltime_fereastra-180);
    outtextxy(1264,inaltime_fereastra-220,text_buton);

    strcpy(text_buton, "STERGERE NOD");
    bar(1220,inaltime_fereastra-160,1480,inaltime_fereastra-100);
    rectangle(1220,inaltime_fereastra-160,1480,inaltime_fereastra-100);
    outtextxy(1264,inaltime_fereastra-140,text_buton);

    strcpy(text_buton, "INAPOI LA MENIU");
    bar(1220,inaltime_fereastra-80,1480,inaltime_fereastra-20);
    rectangle(1220,inaltime_fereastra-80,1480,inaltime_fereastra-20);
    outtextxy(1243,inaltime_fereastra-60,text_buton);
}

void Afis_Contor()
{
    setcolor(BLACK);
    setbkcolor(COLOR(219, 218, 232));
    char text[] = "BLOCURI RAMASE: ";
    char contor[10];
    itoa(contor_blocuri, contor, 10);
    strcat(text, contor);
    outtextxy(15,inaltime_fereastra-35,text);
}

void Inserare_Coordonate_Blocuri(int x_st, int y_st, int x_dr, int y_dr, int j)
{
    VectorBlocuri[j].stup.x = x_st;
    VectorBlocuri[j].stup.y = y_st;
    VectorBlocuri[j].drdown.x = x_dr;
    VectorBlocuri[j].drdown.y = y_dr;
}

void DESEN_BLOC_START(int x, int y,int cul)
{
    char text[]="START";
    int lungime_text = textwidth(text);

    setfillstyle(1,cul);
    setcolor(CYAN);
    bar(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    rectangle(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-8,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-10, y-15, x+lungime_text+10, y+15, 1);

}
void DESEN_BLOC_STOP(int x, int y, int j,int cul)
{
    char text[]="STOP";
    int lungime_text = textwidth(text);

    setfillstyle(1,cul);
    setcolor(RED);
    bar(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    rectangle(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-8,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-10, y-15, x+lungime_text+10, y+15,j);
}
void DESEN_BLOC_INTRARE(char text[], int x, int y, int j,int cul)
{
    int lungime_text = textwidth(text);

    setfillstyle(1,cul);
    setcolor(COLOR(7,22,237));

    line(x-lungime_text-5,y-15,x+lungime_text+5,y-15);
    line(x-lungime_text-10,y+15,x+lungime_text+10,y+15);
    line(x-lungime_text-5,y-15,x-lungime_text-10,y+15);
    line(x+lungime_text+5,y-15,x+lungime_text+10,y+15);

    floodfill(x,y,COLOR(7,22,237));
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-8,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-10, y-15, x+lungime_text+10, y+15,j);
}
void DESEN_BLOC_IESIRE(char text[], int x, int y, int j,int cul)
{
    int lungime_text = textwidth(text);

    setfillstyle(1,cul);
    setcolor(COLOR(237,7,107));

    line(x-lungime_text-5,y+15,x+lungime_text+5,y+15);
    line(x-lungime_text-10,y-15,x+lungime_text+10,y-15);
    line(x-lungime_text-5,y+15,x-lungime_text-10,y-15);
    line(x+lungime_text+5,y+15,x+lungime_text+10,y-15);

    floodfill(x+1,y+1,COLOR(237,7,107));
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-8,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-10, y-15, x+lungime_text+10, y+15,j);
}
void DESEN_BLOC_DECIZIE(char text[],int x, int y, int j,int cul)
{
    int lungime_text = textwidth(text);
    int inaltime_text = textheight(text);

    setfillstyle(1,cul);
    setcolor(COLOR(12,245,90));

    line(x-lungime_text-lungime_text/2,y+inaltime_text,x+lungime_text+lungime_text/2,y+inaltime_text);
    line(x-lungime_text-lungime_text/2,y+inaltime_text,x,y-inaltime_text);
    line(x+lungime_text+lungime_text/2,y+inaltime_text,x,y-inaltime_text);

    floodfill(x,y,COLOR(12,245,90));
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-1,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-lungime_text/2, y-inaltime_text, x+lungime_text+lungime_text/2, y+inaltime_text,j);
}

void DESEN_BLOC_ATRIBUIRE(char text[],int x, int y, int j,int cul)
{
    int lungime_text = textwidth(text);

    setfillstyle(1,cul);
    setcolor(COLOR(227, 124, 14));

    bar(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    rectangle(x-lungime_text-10,y-15,x+lungime_text+10,y+15);
    setbkcolor(cul);
    setcolor(BLACK);
    outtextxy(x-lungime_text/2,y-8,text);

    Inserare_Coordonate_Blocuri(x-lungime_text-10, y-15, x+lungime_text+10, y+15,j);
}

void CitireSir(char s[],int x,int y)
{
    strcpy(s,"");
    bool PrimaApasare = false;
    char s1[50];
    char t[2];
    char tasta = '\0';
    t[0] = tasta;
    t[1] = '\0';
    strcat(s, t);
    strcpy(s1, s);
    setcolor(BLACK);

    do
    {
        tasta = getch();
        if (tasta != '\0' && PrimaApasare == false)
        {
            PrimaApasare = true;
            bar(x-10, y-20, 1480, y+40);
        }
        if (tasta == 8) // backspace
            if (strlen(s)>0)
            {
                setcolor(COLOR(219, 218, 232));
                outtextxy(x+strlen(s1)*14, y, "_");
                strcpy(s1, s);
                outtextxy(x, y, s1);
                s[strlen(s)-1] = '\0';
                strcpy(s1, s);
                setcolor(BLACK);
                outtextxy(x+strlen(s1)*14, y, "_");
                outtextxy(x, y, s1);
            }
            else
                Beep(400,200);
        else
        {
            if(tasta!=13)
            {
                setcolor(COLOR(219, 218, 232));
                outtextxy(x+strlen(s1-1)*14, y, "_");
                setcolor(BLACK);
                t[0]=tasta;
                t[1]='\0';
                strcat(s,t);
                strcpy(s1,s);
                setcolor(BLACK);
                outtextxy(x,y,s1);
                setcolor(BLACK);
                outtextxy(x+strlen(s1)*14, y, "_");
            }
        }
    }
    while (tasta != 13 && PrimaApasare);
    setcolor(COLOR(219, 218, 232));
    outtextxy(x+strlen(s1)*14, y, "_");
}

void StergereDesenBlocInitial(char text[], int j)
{
    setfillstyle(1,COLOR(219, 218, 232));
    setcolor(BLACK);
    rectangle(VectorBlocuri[j].stup.x,VectorBlocuri[j].stup.y,VectorBlocuri[j].drdown.x,VectorBlocuri[j].drdown.y);
    floodfill(VectorBlocuri[j].stup.x + textwidth(text) + 10,VectorBlocuri[j].stup.y + 15,COLOR(219, 218, 232));
    setcolor(COLOR(219, 218, 232));
    rectangle(VectorBlocuri[j].stup.x,VectorBlocuri[j].stup.y,VectorBlocuri[j].drdown.x,VectorBlocuri[j].drdown.y);
    floodfill(VectorBlocuri[j].stup.x + textwidth(text) + 10,VectorBlocuri[j].stup.y + 15,COLOR(219, 218, 232));
    setcolor(BLACK);
}

void STERGERE_BLOC_INTRODUCERE()
{
    setfillstyle(1, COLOR(0,255,85));
    bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
    bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
    bar(1220,inaltime_fereastra-480,1480,inaltime_fereastra-420);
}

void DESEN_BLOC_INTRODUCERE (char tip, int j)
{
    setfillstyle(1, COLOR(219, 218, 232));
    setbkcolor(COLOR(219, 218, 232));
    char text[50],txt[50]="";
    strcpy(text,VectorBlocuri[j].text);
    if (tip == 'i' || tip == 'o')
    {
        bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
        setcolor(BLACK);
        outtextxy(1250,inaltime_fereastra-300,"Introdu datele");

        CitireSir(VectorBlocuri[j].text, 1230, inaltime_fereastra-300);
    }
    else if(tip == 'a')
    {
        bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
        setcolor(BLACK);
        outtextxy(1250,inaltime_fereastra-300,"Introdu datele");
        setbkcolor(COLOR(0,255,85));
        outtextxy(1225,inaltime_fereastra-360,"Scrie o variabila");
        setbkcolor(COLOR(219, 218, 232));
        CitireSir(txt, 1230, inaltime_fereastra-300);
        strcpy(VectorBlocuri[j].text,txt);
        strcat(VectorBlocuri[j].text,"<-");
        setbkcolor(COLOR(0,255,85));
        setcolor(BLACK);
        outtextxy(1225,inaltime_fereastra-360,"Introdu o expresie");
        setbkcolor(COLOR(219, 218, 232));
        outtextxy(1230,inaltime_fereastra-300,VectorBlocuri[j].text);
        CitireSir(txt, 1240+textwidth(VectorBlocuri[j].text), inaltime_fereastra-300);
        strcat(VectorBlocuri[j].text,txt);
    }
    else if(tip == 'd')
    {
        bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
        setcolor(BLACK);
        outtextxy(1280,inaltime_fereastra-300,"Expresie 2");

        bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
        setcolor(BLACK);
        outtextxy(1300,inaltime_fereastra-380,"Relatie");

        bar(1220,inaltime_fereastra-480,1480,inaltime_fereastra-420);
        setcolor(BLACK);
        outtextxy(1280,inaltime_fereastra-460,"Expresie 1");

        CitireSir(txt, 1230,inaltime_fereastra-460);
        strcpy(VectorBlocuri[j].text,txt);
        strcpy(txt,"");
        CitireSir(txt, 1230, inaltime_fereastra-380);
        strcat(VectorBlocuri[j].text,txt);
        strcpy(txt,"");
        CitireSir(txt, 1230, inaltime_fereastra-300);
        strcat(VectorBlocuri[j].text,txt);
    }
    switch(VectorBlocuri[j].tip)
    {
    case 'i':
        StergereDesenBlocInitial(text, j);
        DESEN_BLOC_INTRARE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + 10, VectorBlocuri[j].stup.y + 15,j,14);
        STERGERE_BLOC_INTRODUCERE();
        break;
    case 'o':
        StergereDesenBlocInitial(text, j);
        DESEN_BLOC_IESIRE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + 10, VectorBlocuri[j].stup.y + 15,j,14);
        STERGERE_BLOC_INTRODUCERE();
        break;
    case 'a':
        StergereDesenBlocInitial(text, j);
        DESEN_BLOC_ATRIBUIRE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + 10, VectorBlocuri[j].stup.y + 15,j,14);
        STERGERE_BLOC_INTRODUCERE();
        break;
    case 'd':
        StergereDesenBlocInitial(text, j);
        DESEN_BLOC_DECIZIE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + textwidth(VectorBlocuri[j].text)/2, VectorBlocuri[j].stup.y + textheight(VectorBlocuri[j].text),j,14);
        STERGERE_BLOC_INTRODUCERE();
        break;
    default:
        break;
    }

    setcolor(COLOR(219, 218, 232));
    setbkcolor(COLOR(219, 218, 232));
    setfillstyle(1, COLOR(219, 218, 232));
}

bool Suprapus(int xMouse,int yMouse,char tip)
{
    for(int j=1; j<i; j++)
        if(VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse)
        {
            return false;
        }
    return true;
}


void DeseneazaNod(char tip,int &j)
{
    int xMouse,yMouse;
    clearmouseclick(WM_LBUTTONDOWN);
    STERGERE_BLOC_INTRODUCERE();
    switch(tip)
    {
    case 's':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1100 && Suprapus(xMouse,yMouse,'s'))
                DESEN_BLOC_START(xMouse,yMouse,14);
        }
        break;
    case 't':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1100 && Suprapus(xMouse,yMouse,'t'))
                DESEN_BLOC_STOP(xMouse,yMouse,j,14);
            else
            {
                VectorBlocuri[j].tip = '\0';
                j--;
            }
        }
        break;
    case 'i':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1100 && Suprapus(xMouse,yMouse,'i'))
            {
                DESEN_BLOC_INTRARE(VectorBlocuri[i].text,xMouse,yMouse,i,14);
                DESEN_BLOC_INTRODUCERE(tip,i);
            }
            else
            {
                VectorBlocuri[i].tip = '\0';
                i--;
            }
        }
        break;
    case 'o':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1100 && Suprapus(xMouse,yMouse,'o'))
            {
                DESEN_BLOC_IESIRE(VectorBlocuri[i].text,xMouse,yMouse,i,14);
                DESEN_BLOC_INTRODUCERE(tip,i);
            }
            else
            {
                VectorBlocuri[i].tip = '\0';
                i--;
            }
        }
        break;
    case 'd':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1050 && Suprapus(xMouse,yMouse,'d'))
            {
                DESEN_BLOC_DECIZIE(VectorBlocuri[i].text,xMouse,yMouse,i,14);
                DESEN_BLOC_INTRODUCERE(tip,i);
            }
            else
            {
                VectorBlocuri[i].tip = '\0';
                i--;
            }
        }
        break;
    case 'a':
        while (!ismouseclick(WM_LBUTTONDOWN))
            delay(50);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(yMouse>=90 && xMouse<=1100 && Suprapus(xMouse,yMouse,'a'))
            {
                DESEN_BLOC_ATRIBUIRE(VectorBlocuri[i].text,xMouse,yMouse,i,14);
                DESEN_BLOC_INTRODUCERE(tip,i);
            }
            else
            {
                VectorBlocuri[i].tip = '\0';
                i--;
            }
        }
        break;
    }
}


void StergereBloc(int j)
{
    VectorBlocuri[j].tip = '0';
    VectorBlocuri[j].stup.x = 0;
    VectorBlocuri[j].stup.y = 0;
    VectorBlocuri[j].drdown.x = 0;
    VectorBlocuri[j].drdown.y = 0;
    VectorBlocuri[j].text[0] = '\0';
    VectorBlocuri[VectorBlocuri[j].pLeg[3]].pLeg[1]= 0;
    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1]= 0;
    VectorBlocuri[j].pLeg[0]= 0;
    VectorBlocuri[j].pLeg[1]= 0;
    VectorBlocuri[j].pLeg[2]= 0;
    VectorBlocuri[j].pLeg[3]= 0;
}

void StergereDesenBloc(int x, int y, int j)
{
    setfillstyle(1,COLOR(219, 218, 232));
    setcolor(BLACK);
    rectangle(VectorBlocuri[j].stup.x,VectorBlocuri[j].stup.y,VectorBlocuri[j].drdown.x,VectorBlocuri[j].drdown.y);
    floodfill(x,y,COLOR(219, 218, 232));
    setcolor(COLOR(219, 218, 232));
    rectangle(VectorBlocuri[j].stup.x,VectorBlocuri[j].stup.y,VectorBlocuri[j].drdown.x,VectorBlocuri[j].drdown.y);
    floodfill(x,y,COLOR(219, 218, 232));
    setcolor(BLACK);
}

void InterschimbareValoriBlocuri(int b1,int b2)
{
    VectorBlocuri[101].tip = VectorBlocuri[b1].tip;
    VectorBlocuri[101].stup.x = VectorBlocuri[b1].stup.x;
    VectorBlocuri[101].stup.y = VectorBlocuri[b1].stup.y;
    VectorBlocuri[101].drdown.x = VectorBlocuri[b1].drdown.x;
    VectorBlocuri[101].drdown.y = VectorBlocuri[b1].drdown.y;
    strcpy(VectorBlocuri[101].text,VectorBlocuri[b1].text);
    VectorBlocuri[101].pLeg[0]= VectorBlocuri[b1].pLeg[0];
    VectorBlocuri[101].pLeg[1]= VectorBlocuri[b1].pLeg[1];
    VectorBlocuri[101].pLeg[2]= VectorBlocuri[b1].pLeg[2];
    VectorBlocuri[101].pLeg[3]= VectorBlocuri[b1].pLeg[3];

    VectorBlocuri[b1].tip = VectorBlocuri[b2].tip;
    VectorBlocuri[b1].stup.x = VectorBlocuri[b2].stup.x;
    VectorBlocuri[b1].stup.y = VectorBlocuri[b2].stup.y;
    VectorBlocuri[b1].drdown.x = VectorBlocuri[b2].drdown.x;
    VectorBlocuri[b1].drdown.y = VectorBlocuri[b2].drdown.y;
    strcpy(VectorBlocuri[b1].text,VectorBlocuri[b2].text);
    VectorBlocuri[b1].pLeg[0]= VectorBlocuri[b2].pLeg[0];
    VectorBlocuri[b1].pLeg[1]= VectorBlocuri[b2].pLeg[1];
    VectorBlocuri[b1].pLeg[2]= VectorBlocuri[b2].pLeg[2];
    VectorBlocuri[b1].pLeg[3]= VectorBlocuri[b2].pLeg[3];

    VectorBlocuri[b2].tip = VectorBlocuri[101].tip;
    VectorBlocuri[b2].stup.x = VectorBlocuri[101].stup.x;
    VectorBlocuri[b2].stup.y = VectorBlocuri[101].stup.y;
    VectorBlocuri[b2].drdown.x = VectorBlocuri[101].drdown.x;
    VectorBlocuri[b2].drdown.y = VectorBlocuri[101].drdown.y;
    strcpy(VectorBlocuri[b2].text,VectorBlocuri[101].text);
    VectorBlocuri[b2].pLeg[0]= VectorBlocuri[101].pLeg[0];
    VectorBlocuri[b2].pLeg[1]= VectorBlocuri[101].pLeg[1];
    VectorBlocuri[b2].pLeg[2]= VectorBlocuri[101].pLeg[2];
    VectorBlocuri[b2].pLeg[3]= VectorBlocuri[101].pLeg[3];
}

void SalvareTemporaraValoriBloc(int bloc)
{
    VectorBlocuri[101].tip = VectorBlocuri[bloc].tip;
    VectorBlocuri[101].stup.x = VectorBlocuri[bloc].stup.x;
    VectorBlocuri[101].stup.y = VectorBlocuri[bloc].stup.y;
    VectorBlocuri[101].drdown.x = VectorBlocuri[bloc].drdown.x;
    VectorBlocuri[101].drdown.y = VectorBlocuri[bloc].drdown.y;
    strcpy(VectorBlocuri[101].text,VectorBlocuri[bloc].text);
    VectorBlocuri[101].pLeg[0]= VectorBlocuri[bloc].pLeg[0];
    VectorBlocuri[101].pLeg[1]= VectorBlocuri[bloc].pLeg[1];
    VectorBlocuri[101].pLeg[2]= VectorBlocuri[bloc].pLeg[2];
    VectorBlocuri[101].pLeg[3]= VectorBlocuri[bloc].pLeg[3];
}

void ResetareVectorBlocuri()
{
    for(int j=1; j<=i; j++)
        StergereBloc(j);
    i=2;
}

void DeseneazaLinie(int x1, int y1, int x2, int y2, int cul)
{
    setcolor(cul);
    line(x1,y1,x1,(y1+y2)/2);
    line(x1,(y1+y2)/2,x2,(y1+y2)/2);
    line(x2,(y1+y2)/2,x2,y2);
}

void Miscare_Bloc()
{
    while (!ismouseclick(WM_LBUTTONDOWN))
        delay(50);
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        int xMouse = mousex();
        int yMouse = mousey();
        for(int j=1; j<i; j++)
            if(VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse)
            {
                SalvareTemporaraValoriBloc(j);

                int xMouse_nou, yMouse_nou;
                switch(VectorBlocuri[j].tip)
                {
                case 's':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1100 && Suprapus(xMouse_nou,yMouse_nou,'s'))
                        {
                            int x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_pleaca_bloc_curent, y_linie_pleaca_bloc_curent;

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                                y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_START(xMouse_nou, yMouse_nou, 14);

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,BLACK);
                                setcolor(BLACK);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                case 't':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1100 && Suprapus(xMouse_nou,yMouse_nou,'t'))
                        {
                            int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_STOP(xMouse_nou, yMouse_nou, j, 14);

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                                setcolor(BLACK);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                case 'i':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1100 && Suprapus(xMouse_nou,yMouse_nou,'i'))
                        {
                            int x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_pleaca_bloc_curent, y_linie_pleaca_bloc_curent;
                            int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                                y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_INTRARE(VectorBlocuri[j].text,xMouse_nou,yMouse_nou,j,14);

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                                int cul = BLACK;
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                                    cul = RED;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                                setcolor(cul);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                           if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                                setcolor(BLACK);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                case 'o':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1100 && Suprapus(xMouse_nou,yMouse_nou,'o'))
                        {
                            int x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_pleaca_bloc_curent, y_linie_pleaca_bloc_curent;
                            int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                                y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                               if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_IESIRE(VectorBlocuri[j].text, xMouse_nou, yMouse_nou, j, 14);

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                                int cul = BLACK;
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                                    cul = RED;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                                setcolor(cul);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                           if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                                setcolor(BLACK);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                case 'd':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1050 && Suprapus(xMouse_nou,yMouse_nou,'d'))
                        {
                            int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                            int x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_pleaca_bloc_curent_A, y_linie_pleaca_bloc_curent_A;

                            int x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_pleaca_bloc_curent_F, y_linie_pleaca_bloc_curent_F;

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_bloc_urmator_A = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                                y_linie_bloc_urmator_A = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                                x_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.x + 2;
                                y_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent_A,y_linie_pleaca_bloc_curent_A,x_linie_bloc_urmator_A,y_linie_bloc_urmator_A,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A-5, y_linie_bloc_urmator_A-5);
                                line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A+5, y_linie_bloc_urmator_A-5);
                            }

                            if(VectorBlocuri[j].pLeg[2] != 0)
                            {
                                x_linie_bloc_urmator_F = (VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2;
                                y_linie_bloc_urmator_F = VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y - 2;
                                x_linie_pleaca_bloc_curent_F = VectorBlocuri[j].stup.x + 2;
                                y_linie_pleaca_bloc_curent_F = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent_F,y_linie_pleaca_bloc_curent_F,x_linie_bloc_urmator_F,y_linie_bloc_urmator_F,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F-5, y_linie_bloc_urmator_F-5);
                                line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F+5, y_linie_bloc_urmator_F-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_DECIZIE(VectorBlocuri[j].text,xMouse_nou,yMouse_nou,j,14);

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                                setcolor(BLACK);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.x + 2;
                                y_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.y + 2;
                                int cul = BLACK;
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                                    cul = RED;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent_A,y_linie_pleaca_bloc_curent_A,x_linie_bloc_urmator_A,y_linie_bloc_urmator_A,cul);
                                setcolor(cul);
                                line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A-5, y_linie_bloc_urmator_A-5);
                                line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A+5, y_linie_bloc_urmator_A-5);
                            }

                            if(VectorBlocuri[j].pLeg[2] != 0)
                            {
                                x_linie_pleaca_bloc_curent_F = VectorBlocuri[j].stup.x + 2;
                                y_linie_pleaca_bloc_curent_F = VectorBlocuri[j].drdown.y + 2;
                                int cul = BLACK;
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                                    cul = RED;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent_F,y_linie_pleaca_bloc_curent_F,x_linie_bloc_urmator_F,y_linie_bloc_urmator_F,cul);
                                setcolor(cul);
                                line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F-5, y_linie_bloc_urmator_F-5);
                                line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F+5, y_linie_bloc_urmator_F-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                case 'a':
                    while (!ismouseclick(WM_LBUTTONDOWN))
                        delay(50);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        xMouse_nou = mousex();
                        yMouse_nou = mousey();
                        if(yMouse_nou>=90 && xMouse_nou<=1100 && Suprapus(xMouse_nou,yMouse_nou,'a'))
                        {
                            int x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_pleaca_bloc_curent, y_linie_pleaca_bloc_curent;
                            int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                                y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                            if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                                setcolor(COLOR(219, 218, 232));
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }

                            StergereDesenBloc(xMouse, yMouse, j);
                            DESEN_BLOC_ATRIBUIRE(VectorBlocuri[j].text,xMouse_nou,yMouse_nou,j,14);

                            if(VectorBlocuri[j].pLeg[1] != 0)
                            {
                                x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                                int cul = BLACK;
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                                    cul = RED;

                                DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                                setcolor(cul);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                                line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                            }

                           if(VectorBlocuri[j].pLeg[0] != 0)
                            {
                                if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                                {
                                    x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                                    y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                }
                                else
                                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                    {
                                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                                    }
                                    else
                                    {
                                        x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                                    }
                                x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                                y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                                DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                                setcolor(BLACK);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                                line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                            }
                        }
                        else
                            InterschimbareValoriBlocuri(j,101);
                    }
                    break;
                }
            }
    }
}

void Stergere()
{
    int xMouse,yMouse;
    while (!ismouseclick(WM_LBUTTONDOWN))
        delay(50);
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        xMouse=mousex();
        yMouse=mousey();
    }
    for(int j=2; j<=i; j++)
        if(VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse )
        {
            switch(VectorBlocuri[j].tip)
            {
            case 't':
                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip != 'd')
                    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                else
                    if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd')
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                            VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] = 0;

                StergereDesenBloc(xMouse, yMouse, j);

                StergereBloc(j);
                for(int k=j; k<=i; k++)
                    InterschimbareValoriBlocuri(k,k+1);
                i--;
                j--;
                break;
            case 'i':
                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip != 'd')
                    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                else
                    if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd')
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                            VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] = 0;
                if(VectorBlocuri[j].pLeg[1] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[0] = 0;

                StergereDesenBloc(xMouse, yMouse, j);

                StergereBloc(j);
                for(int k=j; k<=i; k++)
                    InterschimbareValoriBlocuri(k,k+1);
                i--;
                j--;
                break;
            case 'o':
                int x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_pleaca_bloc_curent, y_linie_pleaca_bloc_curent;
                int x_linie_bloc_anterior, y_linie_bloc_anterior, x_linie_vine_bloc_curent, y_linie_vine_bloc_curent;

                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip != 'd')
                    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                else
                    if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd')
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                            VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] = 0;
                if(VectorBlocuri[j].pLeg[1] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[0] = 0;

                StergereDesenBloc(xMouse, yMouse, j);

                StergereBloc(j);
                for(int k=j; k<=i; k++)
                    InterschimbareValoriBlocuri(k,k+1);
                i--;
                j--;
                break;
            case 'a':
                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip != 'd')
                    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                else
                    if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd')
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                            VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] = 0;
                if(VectorBlocuri[j].pLeg[1] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[0] = 0;

                StergereDesenBloc(xMouse, yMouse, j);

                StergereBloc(j);
                for(int k=j; k<=i; k++)
                    InterschimbareValoriBlocuri(k,k+1);
                i--;
                j--;
                break;
            case 'd':
                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator_A = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator_A = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.x + 2;
                    int y_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent_A,y_linie_pleaca_bloc_curent_A,x_linie_bloc_urmator_A,y_linie_bloc_urmator_A,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A-5, y_linie_bloc_urmator_A-5);
                    line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A+5, y_linie_bloc_urmator_A-5);
                }

                if(VectorBlocuri[j].pLeg[2] != 0)
                {
                    int x_linie_bloc_urmator_F = (VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2;
                    int y_linie_bloc_urmator_F = VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent_F = VectorBlocuri[j].stup.x - 2;
                    int y_linie_pleaca_bloc_curent_F = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent_F,y_linie_pleaca_bloc_curent_F,x_linie_bloc_urmator_F,y_linie_bloc_urmator_F,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F-5, y_linie_bloc_urmator_F-5);
                    line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F+5, y_linie_bloc_urmator_F-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip != 'd')
                    VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                else
                    if(VectorBlocuri[j].pLeg[0] != 0 && VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd')
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                            VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] = 0;
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                                VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] = 0;
                if(VectorBlocuri[j].pLeg[1] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[0] = 0;
                if(VectorBlocuri[j].pLeg[2] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[2]].pLeg[0] = 0;
                if(VectorBlocuri[j].pLeg[3] != 0)
                    VectorBlocuri[VectorBlocuri[j].pLeg[3]].pLeg[1] = 0;

                StergereDesenBloc(xMouse, yMouse, j);

                StergereBloc(j);
                for(int k=j; k<=i; k++)
                    InterschimbareValoriBlocuri(k,k+1);
                i--;
                j--;
                break;
            }
        }
}

void Editare()
{
    int xMouse,yMouse;
    while (!ismouseclick(WM_LBUTTONDOWN))
        delay(50);
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        xMouse=mousex();
        yMouse=mousey();
    }
    for(int j=2; j<=i; j++)
        if(VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse)
        {

            switch(VectorBlocuri[j].tip)
            {
            case 'i':
                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }

                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                   int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                   int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                   int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                   int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                DESEN_BLOC_INTRODUCERE(VectorBlocuri[j].tip,j);

                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                    int cul = BLACK;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                        cul = RED;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                    setcolor(cul);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                    setcolor(BLACK);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }
                break;
            case 'o':
                    if(VectorBlocuri[j].pLeg[1] != 0)
                    {
                        int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                        int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                        setcolor(COLOR(219, 218, 232));
                        line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                        line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                    }

                    if(VectorBlocuri[j].pLeg[0] != 0)
                    {
                        int x_linie_bloc_anterior, y_linie_bloc_anterior;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                            {
                                x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                            }
                            else
                            {
                                x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                            }
                        int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                        DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                        setcolor(COLOR(219, 218, 232));
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                    }

                    DESEN_BLOC_INTRODUCERE(VectorBlocuri[j].tip,j);

                    if(VectorBlocuri[j].pLeg[1] != 0)
                    {
                        int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                        int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                        int cul = BLACK;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                            cul = RED;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                        setcolor(cul);
                        line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                        line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                    }

                    if(VectorBlocuri[j].pLeg[0] != 0)
                    {
                        int x_linie_bloc_anterior, y_linie_bloc_anterior;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                            {
                                x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                            }
                            else
                            {
                                x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                            }
                        int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                        DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                        setcolor(BLACK);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                    }
                    break;
            case 'd':
                    if(VectorBlocuri[j].pLeg[0] != 0)
                    {
                        int x_linie_bloc_anterior, y_linie_bloc_anterior;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                            {
                                x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                            }
                            else
                            {
                                x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                            }
                        int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                        DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                        setcolor(COLOR(219, 218, 232));
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                    }

                    if(VectorBlocuri[j].pLeg[1] != 0)
                    {
                        int x_linie_bloc_urmator_A = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                        int y_linie_bloc_urmator_A = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.x + 2;
                        int y_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.y + 2;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent_A,y_linie_pleaca_bloc_curent_A,x_linie_bloc_urmator_A,y_linie_bloc_urmator_A,COLOR(219, 218, 232));
                        setcolor(COLOR(219, 218, 232));
                        line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A-5, y_linie_bloc_urmator_A-5);
                        line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A+5, y_linie_bloc_urmator_A-5);
                    }

                    if(VectorBlocuri[j].pLeg[2] != 0)
                    {
                        int x_linie_bloc_urmator_F = (VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2;
                        int y_linie_bloc_urmator_F = VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent_F = VectorBlocuri[j].stup.x + 2;
                        int y_linie_pleaca_bloc_curent_F = VectorBlocuri[j].drdown.y + 2;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent_F,y_linie_pleaca_bloc_curent_F,x_linie_bloc_urmator_F,y_linie_bloc_urmator_F,COLOR(219, 218, 232));
                        setcolor(COLOR(219, 218, 232));
                        line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F-5, y_linie_bloc_urmator_F-5);
                        line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F+5, y_linie_bloc_urmator_F-5);
                    }

                    DESEN_BLOC_INTRODUCERE(VectorBlocuri[j].tip,j);

                    if(VectorBlocuri[j].pLeg[0] != 0)
                    {
                        int x_linie_bloc_anterior, y_linie_bloc_anterior;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                            if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                            {
                                x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                            }
                            else
                            {
                                x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                                y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                            }
                        int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                        int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                        DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                        setcolor(BLACK);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                        line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                    }

                    if(VectorBlocuri[j].pLeg[1] != 0)
                    {
                        int x_linie_bloc_urmator_A = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                        int y_linie_bloc_urmator_A = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.x + 2;
                        int y_linie_pleaca_bloc_curent_A = VectorBlocuri[j].drdown.y + 2;
                        int cul = BLACK;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                            cul = RED;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent_A,y_linie_pleaca_bloc_curent_A,x_linie_bloc_urmator_A,y_linie_bloc_urmator_A,cul);
                        setcolor(cul);
                        line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A-5, y_linie_bloc_urmator_A-5);
                        line(x_linie_bloc_urmator_A, y_linie_bloc_urmator_A, x_linie_bloc_urmator_A+5, y_linie_bloc_urmator_A-5);
                    }

                    if(VectorBlocuri[j].pLeg[2] != 0)
                    {
                        int x_linie_bloc_urmator_F = (VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2;
                        int y_linie_bloc_urmator_F = VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y - 2;
                        int x_linie_pleaca_bloc_curent_F = VectorBlocuri[j].stup.x + 2;
                        int y_linie_pleaca_bloc_curent_F = VectorBlocuri[j].drdown.y + 2;
                        int cul = BLACK;
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[2]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[2]].pLeg[3] == j)
                            cul = RED;

                        DeseneazaLinie(x_linie_pleaca_bloc_curent_F,y_linie_pleaca_bloc_curent_F,x_linie_bloc_urmator_F,y_linie_bloc_urmator_F,cul);
                        setcolor(cul);
                        line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F-5, y_linie_bloc_urmator_F-5);
                        line(x_linie_bloc_urmator_F, y_linie_bloc_urmator_F, x_linie_bloc_urmator_F+5, y_linie_bloc_urmator_F-5);
                    }
                break;
            case 'a':
                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,COLOR(219, 218, 232));
                    setcolor(COLOR(219, 218, 232));
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }

                DESEN_BLOC_INTRODUCERE(VectorBlocuri[j].tip,j);

                if(VectorBlocuri[j].pLeg[1] != 0)
                {
                    int x_linie_bloc_urmator = (VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2;
                    int y_linie_bloc_urmator = VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y - 2;
                    int x_linie_pleaca_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_pleaca_bloc_curent = VectorBlocuri[j].drdown.y + 2;
                    int cul = BLACK;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[1]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[1]].pLeg[3] == j)
                        cul = RED;

                    DeseneazaLinie(x_linie_pleaca_bloc_curent,y_linie_pleaca_bloc_curent,x_linie_bloc_urmator,y_linie_bloc_urmator,cul);
                    setcolor(cul);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator-5, y_linie_bloc_urmator-5);
                    line(x_linie_bloc_urmator, y_linie_bloc_urmator, x_linie_bloc_urmator+5, y_linie_bloc_urmator-5);
                }

                if(VectorBlocuri[j].pLeg[0] != 0)
                {
                    int x_linie_bloc_anterior, y_linie_bloc_anterior;
                    if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[1] == j)
                    {
                        x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x + 2;
                        y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                    }
                    else
                        if(VectorBlocuri[VectorBlocuri[j].pLeg[0]].tip == 'd' && VectorBlocuri[VectorBlocuri[j].pLeg[0]].pLeg[2] == j)
                        {
                            x_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x + 2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y + 2;
                        }
                        else
                        {
                            x_linie_bloc_anterior = (VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.x +  VectorBlocuri[VectorBlocuri[j].pLeg[0]].stup.x)/2;
                            y_linie_bloc_anterior = VectorBlocuri[VectorBlocuri[j].pLeg[0]].drdown.y +2;
                        }
                    int x_linie_vine_bloc_curent = (VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2;
                    int y_linie_vine_bloc_curent = VectorBlocuri[j].stup.y - 2;

                    DeseneazaLinie(x_linie_bloc_anterior,y_linie_bloc_anterior,x_linie_vine_bloc_curent,y_linie_vine_bloc_curent,BLACK);
                    setcolor(BLACK);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent-5, y_linie_vine_bloc_curent-5);
                    line(x_linie_vine_bloc_curent, y_linie_vine_bloc_curent, x_linie_vine_bloc_curent+5, y_linie_vine_bloc_curent-5);
                }
            break;
            }
        }
}

void AfisareStructuraBloc()
{
    for(int j=1; j<i; j++)
    {
        cout<<"Nod: "<<j<<" Tip: "<<VectorBlocuri[j].tip<<" Coordonate: "<< VectorBlocuri[j].stup.x <<' '<<VectorBlocuri[j].stup.y<<"   "<<VectorBlocuri[j].drdown.x<<' '<<VectorBlocuri[j].drdown.y<<'\n';
        cout<<"Legaturi: "<<VectorBlocuri[j].pLeg[0]<<"  "<<VectorBlocuri[j].pLeg[1]<<' '<<VectorBlocuri[j].pLeg[2]<<' '<<VectorBlocuri[j].pLeg[3]<<'\n';
        if (VectorBlocuri[j].tip == 'i' || VectorBlocuri[j].tip == 'o' || VectorBlocuri[j].tip == 'a' || VectorBlocuri[j].tip == 'd')
            cout<<"Text: "<< VectorBlocuri[j].text <<"\n\n";
        else
            cout<<'\n';
    }
}

void DecizieFsauA(int j, int &poz)
{
    setfillstyle(1, COLOR(0,255,100));
    setbkcolor(COLOR(0,255,100));

    bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
    setcolor(BLACK);
    outtextxy(1310,inaltime_fereastra-300,"Falsa");
    rectangle(1222,inaltime_fereastra-318,1478,inaltime_fereastra-262);

    bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
    setcolor(BLACK);
    outtextxy(1285,inaltime_fereastra-380,"Adevarata");
    rectangle(1222,inaltime_fereastra-398,1478,inaltime_fereastra-342);

    setbkcolor(COLOR(219, 218, 232));

    while(1)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);
            if(x>=1220 && x<=1480 && y>=inaltime_fereastra-400&& y<=inaltime_fereastra-340)
            {
                poz = 1;
                setfillstyle(1, COLOR(0,255,85));
                bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
                bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
                break;
            }
            if(x>=1220 && x<=1480 && y>=inaltime_fereastra-320&& y<=inaltime_fereastra-260)
            {
                poz = 2;
                setfillstyle(1, COLOR(0,255,85));
                bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
                bar(1220,inaltime_fereastra-320,1480,inaltime_fereastra-260);
                break;
            }
        }
    }
}

bool Verificare_Existenta_Trasare_Linii()
{
    if(i==2 && VectorBlocuri[1].tip != 's')
        return false;

    int j, nr=0, ok=0;
    for (j=2; j<=i-1; j++)
        if(VectorBlocuri[j].pLeg[0] == 0 || VectorBlocuri[j].tip == 'd' && VectorBlocuri[j].pLeg[3] == 0)
        {
            nr = j;
            ok++;
            break;
        }

    for(j=1; j<=i-1; j++)
        if(VectorBlocuri[j].pLeg[1] == 0 && VectorBlocuri[j].tip != 'd' && nr!=j)
        {
            ok++;
            break;
        }
        else
            if((VectorBlocuri[j].pLeg[1] == 0 && VectorBlocuri[j].tip == 'd' || VectorBlocuri[j].pLeg[2] == 0 && VectorBlocuri[j].tip == 'd') && nr!=j && VectorBlocuri[j].tip != 't')
            {
                ok++;
                break;
            }
    if(ok==2)
        return true;
    return false;
}

void TraseazaLegatura()
{

    bool click = false;
    int nr, xMouse, yMouse, j, poz=1;
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            Beep(2000,100);
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
        }
            for (j=1; j<=i; j++)
                if (VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse && VectorBlocuri[j].tip!= 't')
                {
                    if( VectorBlocuri[j].tip == 'd')
                        DecizieFsauA(j,poz);
                    if(VectorBlocuri[j].pLeg[poz] == 0)
                    {
                        click=true;
                        nr=j;
                        break;
                    }
                }

    }
    while (!click);

    click=false;

    int x1,y1,x2,y2;
    if(VectorBlocuri[nr].tip != 'd')
    {
        x1 = (VectorBlocuri[nr].drdown.x+VectorBlocuri[nr].stup.x)/2;
        y1 = VectorBlocuri[nr].drdown.y+2;
    }
    else
    {
        y1 = VectorBlocuri[nr].drdown.y+2;
        if(poz == 1)
            x1 = VectorBlocuri[nr].drdown.x+2;
        else
            x1 = VectorBlocuri[nr].stup.x+2;
    }

    int este_bucla = 0;
    int k;
    do
    {
        xMouse=mousex();
        yMouse=mousey();
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            Beep(1200,100);
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            for (j=2; j<=i; j++)
                if (VectorBlocuri[j].stup.x<=xMouse && VectorBlocuri[j].stup.y <= yMouse && VectorBlocuri[j].drdown.x >= xMouse && VectorBlocuri[j].drdown.y >= yMouse)
                {
                    if( j!=nr)
                    {
                        if( VectorBlocuri[j].pLeg[0] == 0)
                        {
                            click = true;
                            VectorBlocuri[j].pLeg[0] = nr;
                            VectorBlocuri[nr].pLeg[poz] = j;
                        }
                        else
                            if( VectorBlocuri[j].pLeg[3] == 0 && VectorBlocuri[j].tip == 'd')
                            {
                                este_bucla = 1;
                                click = true;
                                VectorBlocuri[j].pLeg[3] = nr;
                                VectorBlocuri[nr].pLeg[poz] = j;
                                k = j;
                            }
                        xMouse = (VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2;
                        yMouse = VectorBlocuri[j].stup.y-2;
                    }
                }
        }
    }while(!click);

    if(!este_bucla)
    {
        DeseneazaLinie(x1,y1,xMouse,yMouse, BLACK);
        setcolor(BLACK);
        line(xMouse,yMouse,xMouse-5,yMouse-5);
        line(xMouse,yMouse,xMouse+5,yMouse-5);
    }
    else
    {
        DeseneazaLinie(x1,y1,xMouse,yMouse,RED);
        setcolor(RED);
        line(xMouse,yMouse,xMouse-5,yMouse-5);
        line(xMouse,yMouse,xMouse+5,yMouse-5);
    }
}

int VerificareExistentaVariabila(char text[],int j)
{
    while(j>1)
    {
        if(VectorBlocuri[j].tip== 'i' && strcmp(text, VectorBlocuri[j].text)==0)
            return j;
        j = VectorBlocuri[j].pLeg[0];
    }
    return 0;
}

float Evaluare(char text[],int nr)
{
    functie F;
    int k=1;
    int contor;
    float valoare=0;
    char txt[50];
    strcpy(F.vect[0],"(");
    strcpy(F.expresie,"(");
    for(int j=0; j<strlen(text); j++)
    {
        if(strchr("=#^<>+-*/()",text[j]))
        {
            F.vect[k][0]=text[j];
            F.vect[k][1]='\0';
        }
        if(strchr("1234567890",text[j]))
        {
            int contor=0;
            while(strchr("1234567890",text[j]))
            {
                F.vect[k][contor]=text[j];
                j++;
                contor++;
            }
            F.vect[k][contor]='\0';
            j--;
        }
        if(('a'<=text[j]&&text[j]<='z')||('A'<=text[j]&&text[j]<='Z')||text[j]=='_')
        {
            int contor=0;
            while((('a'<=text[j]&&text[j]<='z')||('A'<=text[j]&&text[j]<='Z')||text[j]=='_')||strchr("1234567890",text[j]))
            {
                F.vect[k][contor]=text[j];
                j++;
                contor++;
            }
            F.vect[k][contor]='\0';
            itoa (VectorBlocuri[VerificareExistentaVariabila(F.vect[k],nr)].val,F.vect[k],10);
            j--;
        }
        k++;
        strcat(F.expresie,F.vect[k-1]);
    }
    strcpy(F.vect[k],")");
    strcat(F.expresie,")");
    F.lung=strlen(F.expresie);
    valoare=ValoareFunctie(F,0);
    return valoare;
}

void ExecutareSchema(int j)
{
    setfillstyle(1, COLOR(219, 218, 232));
    setbkcolor(COLOR(219, 218, 232));
    int poz=1;
    int Buclainf=0;
    bool gresit= false;
    char text[50],txt[50];
    char *p;
    while(VectorBlocuri[j].tip != 't' && !gresit)
    {
        j = VectorBlocuri[j].pLeg[poz];
        poz=1;
        switch(VectorBlocuri[j].tip)
        {
        case '\0':
            gresit=true;
            setbkcolor(COLOR(0,255,85));
            setcolor(BLACK);
            outtextxy(1210,inaltime_fereastra-460,"Nu exista un bloc de");
            outtextxy(1230,inaltime_fereastra-440,"stop!(cel putin)");
            setbkcolor(COLOR(219, 218, 232));
            break;
        case 't':
            break;
        case 'i':
            bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
            setcolor(BLACK);
            CitireSir(VectorBlocuri[j].v,1230, inaltime_fereastra-380);
            setfillstyle(1, COLOR(0,255,85));
            bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
            setfillstyle(1, COLOR(219, 218, 232));
            setbkcolor(COLOR(219, 218, 232));
            while(!(atof(VectorBlocuri[j].v)!=0.0 && strchr("0123456789",VectorBlocuri[j].v[0])))
            {
                setbkcolor(COLOR(0,255,85));
                setcolor(BLACK);
                outtextxy(1230,inaltime_fereastra-460,"Citeste un numar!");
                setbkcolor(COLOR(219, 218, 232));
                bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
                CitireSir(VectorBlocuri[j].v,1230, inaltime_fereastra-380);
                setfillstyle(1, COLOR(0,255,85));
                bar(1220,inaltime_fereastra-480,1480,inaltime_fereastra-420);
                bar(1220,inaltime_fereastra-400,1480,inaltime_fereastra-340);
                setfillstyle(1, COLOR(219, 218, 232));
            }
            VectorBlocuri[j].val= atof(VectorBlocuri[j].v);
            break;
        case 'o':
            if(VerificareExistentaVariabila(VectorBlocuri[j].text,j))
            {
                setbkcolor(COLOR(0,255,85));
                setcolor(BLACK);
                outtextxy(1230,inaltime_fereastra-460,"Rezultatul este: ");
                itoa (VectorBlocuri[VerificareExistentaVariabila(VectorBlocuri[j].text,j)].val,text,10);
                outtextxy(1260,inaltime_fereastra-420,text);
                setbkcolor(COLOR(219, 218, 232));
                delay(3000);
                setfillstyle(1, COLOR(0,255,85));
                bar(1200,inaltime_fereastra-500,1500,inaltime_fereastra-400);
            }
            else
            {
                setbkcolor(COLOR(0,255,85));
                setcolor(BLACK);
                outtextxy(1220,inaltime_fereastra-460,"Variabila nu exista!");
                DESEN_BLOC_IESIRE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + 10, VectorBlocuri[j].stup.y + 15,j,12);
                setbkcolor(COLOR(219, 218, 232));
                gresit = true;
            }
            break;
        case 'd':
            strcpy(txt,VectorBlocuri[j].text);
            p = strtok (txt," =#^<>+-*/()");
            while (p != NULL)
            {
                if(!strchr("1234567890",p[0]))
                {
                    if(!VerificareExistentaVariabila(p,j))
                    {
                        setbkcolor(COLOR(0,255,85));
                        setcolor(BLACK);
                        outtextxy(1205,inaltime_fereastra-460,"Variabile inexistente");
                        DESEN_BLOC_DECIZIE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + textwidth(VectorBlocuri[j].text)/2, VectorBlocuri[j].stup.y + textheight(VectorBlocuri[j].text),j,12);
                        setbkcolor(COLOR(219, 218, 232));
                        gresit = true;
                    }
                }
                p = strtok (NULL, " =#^<>+-*/()");
            }
            strcpy(txt,VectorBlocuri[j].text);
            if(!Evaluare(txt,j))
                poz=2;
            break;
        case 'a':
            int k=0;
            while(VectorBlocuri[j].text[k]!='<'&&VectorBlocuri[j].text[k+1]!='-')
                k++;
            strncpy(text,VectorBlocuri[j].text,k);
            text[k]='\0';
            strcpy(txt,VectorBlocuri[j].text);
            p = strtok (txt," =#^<>+-*/()");
            while (p != NULL)
            {
                if(!strchr("1234567890",p[0]))
                {
                    if(!VerificareExistentaVariabila(p,j))
                    {
                        setbkcolor(COLOR(0,255,85));
                        setcolor(BLACK);
                        outtextxy(1205,inaltime_fereastra-460,"Variabile inexistente");
                        DESEN_BLOC_ATRIBUIRE(VectorBlocuri[j].text, VectorBlocuri[j].stup.x + textwidth(VectorBlocuri[j].text) + 10, VectorBlocuri[j].stup.y + 15,j,12);
                        setbkcolor(COLOR(219, 218, 232));
                        gresit = true;
                    }
                }
                p = strtok (NULL, " =#^<>+-*/()");
            }
            strcpy(txt,VectorBlocuri[j].text+k+2);
            VectorBlocuri[VerificareExistentaVariabila(text,j)].val=Evaluare(txt,j);
            break;
        }
        Buclainf++;
        if(Buclainf==120)
        {
            setbkcolor(COLOR(0,255,85));
            setcolor(BLACK);
            outtextxy(1220,inaltime_fereastra-460,"Bucla infinita!");
            setbkcolor(COLOR(219, 218, 232));
            gresit=true;
            break;
        }
    }
    if(gresit)
    {
        delay(3000);
        setfillstyle(1, COLOR(0,255,85));
        bar(1200,inaltime_fereastra-500,1500,inaltime_fereastra-400);
        setbkcolor(COLOR(0,255,85));
        setcolor(BLACK);
        outtextxy(1220,inaltime_fereastra-460,"Schema incorecta!");
        setbkcolor(COLOR(219, 218, 232));
        delay(2000);
        bar(1200,inaltime_fereastra-500,1500,inaltime_fereastra-400);
        setfillstyle(1,COLOR(219, 218, 232));
    }
}


void SalvareSchema(char NumeFisier[],char txt[])
{
    FILE* fis;
    fis = fopen(NumeFisier, txt);
    for(int j=1; j<i; j++)
    {
        fprintf(fis,"%c %d %d %d %d %d %d %d %d",VectorBlocuri[j].tip,VectorBlocuri[j].stup.x,VectorBlocuri[j].stup.y,VectorBlocuri[j].drdown.x,VectorBlocuri[j].drdown.y,VectorBlocuri[j].pLeg[0],VectorBlocuri[j].pLeg[1],VectorBlocuri[j].pLeg[2],VectorBlocuri[j].pLeg[3]);
        if((VectorBlocuri[j].tip == 'i' || VectorBlocuri[j].tip == 'o' || VectorBlocuri[j].tip == 'a' || VectorBlocuri[j].tip == 'd'))
            fprintf(fis,"\n%s",VectorBlocuri[j].text);
        fprintf(fis,"\n");
    }
    if(strstr(NumeFisier,"BackupSave.txt"))
    {
        scontor++;
        saves[scontor] = ftell(fis);
    }

    fclose(fis);
}

void CitireSchemaDinFisier(long start,char NumeFisier[])
{
    FILE* fis;
    fis = fopen(NumeFisier, "r");
    fseek(fis,start,SEEK_SET);
    i=1;
    while(!feof(fis))
    {
        fscanf(fis, "%c %d %d %d %d %d %d %d %d\n",&VectorBlocuri[i].tip,&VectorBlocuri[i].stup.x,&VectorBlocuri[i].stup.y,&VectorBlocuri[i].drdown.x,&VectorBlocuri[i].drdown.y,&VectorBlocuri[i].pLeg[0],&VectorBlocuri[i].pLeg[1],&VectorBlocuri[i].pLeg[2],&VectorBlocuri[i].pLeg[3]);
        if((VectorBlocuri[i].tip == 'i' || VectorBlocuri[i].tip == 'o' || VectorBlocuri[i].tip == 'a' || VectorBlocuri[i].tip == 'd'))
            fscanf(fis,"%s\n",&VectorBlocuri[i].text);
    switch(VectorBlocuri[i].tip)
    {
    case 's':
        DESEN_BLOC_START(VectorBlocuri[i].stup.x + textwidth("START") + 10, VectorBlocuri[i].stup.y + 15,14);
        break;
    case 't':
        DESEN_BLOC_STOP(VectorBlocuri[i].stup.x + textwidth("STOP") + 10, VectorBlocuri[i].stup.y + 15,i,14);
        break;
    case 'i':
        DESEN_BLOC_INTRARE(VectorBlocuri[i].text,VectorBlocuri[i].stup.x + textwidth(VectorBlocuri[i].text) + 10, VectorBlocuri[i].stup.y + 15,i,14);
        break;
    case 'o':
        DESEN_BLOC_IESIRE(VectorBlocuri[i].text,VectorBlocuri[i].stup.x + textwidth(VectorBlocuri[i].text) + 10, VectorBlocuri[i].stup.y + 15,i,14);
        break;
    case 'd':
        DESEN_BLOC_DECIZIE(VectorBlocuri[i].text, VectorBlocuri[i].stup.x + textwidth(VectorBlocuri[i].text) + textwidth(VectorBlocuri[i].text)/2, VectorBlocuri[i].stup.y + textheight(VectorBlocuri[i].text),i,14);
        break;
    case 'a':
        DESEN_BLOC_ATRIBUIRE(VectorBlocuri[i].text,VectorBlocuri[i].stup.x + textwidth(VectorBlocuri[i].text) + 10, VectorBlocuri[i].stup.y + 15,i,14);
        break;
    }
        i++;
    }
    contor_blocuri=contor_blocuri-i+1;
    for(int j=1;j<i;j++)
    {
        if(VectorBlocuri[j].tip != 'd')
        {
            if(VectorBlocuri[j].pLeg[1]!=0)
            {
                DeseneazaLinie((VectorBlocuri[j].drdown.x+VectorBlocuri[j].stup.x)/2,VectorBlocuri[j].drdown.y+2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2, BLACK);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2-5,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2-5);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2+5,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2-5);
            }
        }
        else
        {
            if(VectorBlocuri[j].pLeg[1]!=0)
            {
                DeseneazaLinie(VectorBlocuri[j].drdown.x+2,VectorBlocuri[j].drdown.y+2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2, BLACK);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2-5,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2-5);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[1]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.x)/2+5,VectorBlocuri[VectorBlocuri[j].pLeg[1]].stup.y-2-5);
            }
            if(VectorBlocuri[j].pLeg[2]!=0)
            {
                DeseneazaLinie(VectorBlocuri[j].stup.x+2,VectorBlocuri[j].drdown.y+2,(VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y-2, BLACK);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2-5,VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y-2-5);
                line((VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2,VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y-2,(VectorBlocuri[VectorBlocuri[j].pLeg[2]].drdown.x + VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.x)/2+5,VectorBlocuri[VectorBlocuri[j].pLeg[2]].stup.y-2-5);
            }
            if(VectorBlocuri[j].pLeg[3]!=0)
            {
                DeseneazaLinie(VectorBlocuri[VectorBlocuri[j].pLeg[3]].stup.x+2,VectorBlocuri[VectorBlocuri[j].pLeg[3]].drdown.y+2,(VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2,VectorBlocuri[j].stup.y-2, BLACK);
                line((VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2,VectorBlocuri[j].stup.y-2,(VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2-5,VectorBlocuri[j].stup.y-2-5);
                line((VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2,VectorBlocuri[j].stup.y-2,(VectorBlocuri[j].drdown.x + VectorBlocuri[j].stup.x)/2+5,VectorBlocuri[j].stup.y-2-5);
            }
        }
    }
    AfisareStructuraBloc();
    fclose(fis);
}
void ParcurgereDecizie(int j)
{
    int poz=1;
    int aux=j;
    int Buclainf=0;
    j = VectorBlocuri[j].pLeg[poz];
    while(VectorBlocuri[j].tip != '\0'&&aux!=j)
    {
        poz = 1;
        if(aux!=j)
        {
            switch(VectorBlocuri[j].tip)
            {
            case 't':
                break;
            case 'i':
                fprintf(fisi,"\nfloat %s;\n",VectorBlocuri[j].text);
                fprintf(fisi,"cin>> %s;\n",VectorBlocuri[j].text);
                break;
            case 'o':
                fprintf(fisi,"\ncout<< %s;\n",VectorBlocuri[j].text);
                break;
            case 'd':
                if(VectorBlocuri[j].pLeg[3]!=0)
                    fprintf(fisi,"\nwhile( %s )\n{",VectorBlocuri[j].text);
                else
                    fprintf(fisi,"\nif( %s )\n{",VectorBlocuri[j].text);
                ParcurgereDecizie(j);
                poz=2;
                if(VectorBlocuri[j].pLeg[3]==0)
                    fprintf(fisi,"\n}\nelse\n{");
                else
                    fprintf(fisi,"\n}\n");
                break;
            case 'a':
                char txt[51];
                int k=0;
                for(int t=0; t<=strlen(VectorBlocuri[j].text)-1; t++)
                {
                    if(VectorBlocuri[j].text[t] != '<')
                    {
                       txt[k++]=VectorBlocuri[j].text[t];
                    }
                    else
                    {
                        txt[k++] = ' ';
                        txt[k++] = '=';
                        txt[k++] = ' ';
                        t++;
                    }
                }
                txt[k]='\0';
                fprintf(fisi,"\n%s;\n",txt);
                break;
            }
        }
        j = VectorBlocuri[j].pLeg[poz];
        Buclainf++;
        if(Buclainf==100)
            break;
    }
}

void TransformareSchemaInCod(int j)
{
    int poz=1;
    int els=0;
    char *p;
    while(VectorBlocuri[j].tip != '\0')
    {
        poz = 1;
        switch(VectorBlocuri[j].tip)
        {
        case 's':
            fprintf(fisi,"#include <iostream>\nusing namespace std;\n");
            fprintf(fisi,"\nint main()\n{");
            break;
        case 't':
            while(els)
            {
                fprintf(fisi,"\n}\n");
                els--;
            }
            break;
        case 'i':
            fprintf(fisi,"\nfloat %s;\n",VectorBlocuri[j].text);
            fprintf(fisi,"cin>> %s;\n",VectorBlocuri[j].text);
            break;
        case 'o':
            fprintf(fisi,"\ncout<< %s;\n",VectorBlocuri[j].text);
            break;
        case 'd':
            if(VectorBlocuri[j].pLeg[3]!=0)
                fprintf(fisi,"\nwhile( %s )\n{",VectorBlocuri[j].text);
            else
                fprintf(fisi,"\nif( %s )\n{",VectorBlocuri[j].text);
            ParcurgereDecizie(j);
            poz=2;
            if(VectorBlocuri[j].pLeg[3]==0)
            {
                fprintf(fisi,"\n}\nelse\n{");
                els++;
            }
            else
                fprintf(fisi,"\n}\n");
            break;
        case 'a':
            char txt[51];
            int k=0;
            for(int t=0; t<=strlen(VectorBlocuri[j].text)-1; t++)
            {
                if(VectorBlocuri[j].text[t] != '<')
                {
                   txt[k++]=VectorBlocuri[j].text[t];
                }
                else
                {
                    txt[k++] = ' ';
                    txt[k++] = '=';
                    txt[k++] = ' ';
                    t++;
                }
            }
            txt[k]='\0';
            fprintf(fisi,"\n%s;\n",txt);
            break;
        }
        j = VectorBlocuri[j].pLeg[poz];
    }
    fprintf(fisi,"\nreturn 0;\n}");
}

void Undo()
{
    FILE*fisaux,* fis;
    fis = fopen("BackupSave.txt", "r");
    fisaux = fopen("Auxiliar.txt", "w+");
    saves[scontor]=0;
    scontor--;
    char c;
    rewind(fis);
    while(ftell(fisaux)!=saves[scontor])
    {
        c = getc(fis);
        putc(c,fisaux);
    }
    fclose(fis);
    fclose(fisaux);
    remove("BackupSave.txt");
    rename("Auxiliar.txt","BackupSave.txt");
}

void Interfata_Aplicatie()
{
    Meniu();
    bool Sfisier=false;
    bool linie_apare = true;
    while(1 || afis_linie_titlu_meniu)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);
            if(x>=850 && x<=1075 && y>=inaltime_fereastra/2-30 && y<=inaltime_fereastra/2+30) // Oprire
                exit(1);
            else if(x>=682 && x<=849 && y>=inaltime_fereastra/2-30 && y<=inaltime_fereastra/2+30) // Schema din fisier
            {
                Sfisier=true;
                afis_linie_titlu_meniu = false;
            }
            if((x>=430 && x<=681 && y>=inaltime_fereastra/2-30 && y<=inaltime_fereastra/2+30)||Sfisier) // Desenare
            {
                afis_linie_titlu_meniu = false;
                cleardevice();
                Meniu_Desen();
                if(Sfisier)
                {
                    CitireSchemaDinFisier(0,"SchemaSalvata.txt");
                    SalvareSchema("BackupSave.txt","w+");
                    Afis_Contor();
                }
                while(1)
                {
                    if (ismouseclick(WM_LBUTTONDOWN))
                    {
                        getmouseclick(WM_LBUTTONDOWN, x, y);
                        clearmouseclick(WM_LBUTTONDOWN);
                        if(x>=249 && x<=346 && y>=20 && y<=50)  // Start
                        {
                            if(VectorBlocuri[1].tip!='s')
                            {
                                VectorBlocuri[1].tip='s';
                                DeseneazaNod(VectorBlocuri[1].tip,i);
                                contor_blocuri--;
                                Afis_Contor();
                                SalvareSchema("BackupSave.txt","w+");
                            }
                            else
                            {
                                contor_start++;
                                if(contor_start == 5)
                                {
                                    initwindow(600,100," ");
                                    setbkcolor(BLACK);
                                    setcolor(RED);
                                    settextstyle(4,HORIZ_DIR,2);
                                    char anunt[] = "BRAVO! ITI PLACE SA APESI PE START!";
                                    outtextxy(300-textwidth(anunt)/2,50-textheight(anunt)/2,anunt);
                                }
                                setcolor(BLACK);
                                setbkcolor(COLOR(0,255,85));
                                outtextxy(1225, inaltime_fereastra-460, "Ai deja bloc START");
                                delay(3000);
                            }
                        }
                        if(x>=347&&x<=434 && y>=20&&y<=50)  // Stop
                        {
                            contor_blocuri--;
                            VectorBlocuri[i].tip='t';
                            DeseneazaNod(VectorBlocuri[i].tip,i);
                            Afis_Contor();
                            i++;
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=435&&x<=560 && y>=20&&y<=50)  // Intrare
                        {
                            contor_blocuri--;
                            VectorBlocuri[i].tip='i';
                            strcpy(VectorBlocuri[i].text,"INTRARE");
                            DeseneazaNod(VectorBlocuri[i].tip,i);
                            Afis_Contor();
                            i++;
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=561&&x<=672 && y>=20&&y<=50)  // Iesire
                        {
                            contor_blocuri--;
                            VectorBlocuri[i].tip='o';
                            strcpy(VectorBlocuri[i].text,"AFISARE");
                            DeseneazaNod(VectorBlocuri[i].tip,i);
                            Afis_Contor();
                            i++;
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=673&&x<=798 && y>=20&&y<=50)  // Decizie
                        {
                            contor_blocuri--;
                            VectorBlocuri[i].tip='d';
                            strcpy(VectorBlocuri[i].text,"DECIZIE");
                            DeseneazaNod(VectorBlocuri[i].tip,i);
                            Afis_Contor();
                            i++;
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=799&&x<=951 && y>=20&&y<=50)  // Atribuire
                        {
                            contor_blocuri--;
                            VectorBlocuri[i].tip='a';
                            strcpy(VectorBlocuri[i].text,"ATRIBUIRE");
                            DeseneazaNod(VectorBlocuri[i].tip,i);
                            Afis_Contor();
                            i++;
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=1220 && x<=1480 && y>=20&&y<=80) // Executare schema
                        {
                            fisi = fopen("Cod.txt", "w+");
                            TransformareSchemaInCod(1);
                            fclose(fisi);
                            ExecutareSchema(1);
                        }
                        if(x>=1220 && x<=1480 && y>=100 && y<=160) // Unire blocuri
                        {
                            if(Verificare_Existenta_Trasare_Linii())
                            {
                                TraseazaLegatura();
                                SalvareSchema("BackupSave.txt","a+");
                            }
                        }
                        if(x>=1220 && x<=1480 && y>=180 && y<=240) // Salvare schema
                        {
                            Beep(400,300);
                            SalvareSchema("SchemaSalvata.txt","w+");
                        }
                        if(x>=1220 && x<=1480 && y>=260&&y<=320) // Mutare Bloc
                        {
                            Miscare_Bloc();
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=1220 && x<=1480 && y>=inaltime_fereastra-240 && y<=inaltime_fereastra-180)  // Editare
                        {
                            Editare();
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(x>=1220 && x<=1480 && y>=inaltime_fereastra-160 && y<=inaltime_fereastra-100)  // Stergere
                        {
                            contor_blocuri++;
                            Stergere();
                            Afis_Contor();
                            STERGERE_BLOC_INTRODUCERE();
                            SalvareSchema("BackupSave.txt","a+");
                        }
                        if(scontor!=0) // Undo
                        {
                            setfillstyle(1, COLOR(0,255,85));
                            setbkcolor(COLOR(0,255,85));
                            setcolor(BLACK);
                            bar(1000,20,1100,50);
                            outtextxy(1022,27,"UNDO");
                            rectangle(1000,20,1100,50);
                            setbkcolor(COLOR(219, 218, 232));
                            if(x>=1000 && x<=1100 && y>=20 && y<=50)
                            {
                                Undo();
                                contor_blocuri = 100;
                                ResetareVectorBlocuri();
                                cleardevice();
                                Meniu_Desen();
                                CitireSchemaDinFisier(saves[scontor-1],"BackupSave.txt");
                                Afis_Contor();
                            }
                        }
                        if(x>=1220 && x<=1480 && y>=inaltime_fereastra-80 && y<=inaltime_fereastra-20)  // Inapoi la Meniu
                        {
                            scontor=0;
                            contor_blocuri = 100;
                            ResetareVectorBlocuri();
                            cleardevice();
                            afis_linie_titlu_meniu = true;
                            Interfata_Aplicatie();
                        }
                    }
                }
            }
        }
        if(linie_apare && afis_linie_titlu_meniu)
        {
            setfillstyle(1, COLOR(49, 52, 54));
            bar(1121,0.32*inaltime_fereastra+1,1178,0.32*inaltime_fereastra+10);
            setfillstyle(1, COLOR(88, 112, 128));
            bar(1120,0.32*inaltime_fereastra,1177,0.32*inaltime_fereastra+9);
            linie_apare = false;
            delay(500);
        }
        else
            if(!linie_apare && afis_linie_titlu_meniu)
            {
                setfillstyle(1, COLOR(204, 207, 207));
                bar(1120,0.32*inaltime_fereastra,1177,0.32*inaltime_fereastra+9);
                bar(1121,0.32*inaltime_fereastra+1,1178,0.32*inaltime_fereastra+10);
                linie_apare = true;
                delay(500);
            }
    }
}

int main()
{
    initwindow(1500, inaltime_fereastra, "InterSchem");
    Interfata_Aplicatie();
    getch();
    closegraph();
    return 0;
}
