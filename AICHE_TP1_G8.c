#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <locale.h>

/***
chaque page il y 10 ligne
chaque ligne il y au max 80 -> 90 caractaire
*/
typedef struct info{
    int lig;
    int pag;
    int pos;
}info;

/****cette fonction va ecrire le fichier donner sur un forma facile a traiter */
void forma_text(FILE *fichier){
    FILE *new_f=fopen("forma_text.txt","w");
    int cpt,end_ligne;
    char c=fgetc(fichier);
    if(c>96)c-=32;
    while(c!=EOF){
        cpt=0;
        end_ligne=0;
        while (c!=EOF && end_ligne==0){
            fprintf(new_f,"%c",c);
            cpt++;
            c=fgetc(fichier);
            if(cpt>80 && c==' '){
                c=fgetc(fichier);
                end_ligne=1;
            }
        }
        if(c!='\n'){
        fprintf(new_f,"\n");}
    }
    fclose(new_f);
}

int nb_ligne(){
    FILE *f=fopen("forma_text.txt","r");
    char c =fgetc(f);
    int cpt=0;
    while(c!=EOF){
        if(c=='\n'){cpt++;}
        c=fgetc(f);
    }
    fclose(f);
    return cpt;
}

void affiche_ligne(FILE *f,int *nc,int *nm){
    char c = fgetc(f);
    while(c!=EOF && c !='\n'){
        printf("%c",c);
        c=fgetc(f);(*nc)++;
        if(c==' ')(*nm)++;
    }
}

void affiche_page(FILE *f,int page,int lig,int nl,int *nmt,int *nct){
    int ligne=page*20+1,nm=0,nc=0;
    printf("\n\n\n");
    while(lig<20){
        printf("     [%3d].  ",ligne);
        affiche_ligne(f,&nc,&nm);
        *nct+=nc;
        *nmt+=nm;
        lig++;
        ligne++;
        printf("\n");
    }
    printf("\n\n    nombre de caractères : %d /nombre de mots : %d / ligne de cette page : %d - %d / page : %d \n\n",nc,nm,ligne-20,ligne-1,page+1);
}

void display(){
    char next;
    int page=0,n,nl=nb_ligne();///nl :veux dire nombre de ligne
    int nmt=0,nct=0;///nmt : veux dire nombre de mots totale / nct : veux dire nombre de caractéres totale
    n=nl;
    FILE *f=fopen("forma_text.txt","r");
    while(nl>0){
        if(nl>20){affiche_page(f,page,0,n,&nmt,&nct);
        }else{affiche_page(f,page,20-nl,n,&nmt,&nct);}
        page++;
        nl-=20;
        system("pause");
        system(" cls ");
    }
    printf("\n\n\n\n\n\n    information sur le text :\n       Nombre de caractères totale : %d\n       nombre de mots totale : %d\n       nombre de lignes totale : %d\n       nombre de pages totale : %d \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",nct,nmt,n,page);
    system("pause");
    fclose(f);
}
///************************************************************************
///********************** partie rechercher et remplacer ******************
///************************************************************************

/// le vrai nom de cette function est search
/// j ai partager la fonction search_replace en 2 fonction 1..searsh  2..replace
void searsh(char mot[30],info t[100],int *n_ocu){
    FILE *f=fopen("forma_text.txt","r");
    char test[30],c;
    int cpt_ocu=0,cpt_lig=1,cpt_pos=1,i,j=0;
    c=fgetc(f);
    while(c!=EOF){
        i=0;
        while(c!=' ' && c != '\n' && c!= EOF){
            test[i]=c;
            c=fgetc(f);
            i++;
        }
        if(test[0]<91 && test[0]>64){test[0]+=32;}
        if(strcmp(test,mot)==0){
            cpt_ocu++;
            t[j].lig=cpt_lig;
            t[j].pag=(cpt_lig/20);
            t[j].pos=cpt_pos;
            j++;
        }
        if(c==' ' ){cpt_pos++;}
        if(c=='\n'){cpt_lig++;cpt_pos=0;}
        c=fgetc(f);i=0;
        while(i<30){test[i]='\0';i++;}
    }
    fclose(f);
    *n_ocu = cpt_ocu;
}

///nc : nombre de caractere du mot a remplacer
///n_ocu : nombre d' occurrence du mot rechercher
///mot : le mot
void replace(char mot[30],info t[100],int n_ocu,int nc){
    FILE *f1=fopen("forma_text.txt","r");
    FILE *f2=fopen("for.txt","w");
    int i=0,cpt_lig=1,cpt_pos=1,cpt=0;
    char c;
    while(i<n_ocu){
        cpt=0;
        if(cpt_lig==t[i].lig && cpt_pos==t[i].pos){
            while(nc>cpt){fgetc(f1);cpt++;}
            fprintf(f2,"%s",mot);
            i++;
        }
        c=fgetc(f1);
        fprintf(f2,"%c",c);
        if(c==' ' ){cpt_pos++;}
        if(c=='\n'){cpt_lig++;cpt_pos=0;}
    }
    fclose(f1);
    fclose(f2);
    remove("forma_text.txt");
    rename("for.txt","forma_text.txt");
}

void save_info(info t[100],int n_ocu,char nom1[30],char nom2[30]){
    FILE *f=fopen("rep_words.txt","a");
    int i;
    fprintf(f,"\nle mot ' %s ' est remplacer par ' %s ', ces cordonnees sont :\n",nom1,nom2);
    for(i=0;i<n_ocu;i++){
        fprintf(f," %d\.  la page : %d\tla ligne : %d\tla position : %d\n",i,t[i].pag,t[i].lig,t[i].pos);
    }
    fclose(f);
}

void give_all_info(){
    FILE *f=fopen("rep_words.txt","r");
    char c=fgetc(f);
    int i=0;
    while(c!=EOF){
        printf("%c",c);
        c=fgetc(f);
        if(c=='\n'){i++;}
        if(i==25){
            printf("\n\n\n");
            system("pause");
            system( "cls" );
            i=0;
        }
    }
}

void give_info(info t[100],int n_ocu){
    printf("\t\t coordonnees des mot trouver\n");
    int i;
    for(i=0;i<n_ocu;i++){
        printf(" %d\.  la page : %d\tla ligne : %d\tla position : %d\n",i,t[i].pag,t[i].lig,t[i].pos);
    }
}

void search_replace(){
    char mot_rech[30],mot_remp[30]; int n_ocu,i=0;
    printf("Enter le mot a rechercher : ");
    scanf("%s",&mot_rech);
    while(i<strlen(mot_rech)){
        if(mot_rech[i]<91 && mot_rech[i]>64){mot_rech[i]+=32;}
        i++;
    }
    info tab[100];
    searsh(mot_rech,tab,&n_ocu);
    printf("Enter le mot qui sera a la place du premier : ");
    scanf("%s",&mot_remp);
    while(i<strlen(mot_remp)){
        if(mot_remp[i]<91 && mot_remp[i]>64){mot_remp[i]+=32;}
        i++;
    }
    replace(mot_remp,tab,n_ocu,strlen(mot_rech));
    printf("\nSi vous voulez avoirdes inffrmation sur le mot remplacer entre '1' :");
    scanf("%d",&i);
    if(i==1){give_info(tab,n_ocu);}
    save_info(tab,n_ocu,mot_rech,mot_remp);
}

///*****************************************************************
///************************ partie cryptographie *******************
///*****************************************************************

int prem(long n){
    long i=2 , a=n/2 ;int premier=1;///1 veux dire que ce chifre est premier
    if(n==1){premier=0;
    }else{
        while(i<a && premier==1){
            if(n % i == 0){premier=0;}///0 veux dire que ce chifre n est pas premier
            i++;
        }
    }
    return premier;
}

///cette fuction veux dire que a est premier avec h
long aph(long  a,long h){
    long temp;
    while (1)
    {
        temp = a%h;
        if (temp == 0)
          return h;
        a = h;
        h = temp;
    }
}

long calcule_e(long phi){
    long e=2;
    while(e<phi){
        if(aph(e,phi)==1){
            break;
        }else{
        e++;
        }
    }
    return e;
}

long calcule_d(long e,long phi){
    long d=2;
    while(1){
        if((e*d)%phi==1){
            break;
        }else{
            d++;
        }
    }
    return d;
}

///ce module retourne m^e mod n
long calcule_puis(long m,long e,long n){
    long long m1=1;
    while(e>0){
        m1=m1*m;
        m1=m1%n;
        e--;
    }
    return m1;
}

void cle_RSA(long *n,long *e,long *d){
    long p,q,phi;
    while(1){
        printf("Entre le p et le q :\n");
        printf("             (*** LES DEUX NOMBRES DOIVE ETRE PLUS GRAND QUE 1 ***)\n");
        printf("p = ");
        while(1){
            scanf("%d",&p);
            if(prem(p)==1){break;}
            else{printf("EROR : %d n est pas un nombre premier\np = ",p);}
        }
        printf("q = ");
        while(1){
            scanf("%d",&q);
            if(prem(q)==1){break;}
            else{printf("EROR : %d n est pas un nombre premier\nq = ",q);}
        }
        *n=p*q;
        if(*n>300){break;
        }else{printf("Vous devez entrer des nombres 'p' et 'q' plus grand \n");}
    }
    phi=(q-1)*(p-1);
    *e=calcule_e(phi);
    *d=calcule_d(*e,phi);
}

void save_cle(){
    FILE *f=fopen("cle.txt","w");
    long n,d,e;
    cle_RSA(&n,&e,&d);
    fprintf(f,"%d %d %d",n,e,d);
    fclose(f);
}

long get_n(){
    FILE *f=fopen("cle.txt","r");
    char n[30],c=fgetc(f);
    int i=0;
    while(c!=' '&& c!=EOF){
        n[i]=c;
        c=fgetc(f);
        i++;
    }
    fclose(f);
    return atol(n);
}

long get_e(){
    FILE *f=fopen("cle.txt","r");
    char e[30],c=fgetc(f);
    int i=0;
    while(c!=' '){c=fgetc(f);}
    c=fgetc(f);
    while(c!=' '){
        e[i]=c;
        c=fgetc(f);
        i++;
    }
    fclose(f);
    return atol(e);
}

long get_d(){
    FILE *f=fopen("cle.txt","r");
    char d[30],c=fgetc(f);
    int i=0;
    while(c!=' '){c=fgetc(f);}
    c=fgetc(f);
    while(c!=' '){c=fgetc(f);}
    c=fgetc(f);
    while(c!=EOF){
        d[i]=c;
        c=fgetc(f);
        i++;
    }
    fclose(f);
    return atol(d);
}

long getnum(FILE *f){
    char c=fgetc(f),chaine[30];
    int i=0;
    if  ( c == EOF ){return -1;}
    else if(c==' ' ){return -2;}
    else if(c=='\n'){return -3;}
    else{
        while(c!='/'){
            chaine[i]=c;
            i++;
            c=fgetc(f);
        }
    return atol(chaine);
    }
}

void crypt2_RSA(FILE *f,long n,long e){
    FILE *f1=fopen("fich_cryp.txt","w");
    long c,m=fgetc(f);
    while(m!=EOF){
        if (  m == ' ' ){fprintf(f1, " ");}
        else if(m=='\n'){fprintf(f1,"\n");}
        else{

            c=calcule_puis(m,e,n);
            fprintf(f1,"%d/",c);
        }
        m=fgetc(f);
    }
    fclose(f1);
}

void decrypt2_RSA(long n,long d){
    printf("\n");
    FILE *f=fopen("fich_cryp.txt","r");
    FILE *f1=fopen("fich_decryp.txt","w");
    long m,c=getnum(f);
    while(c!=-1){
        if ( c == -2 ){fprintf(f1, " ");}
        else if(c==-3){fprintf(f1,"\n");}
        else{
            m=calcule_puis(c,d,n);
            fprintf(f1,"%c",m);
        }
        c=getnum(f);
    }
    fclose(f);
    fclose(f1);
}

///*******************************************************************************
///*******************************************************************************
///*************************** partie affichage **********************************
///*******************************************************************************
///*******************************************************************************


void menu(){
    int i=0;
    printf("**************************************************************************************************************");
    printf("**************************************************************************************************************");
    printf("***************************         ***   ***  ******  ***   **   **   **          ***************************");
    printf("***************************         **** ****  **      ****  **   **   **          ***************************");
    printf("***************************         ** *** **  ******  ** ** **   **   **          ***************************");
    printf("***************************         **  *  **  **      **  ****   **   **          ***************************");
    printf("***************************         **     **  ******  **   ***   *******          ***************************");
    printf("**************************************************************************************************************");
    printf("**************************************************************************************************************");

    printf("\n\n\n\n");

    printf("**************************************************************************************************************");
    printf("**************************************************************************************************************");
    printf("***************************                1. afficher le text                     ***************************");
    printf("***************************                2. remplacer un mot                     ***************************");
    printf("***************************                3. modifier les clefs                   ***************************");
    printf("***************************                4. crypter un fichier                   ***************************");
    printf("***************************                5. decrypter un fichier                 ***************************");
    printf("***************************                6. EXIT                                 ***************************");
    printf("**************************************************************************************************************");
    printf("**************************************************************************************************************");
}

void check_rep(FILE *f){
    int rep;
    while(1){
        system("cls");
        menu();
        while(1){
            printf("                                           ");
            scanf("%d",&rep);
            if(rep<7 && rep>0){break;}
            else{
                printf("EROR... vous devez choisir apartir du menu \n\n\n");
                system("pause");
                system("cls");
                menu();
            }
        }
           if   (rep == 1){
                system("cls");
                display();
        }else if(rep == 2){
            system("cls");
            printf("  ATENTION:  cette operation peux prendre des secondes\n");
            search_replace();
            system("pause");
            system("cls");
            printf("votre demende a ete bien effectue\n \nvoulez vous voir la liste de tout les mots qui on ete remplacer \n               si OUI entre '1' ");
            scanf("%d",&rep);
            if(rep==1){give_all_info();}
            system("pause");
        }else if(rep == 3){
            system("cls");
            printf("  ATENTION:  cette operation peux prendre des minutes \n");
            save_cle();
            system("cls");
            printf("votre demende a ete bien effectue \n\n\n\n\n\n\n\n\n\n\n\n");
            system("pause");
        }else if(rep == 4){
            system("cls");
            printf("  ATENTION:  cette operation peux prendre des minutes \n");
            crypt2_RSA(f,get_n(),get_e());
            system("cls");
            printf("votre demende a ete bien effectue \n\n\n\n\n\n\n\n\n\n\n\n");
            system("pause");
        }else if(rep == 5){
            system("cls");
            printf("  ATENTION:  cette operation peux prendre des minutes \n");
            decrypt2_RSA(get_n(),get_d());
            system("cls");
            printf("votre demende a ete bien effectue \n\n\n\n\n\n\n\n\n\n\n");
            system("pause");
        }else if(rep == 6){break;
        }
    }
}

int main()
{
    setlocale(LC_ALL,"");
    system ( " mode con cols=110 lines=28 " );
    char nom_f[30];
    FILE *f;
    while(1){
        printf(" \n\n\n\n\n\n\n\n\n\n\n\n\n                              Entre le nom du fichier : ");
        scanf("%s",&nom_f);
        f=fopen(nom_f,"r");
        if(f!=NULL){system("cls");break;}else printf("il n existe pas un fichier txt avec le nom donez \n");
        system("pause");
        system("cls");
    }
    forma_text(f);
    fclose(f);
    f=fopen(nom_f,"r");
    check_rep(f);
    fclose(f);
    return 0;
}
