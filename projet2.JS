// ===================================
// Initialisation du module C
// ===================================
window.wasmReady= new Promise((resolve)=>{
    Module.onRuntimeInitialized = function() {
    // Fonctions C exportées via cwrap
        window.creationtache = Module.cwrap('creationtache','number',['string','string','number','string']);
        window.creationlistetache = Module.cwrap('creationlistetache','number',['number','string','string','number']);
        window.modifiertitre = Module.cwrap('modifiertitre','number',['number','string','string']);
        window.modifierprio = Module.cwrap('modifierprio','number',['number','string','number']);
        window.modifierhoraire = Module.cwrap('modifierhoraire','number',['number','string','string']);
        window.modifieretat = Module.cwrap('modifieretat','number',['number','string','string']);
        window.supprimertachet = Module.cwrap('supprimertachet','number',['number','string']);
        window.supprimertacheetat = Module.cwrap('supprimertacheetat','number',['number','string']);
        window.supprimerliste = Module.cwrap('supprimerliste','void',['number']);
        window.sauvegarderlistetache = Module.cwrap('sauvegarderliste','void',['number','string']);
        window.chargementtache = Module.cwrap('chargerbrut','number',['string']);
        window.fill_tasks_from_c = Module.cwrap('fill_tasks_from_c', 'void', ['number']);

        // Liste de tâches initialisée
        window.listetache = 0;
        resolve();
        afficherTachesJS();
    };
});

// ===================================
// Fonctions JS pour l’interface
// ===================================

// Ajouter une tâche via le formulaire
function creationTacheJS() {
    const titre = document.getElementById("titre").value;                       
    const horaire = document.getElementById("horaire").value;
    const prio = parseInt(document.getElementById("priorite").value);

    if (!titre || !horaire || isNaN(prio)) {
        alert("Veuillez remplir correctement tous les champs !");
        return;
    }

    listetache = creationlistetache(listetache, titre, horaire, prio);
    afficherTachesJS();

    // Réinitialiser le formulaire
    document.getElementById("titre").value = "";
    document.getElementById("horaire").value = "";
    document.getElementById("priorite").value = "";
}

// Affichage des tâches côté HTML
function afficherTachesJS() {
  const ul = document.getElementById("listeTaches");
  ul.innerHTML = "";

  // 1) Demander au C de remplir Module.tasks
  fill_tasks_from_c(listetache);

  // 2) Afficher le tableau JS rempli par le C
  const arr = Module.tasks || [];
  arr.forEach((t, i) => {
    const li = document.createElement("li");
    li.textContent = `#${i+1} - ${t.titre} | Durée: ${t.horaire} | Priorité: ${t.priorite} | État: ${t.etat}`;
    ul.appendChild(li);
  });

  // 3) (optionnel mais utile) mettre à jour la barre de progression
  Mise_a_jours_progression(arr);
}
function majProgression(arr) {
  const total = arr.length;
  const done = arr.filter(t => (t.etat || '').toLowerCase() === 'fait').length;
  const pct = total ? Math.round(done * 100 / total) : 0;

  const bar = document.getElementById('barre-remplie');
  const txt = document.getElementById('progress-txt');
  if (bar) bar.style.height = pct + '%';
  if (txt) txt.textContent = pct + '%';
}


// Exemple de fonction de suppression (appel C)
function supprimerTacheJS(titre) {
    listetache = supprimertachet(listetache, titre);
    afficherTachesJS();
}

// Exemple de modification d’état
function modifierEtatJS(titre, nouvelEtat) {
    listetache = modifieretat(listetache, titre, nouvelEtat);
    afficherTachesJS();
}
function modifierprioJS(titre,nouvelprio){
    const n=(typeof nouvelprio==='number')? nouvelprio:parseInt(nouvelprio,10);
    if(!Number.isInteger(n)||n<1){
        alert("priorité invalide(entier>=1)");
        return;
    }
    listetache=modifierprio(listetache,titre,n);
    afficherTachesJS();
}
function modifierhoraireJS(titre,nouvelduree){
    listetache=modifierhoraire(listetache,titre,nouvelduree);
    afficherTachesJS();
}
function modifiertitreJS(titre,nouveautitre){
    listetache=modifiertitre(listetache,titre,nouveautitre);
    afficherTachesJS();
}
// Sauvegarder la liste dans un fichier
function sauvegarderListeJS(nomFichier) {
    sauvegarderliste(listetache, nomFichier);
}

// Charger une liste depuis un fichier
function chargerListeJS(nomFichier) {
    listetache = chargementtache(nomFichier);
    afficherTachesJS();
}

document.addEventListener('DOMContentLoaded', () => {
  const f = document.getElementById('form-creer');
  if (f) f.addEventListener('submit', (e) => { e.preventDefault(); creationTacheJS(); });
});

window.progressMode='pondere';
const WEIGHTS ={1:3,2:2,3:1};
function progression(arr,mode=window.progressMode){
    if (!Array.isArray(arr)||arr.length===0)
        return 0;
    let sommetot=0, sommefait=0;
    for (const t of arr){//une boucle for t in arr
        const p=Number(t.priorite);//number() convertie ce qu'il a en nombre 
        const w=(Number.isFinite(p)&&WEIGHTS[p])? WEIGHTS[p]:1;
        sommetot=sommetot+w;
        if((t.etat||'').toLowerCase()==='fait')sommefait=sommefait+w;
    }
    return sommetot? Math.round((sommefait*100)/sommetot):0;
}

function Mise_a_jours_progression(arr){
    const pourcent=progression(arr);
    const bar=document.getElementById('barre-remplie');
    const txt=document.getElementById('progress-txt')||document.getElementById('progress-text');
    if (bar) bar.style.height=pourcent+'%';
    if(txt)txt.textContent=pourcent+'%';

}
