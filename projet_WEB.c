#include <stdio.h>
#include <emscripten/emscripten.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
//j'ai souvent du mal a m'organiser dans mes journées , et je me perd souvent dans mes taches quotidienne
//donc l'objectif est de créer une TODOLIST en C
//l'objectif est de pouvoir gerer mes tache du quotidiens en les triant par orde de priorité avoir des état sur les tache si elle sont faite elle sont supprimer de la liste (ou je sais pas encore je noterai peut etre juste qu'elle sont faite avec le petit symbole)
//je vais le faire avec une liste chainer : la premiere tache a faire seras en tete de liste les tache les moins importante ou la moins prioritaire seras la derniere. dans chaque case de notre liste chainer on aura un titre / resumer de la tache en quelque mot , puis on aura un int qui désigneras l'ordre de priorité d'éxécution de la tache (de sorte a ce que si on veut en ajouter une nouvelle on puisse la placer ou l'on voudra), il y aura un état que je vais définir sois par un int ou un str je sais pas encore, avec peut etre un str avec la date du jours ou doit etre fait la tache, puis un pointeur suivant qui pointe vers la tache suivant.
//ce que l'on pourait faire c'est que a chaque fois qu'une tache cha,ge d'état la liste se modifie et si toute les tache sont considerer comme faite on suprrimerai la lise créerai une toute nouvelle
//on peut commencer.


typedef struct _tache{
    char titre_de_la_tache[100];
    char horaire[10];
    int priorite;
    char etat[15];
    time_t date_creation;
    struct _tache *suivant;
}tache;
EM_JS(void, js_clear_tasks, (), {
  Module.tasks = [];
});

EM_JS(void,js_push_task,(const char* titre, const char* horaire,int prio,const char*etat),{
    if(!Module.tasks) Module.tasks=[];
    Module.tasks.push({
        titre: UTF8ToString(titre),
        horaire: UTF8ToString(horaire),
        priorite:prio,
        etat:UTF8ToString(etat)
    });
});

EMSCRIPTEN_KEEPALIVE
void fill_tasks_from_c(tache* list) {
  js_clear_tasks();
  for (tache* c = list; c; c = c->suivant) {
    js_push_task(c->titre_de_la_tache, c->horaire, c->priorite, c->etat);
  }
}

//voici notre structure de liste chainer j'ai donc défini etat par un mot qui seras sois "fait" sois"en cours"

//créons la 1er fonction qui crée le debut de la liste chainer ici on veut juste ajouter les élement sans les trier
EMSCRIPTEN_KEEPALIVE
tache *creationtache(const char titre[], const char horaire[], int prio,const char etat[]){
    tache *nouvelletache=malloc(sizeof(tache));
    if (nouvelletache==NULL){
        printf(" Erreur de malloc\n");
    }
    
    strcpy(nouvelletache->titre_de_la_tache, titre);
    strcpy(nouvelletache->horaire, horaire);
    nouvelletache->priorite=prio;
    strcpy(nouvelletache->etat, etat);// definit comme à faire vue qu'elle vient d'etre implanter 
    nouvelletache->date_creation =time(NULL);
    nouvelletache->suivant=NULL;
    return nouvelletache;
}

/*tache *creation_listetache(int n){
    tache *listetache=NULL;
    tache *nouvelletache;
    char titre[100];
    char horaire[10];
    int prio;
    char etat[15];
    int i;
    for (i=0;i<n;i++){
        printf("TACHE %d:\n",i+1);//pour indiquer la 1er tache et a chaque itération il affichera tache 2 tache 3 ect 
        printf("TITRE: \n");
        scanf(" %[^\n]",titre);// LA le scanf va lire tout ce que l'utilisateur écriras 
        printf("HEURE DE LA TACHE:\n");
        scanf("%s",horaire);//il va lire l'horaire (15h)
        printf("PRIORITE:\n");
        scanf("%d",&prio);//la on utilise l'adresse parce que c'est de cette maniere que le scanf pourras trouver l'adresse de la valeuer de prio
        strcpy(etat, "à faire");// caar ça évite de passer par des pointeur et si on a besoin de modifier la chaine cela seras plus simple ainsi
        nouvelletache=creationtache(titre,horaire,prio,etat);//ici on appelle la fonction et on la lance avec ce que l'utilisateur écrira
        nouvelletache->suivant=listetache; // c'est ici qu'on fait pointer notre tache vers la liste de sorte qu'a chaque itération la chaine se forme bien 
        listetache=nouvelletache;// puis on met a jours la tete de liste pour que chaque element pointe bien vers la tete de liste 
    }
    return listetache;
}*/

EMSCRIPTEN_KEEPALIVE

tache* creationlistetache(tache* listetache ,const char* titre,const char* horaire,int prio){
    char etat[15];
    strcpy(etat,"à faire");
    tache* nouvelle=creationtache(titre,horaire,prio,etat);
    nouvelle->suivant=listetache;
    return nouvelle;
}


tache *creation_listetache(){
    tache *listetache=NULL;
    tache *nouvelletache;
    char titre[100];
    char horaire[10];
    int prio;
    char etat[15];
    int i;
    int c=0;
    do{
        printf("TACHE %d:\n",c+1);//pour indiquer la 1er tache et a chaque itération il affichera tache 2 tache 3 ect 
        printf("TITRE: \n");
        scanf(" %[^\n]",titre);// LA le scanf va lire tout ce que l'utilisateur écriras 
        printf("DUREE DE LA TACHE:\n");
        scanf("%s",horaire);//il va lire l'horaire (15h)
        printf("PRIORITE:\n");
        scanf("%d",&prio);//la on utilise l'adresse parce que c'est de cette maniere que le scanf pourras trouver l'adresse de la valeuer de prio
        strcpy(etat, "à faire");// caar ça évite de passer par des pointeur et si on a besoin de modifier la chaine cela seras plus simple ainsi
        nouvelletache=creationtache(titre,horaire,prio,etat);//ici on appelle la fonction et on la lance avec ce que l'utilisateur écrira
        nouvelletache->suivant=listetache; // c'est ici qu'on fait pointer notre tache vers la liste de sorte qu'a chaque itération la chaine se forme bien 
        listetache=nouvelletache;// puis on met a jours la tete de liste pour que chaque element pointe bien vers la tete de liste
        printf("tapez 1 pour ajouter une tache et 0 pour fermer la liste\n");
        scanf("%d",&i);
        c++;
    }while(i==1);    
    
        
    return listetache;
}

// parfait on  a créer une fonction permetant de créée une tache puis une 2eme permetant de répéter la 1er fonction en incluant les donnée partagé par l'utilisateur 


// maintenant on va créer une fonction qui permettra de trier les tache que l'on aura placer sans ordre de priorité, puis on créeras une fonction affiche 
EMSCRIPTEN_KEEPALIVE
int tailleliste(tache *listetache){
    int c=0;
    while(listetache!=NULL){
        c++;
        listetache=listetache->suivant;
    }
    return c;
}

tache *tripriorite(tache *listetache ){
    int n=tailleliste(listetache);
    tache *courant;
    tache *listetachetrie=NULL;
    while(n>=1){
       for(courant=listetache;courant!=NULL; courant=courant->suivant){
        if(courant->priorite==n){
            tache *temp=malloc(sizeof(tache));
            strcpy(temp->titre_de_la_tache , courant->titre_de_la_tache);
            strcpy(temp->horaire , courant->horaire);
            temp->priorite=courant->priorite;
            strcpy(temp->etat , courant->etat);
            temp->suivant=listetachetrie;
            listetachetrie=temp;
        }
       }
       n=n-1;
    }

    return listetachetrie; 
}
EMSCRIPTEN_KEEPALIVE
void supprimerliste(tache *listetache){
    tache *temp;
    while(listetache!=NULL){
        temp=listetache;
        listetache=listetache->suivant;
        free(temp);
    }
}

// créons une fonction qui va supprimer un une tache dans le cas ou j'en ai plus besoin ou que je ne véeux plus d'elle car elle est inutile 

EMSCRIPTEN_KEEPALIVE
tache *supprimertachet(tache *listetachetrie, char titre[]){
    tache *courant;
    tache *precedent=NULL;
    for(courant=listetachetrie ;courant!=NULL ; courant=courant->suivant){
        if (strcmp(courant->titre_de_la_tache, titre)==0){
            if(precedent==NULL){
                listetachetrie=courant->suivant;
            }
            precedent->suivant=courant->suivant;
            free(courant); 
        }
    }
    return listetachetrie;
}


/*void debug_list(tache *liste) {
    int i = 0;
    printf("=== DEBUG LISTE CHAINEE ===\n");
    while (liste != NULL && i < 100) {
        printf("TACHE %d | titre: %s | prio: %d | suivant: %p\n", 
               i, liste->titre_de_la_tache, liste->priorite, (void *)liste->suivant);

        if (liste == liste->suivant) {
            printf("⚠️  ERREUR : la tâche %d se pointe vers elle-même !\n", i);
            break;
        }
        liste = liste->suivant;
        i++;
    }
    if (i >= 100) {
        printf("⚠️  BOUCLE INFINIE OU LISTE TROP LONGUE DÉTECTÉE !\n");
    }
    printf("===========================\n");
}*/



/*tache *nettoyerListe(tache *listetachetrie){
    tache *courant, *suivant_temp;
    tache *precedent=NULL;
    tache
    for (courant=listetachetrie;courant!=NULL;courant=courant->suivant){
        if(difftime(time(NULL), courant->date_creation) >= 86400){
            if (precedent==NULL){
                listetachetrie=courant->suivant;
            }
            suivant_temp=courant->suivant;
            precedent->suivant=courant->suivant;
            free(courant);
            courant=suivant_temp;
        }
    }
    if (listetachetrie==NULL){
        printf("tout les élément on été supprimer");
    }
    return listetachetrie;
}*/
EMSCRIPTEN_KEEPALIVE
tache *nettoyerListe(tache *listetachetrie) {
    tache *courant = listetachetrie;
    tache *precedent = NULL;
    tache *suivant_temp;

    while (courant != NULL) {
        if (difftime(time(NULL), courant->date_creation) >= 86400) {
            suivant_temp = courant->suivant;

            if (precedent == NULL) {
                // Suppression en tête
                listetachetrie = suivant_temp;
            } else {
                precedent->suivant = suivant_temp;
            }

            free(courant);
            courant = suivant_temp;
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }

    if (listetachetrie == NULL) {
        printf("Tous les éléments ont été supprimés.\n");
    }

    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
tache *supprimertacheetat(tache *listetachetrie, char etat[]){
    tache *courant=listetachetrie;
    tache *precedent=NULL;
    while(courant!=NULL){
        if (strcmp(courant->etat, etat)==0){
            tache *a_suppr=courant;
            if(precedent==NULL){
                listetachetrie=courant->suivant;
                courant=listetachetrie;
            }
            else{
            precedent->suivant=courant->suivant;
            courant=courant->suivant;
            }
            free(a_suppr);
        }
        else{
            precedent=courant;
            courant=courant->suivant;
        }
       
    }
    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
void *afficher1tache(tache *listetachetrie){
    char titre_de_la_tache[100];
    char horaire[10];
    int priorite;
    char etat[15];
    time_t date_creation;
    char *date_str = ctime(&listetachetrie->date_creation);
    date_str[strcspn(date_str, "\n")] = '\0';
    printf("|  %s                 | %s      |    %d    |  %s        | %11lld          |\n", listetachetrie->titre_de_la_tache, listetachetrie->horaire,listetachetrie->priorite,listetachetrie->etat, listetachetrie->date_creation);
}




/*8DE CHAQUE COT2 TITRE , horaire 1 priorité 1*/
EMSCRIPTEN_KEEPALIVE
void afficherLISTETACHE(tache *listetache){
    tache *courant;
    int c=1;
    printf("===============================================================================\n");
    printf("  LISTE DES TACHES : \n");
    printf("===============================================================================\n");
    printf(" NUM |        TITRE        | DUREE | PRIORITE |    ETAT    | DATE DE CREATION \n");
    printf("-------------------------------------------------------------------------------\n");
    for (courant=listetache;courant!=NULL;courant=courant->suivant){
        char *date_str = ctime(&courant->date_creation);
        date_str[strcspn(date_str, "\n")] = '\0';
        printf(" %2d |  %s                 | %s      |    %d    |  %s        | %s          |\n", c, courant->titre_de_la_tache, courant->horaire,courant->priorite,courant->etat, date_str);
        c++;
    }
    printf("===============================================================================\n");

}

void afficherrandom(int x){
    int i=0;
    while(i<x){
        printf("%d, ahahah",i);
        i++;
    }
}

/*void afficherlisteTACHE(tache *listetache){
    int c=1;
    printf("===============================================================================\n");
    printf("  LISTE DES TACHES : \n");
    printf("===============================================================================\n");
    printf(" NUM |        TITRE        | HORAIRE | PRIORITE |    ETAT    |\n");
    printf("-------------------------------------------------------------------------------\n");
    while(listetache!=NULL){
        printf(" %d |  %s                 | %s      |    %d    |  %s          |\n", c, listetache->titre_de_la_tache, listetache->horaire,listetache->priorite,listetache->etat);
        c++;
        listetache=listetache->suivant;
    }
    printf("===============================================================================\n");
    printf("Voici votre liste de tache.\n");
}*/

EMSCRIPTEN_KEEPALIVE
tache *modifiertitre(tache *listetachetrie, char ancientitre[], char newtitre[] ){
    tache *courant=listetachetrie;
    

    while(courant!=NULL){
        if(strcmp(courant->titre_de_la_tache, ancientitre )==0){
            strcpy(courant->titre_de_la_tache, newtitre);
        }
        courant=courant->suivant;
    }
    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
tache *modifierhoraire(tache *listetachetrie,char titre[], char newhoraire[] ){
    tache *courant=listetachetrie;
    

    while(courant!=NULL){
        if(strcmp(courant->titre_de_la_tache, titre )==0){
            strcpy(courant->horaire, newhoraire);
        }
        courant=courant->suivant;
    }
    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
tache *modifierprio(tache *listetachetrie,char titre[],  int newprio ){
    tache *courant=listetachetrie;
    while(courant!=NULL){
        if(strcmp(courant->titre_de_la_tache, titre )==0){
            courant->priorite=newprio;
        }
        courant=courant->suivant;
    }
    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
tache *modifieretat(tache *listetachetrie, char titre[] , char newetat[] ){
    tache *courant=listetachetrie;
    

    while(courant!=NULL){
        if(strcmp(courant->titre_de_la_tache, titre )==0){
            strcpy(courant->etat, newetat);
        }
        courant=courant->suivant;
    }
    return listetachetrie;
}
EMSCRIPTEN_KEEPALIVE
void *modifutilisateur(tache *listetachetrie){
    int choix;
    char titre[100];
    char ancientitre[100];
    char newtitre[100];
    char newhoraire[10];
    char newetat[15];
    int newprio;
    
    printf("tapez '1' :Pour modifier le titre de la tache \n");
    printf("tapez '2' :Pour modifier la duree de la tache\n");
    printf("tapez '3' :Pour modifier la priorité de la tache\n");
    printf("tapez '4' :Pour modifier l'état de la tache\n");
    scanf("%d",&choix);
    if(choix!=1 && choix!=2 && choix!=3 &&choix!=4 ){
        printf("veuillez entré une valeur correcte selon votre demande (1 ,2 ,3, ou 4)\n");
    }
    if(choix==1){
        printf("Ecrivez l'ancien titre que vous souhaiter modifier:\n");
        scanf(" %[^\n]",ancientitre);
        printf("Entrez le nouveau TITRE:\n");
        scanf(" %[^\n]", newtitre);
        modifiertitre(listetachetrie,ancientitre,newtitre);
    }
    if(choix==2){
        printf("Ecrivez le titre de la tache que vous souhaiter modifier:\n");
        scanf(" %[^\n]",titre);
        printf("Entrez la nouvelle duree:\n");
        scanf(" %s", newhoraire);
        modifierhoraire(listetachetrie,titre,newhoraire);
    }
    if(choix==3){
        printf("Ecrivez le titre de la tache que vous souhaiter modifier:\n");
        scanf(" %[^\n]",titre);
        printf("Entrez le nouveau numéro de priorité:\n");
        scanf(" %d", &newprio);
        modifierprio(listetachetrie,titre,newprio);
    
    }
    if(choix==4){
        printf("Ecrivez le titre de la tache que vous souhaiter modifier:\n");
        scanf(" %[^\n]",titre);
        printf("Entrez l'état présent de la tache :\n");
        scanf(" %s", newetat);
        modifieretat(listetachetrie,titre,newetat);
    }
}
EMSCRIPTEN_KEEPALIVE
void *rechercher_des_tache(tache *listetachetrie, char etat[]){
    tache *courant =listetachetrie;
    for( courant=listetachetrie;courant!=NULL;courant=courant->suivant){
        if (strcmp(courant->etat,etat)==0){
            afficherLISTETACHE(courant);
        }
    }
}

EMSCRIPTEN_KEEPALIVE
void sauvegarderliste(tache *listetachetrie , const char *nomfichier){
    FILE *fichier=fopen(nomfichier, "w");
    if (fichier==NULL){
        printf("le fichier n'existe pas ou est impossible a ouvrir.");
        return;
    }
    int c=1;
        fprintf(fichier,"===============================================================================\n");
        fprintf(fichier,"📋  LISTE DES TÂCHES : \n");
        fprintf(fichier,"===============================================================================\n");
        fprintf(fichier," N° |        TITRE        | DUREE | PRIORITÉ |    ÉTAT    | DATE DE CREATION \n");
        fprintf(fichier,"-------------------------------------------------------------------------------\n");
    tache *courant;
    for(courant=listetachetrie;courant!=NULL;courant=courant->suivant){
        char *date_str = ctime(&courant->date_creation);
        date_str[strcspn(date_str, "\n")] = '\0';
        fprintf(fichier," %2d |  %s                 | %s      |    %d    |  %s        | %s          |\n", c, courant->titre_de_la_tache, courant->horaire,courant->priorite,courant->etat, date_str);
        c=c+1;
        fprintf(fichier,"===============================================================================\n");
    }
    fclose(fichier);
    printf("fichier créer et sauvegarder\n");
}

EMSCRIPTEN_KEEPALIVE
void sauvegarderbrut(tache *listetachetrie,const char *nomfichier){
    FILE *fichier=fopen(nomfichier,"w");
    if (fichier==NULL){
        printf("erreur\n");
        return;
    }
    tache *courant;
    for (courant=listetachetrie;courant!=NULL;courant=courant->suivant){
        fprintf(fichier,"%s;%s;%d;%s;%11lld;\n", courant->titre_de_la_tache,courant->horaire,courant->priorite,courant->etat,courant->date_creation);
    }
    fclose(fichier);
    printf("fichier brut créer\n");
}
EMSCRIPTEN_KEEPALIVE
tache *chargementBrut( const char *nomfichier){
    FILE *fichier=fopen(nomfichier, "r");
    if (fichier==NULL){
        printf("erreur\n");
        return NULL;
    }
    char ligne[400];
    tache *listesauvegarder=NULL;
    while(fgets(ligne,sizeof(ligne),fichier)){
        char *titre=strtok(ligne,";");
        char *horaire=strtok(NULL,";");
        char *priorite_str=strtok(NULL,";");//d'apres ce que j'ai compris on est entrain de lire et de decouper le fichier ligne par ligne afin d'extraire les donnée brut de notre fichier cela permettra de pas faire d'erreur et on utilise un 'char' pour priorite car strtok renvois les donnée en string(char) cela permettra de jjuste convertir "3"-> 3.
        char *etat=strtok(NULL,";");
        char *date_creation_str=strtok(NULL,"\n");
        //strtok va permettre de decouper des ligne ou des chaine de caractere (de l'endroit de debut à la fin ) ici ligne correspond au debut de la ligne et ; la fin de la ou on veux decouper et lorsque l'on a passer le premiere element découper on reprend sur la meme ligne en utilisant 'NULL' 
        if(titre && horaire && priorite_str && etat && date_creation_str){
            tache *new=malloc(sizeof(tache));
            if (new==NULL){
                printf("erreur d'allocation malloc\n");
                fclose(fichier);
                return NULL;
            }
            strcpy(new->titre_de_la_tache,titre);
            strcpy(new->horaire,horaire);
            new->priorite=atoi(priorite_str);//'atoi' permet de transformer un type char* en valeur entiere int si on utilise cela sur un mot tel que bonjour la fonction renverra 0
            strcpy(new->etat,etat);
            new->date_creation=(time_t)atol(date_creation_str);
            new->suivant=listesauvegarder;
            listesauvegarder=new;
        }
    }
    fclose(fichier);
    printf("chargement brut du fichier %s terminé.\n",nomfichier);
    return listesauvegarder;
}
EMSCRIPTEN_KEEPALIVE
void rechercherparmot(tache *listetachetrie, const char *mot){
    tache *courant;
    int trouver=0;
    
    printf("===============================================================================\n");
    printf(" Voici la liste de tache composer du mot: %s\n",mot);
    printf("===============================================================================\n");
    printf("|        TITRE        | DUREE | PRIORITÉ |    ÉTAT    | DATE DE CREATION \n");
    printf("-------------------------------------------------------------------------------\n");
    for(courant=listetachetrie;courant!=NULL;courant=courant->suivant){
        if(strstr(courant->titre_de_la_tache, mot)!=NULL){
            trouver=1;
            char *date_str = ctime(&courant->date_creation);
            date_str[strcspn(date_str, "\n")] = '\0';
            printf("|  %s                 | %s      |    %d    |  %s        | %s          |\n", courant->titre_de_la_tache, courant->horaire,courant->priorite,courant->etat,date_str);
        }
        
    }
    if(trouver==0){
        printf("le mot n'est pas présent dans la liste de tache, verifier l'orthographe du mot rechercher...\n");
    }
    printf("-------------------------------------------------------------------------------\n");
}
EMSCRIPTEN_KEEPALIVE
int maxprio(tache *listetache){
    tache* courant;
    if (listetache==NULL){
        return -1;
    }
    int max=listetache->priorite;
    for(courant=listetache;courant!=NULL;courant=courant->suivant){
        if(courant->priorite>max){
            max=courant->priorite;
        }
    }
    return max;
    
}
EMSCRIPTEN_KEEPALIVE
tache *newtrilistetache(tache *listetache){
    tache *listetachetrie=NULL;
    int p=maxprio(listetache);
    tache *courant;
    int i;
    for(i=p;i>0;i--){
        for(courant=listetache;courant!=NULL;courant=courant->suivant){
            if(courant->priorite==i){
                tache *newL=malloc(sizeof(tache));
                strcpy(newL->titre_de_la_tache,courant->titre_de_la_tache);
                strcpy(newL->horaire,courant->horaire);
                newL->priorite=courant->priorite;
                strcpy(newL->etat,courant->etat);
                newL->date_creation=courant->date_creation;
                newL->suivant=listetachetrie;
                listetachetrie=newL;
            }
        }
    }
    return listetachetrie;
}

EMSCRIPTEN_KEEPALIVE
int main(){
    tache *listetache=NULL;
    int choix;
    char nomfichier[100]="todolist.txt";
    int x;
    
    do{
        printf("============BIENVENUE DANS LE MENU DE VOTRE TODOLIST===========\n");
        printf("\n");
        printf("1 | CREER DES TACHES\n");
        printf("2 | AFFICHER LES TACHES\n");
        printf("3 | TRIER LES TACHE PAR PRIORITE\n");
        printf("4 | MODIFIER UNE TACHE\n");
        printf("5 | SUPPRIMER UNE TACHE PAR TITRE\n");
        printf("6 | SUPPRIMER TOUTE LES TACHE PAR UN ETAT DONNER\n");
        printf("7 | RECHERCHER UNE TACHE PAR MOT CLE\n");
        printf("8 | SAUVEGARDER LA LISTE DANS UN FICHIER\n");
        printf("9 | CHARGER UNE LISTE DEPUIS UN FICHIER\n");
        printf("0 | QUITTER.\n");
        printf("saisissez votre choix : ");
        scanf("%d",&choix);
        
        switch(choix){//comme des condition on pourrai traduire cela par 'cas 1' ect
            case 1:{
                listetache=creation_listetache();
                break;
            }
            case 2:{
                afficherLISTETACHE(listetache);
                break;
            }
            case 3:{
                listetache=newtrilistetache(listetache);
                printf("Tache triees par prioriter!\n");
                break;
            }
            case 4:{
                modifutilisateur(listetache);
                break;
            }
            case 5:{
                char titre[100];
                printf("Entrer le TITRE de la tache quevous souhaitez supprimer : ");
                scanf(" %[^\n]", titre);
                listetache=supprimertachet(listetache,titre);
                break;
            }
            case 6:{
                char etat[20];
                printf("veuillez entrer l'etat a supprimer : ");
                scanf(" %[^\n]",etat);
                listetache=supprimertacheetat(listetache,etat);
                break;
            }
            case 7:{
                char mot[100];
                printf("veuillez inscrire le mot clé a rechercher : ");
                scanf(" %[^\n]",mot);
                rechercherparmot(listetache,mot);
                break;
            }
            case 8:{
                sauvegarderliste(listetache,nomfichier);
                break;
            }
            case 9:{
                listetache=chargementBrut(nomfichier);
                break;
            }
            case 0:{
                printf("Fermeture de la TO-DO-LIST dans : \n");
                printf("3\n");
                printf("2\n");
                printf("1\n");
                printf("A BIENTOT !!!\n");
                break;
            }
            default:// s'execute si aucune case ne s'effectue (comme un else)
                printf("choix invalide,veuillez réessayer\n");
        }
    }while(choix!=0);
    supprimerliste(listetache);
    return 0;
}







    






 