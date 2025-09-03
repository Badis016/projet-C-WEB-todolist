document.addEventListener("DOMContentLoaded", () => {
  const $ = (id) => document.getElementById(id);

  const form = $("form-creer");
  const btnCreerT = $("btn-creert");
  const btnCreerL = $("btn-creerl");
  let listMode = false;

  btnCreerT?.addEventListener("click", () => {
    listMode = false;
    form?.classList.remove("hidden");
    $("titre")?.focus();
  });

  btnCreerL?.addEventListener("click", () => {
    listMode = true;
    form?.classList.remove("hidden");
    $("titre")?.focus();
  });
  form?.addEventListener("submit", async (e) => {
    e.preventDefault();

  // Attendre que le module soit prêt
    if (window.wasmReady) {
      try { await window.wasmReady; } catch {}
  }

  // Re-teste après initialisation
    if (typeof window.creationlistetache !== "function") {
      alert("Le module n'est pas encore prêt (WASM).");
      return;
  }

  // créer la tâche
  creationTacheJS();

  
    if (!listMode) form?.classList.add("hidden");
  });

  

document.getElementById("btn-modit").addEventListener("click",()=>{
    const ancienTitre=prompt("Titre actuele de la tache a modifier :");
    const nouveauTitre=prompt("entrez le nouveau Titre:");
    if(ancienTitre&&nouveauTitre){
        modifiertitreJS(ancienTitre,nouveauTitre);
    }
});

document.getElementById(btn-modiet).addEventListener("click",()=>{
    const Titre=prompt("Titre de la tache a modifier :");
    const etat=prompt("entrez un des état suivant ('à Faire','En cours', 'Fait'):");
    if (Titre&&etat){
        modifierEtatJS(Titre,etat);
    }
});
document.getElementById("btn-modiprio").addEventListener("click", () => {
  const titre = prompt("Titre de la tâche ?");
  const nouvellePrio = parseInt(prompt("Nouvelle priorité (1, 2 ou 3) ?"));
  if (titre && nouvellePrio) {
    modifierprioJS(titre, nouvellePrio);
  }
});


document.getElementById("btn-modiduree").addEventListener("click", () => {
  const titre = prompt("Titre de la tâche ?");
  const nouvelleDuree = prompt("Nouvelle durée (minutes) ?");
  if (titre && nouvelleDuree) {
    modifierhoraireJS(titre, nouvelleDuree);
  }
});
  // Période (Jour / Semaine / Mois)
  const btnJ = $("btn-j"), btnS = $("btn-s"), btnM = $("btn-m");

  
  btnM?.addEventListener("click", () => {
    document.getElementById("progression")?.classList.add("hidden");

  });


  const showGlobal = () => {
    document.getElementById("progression")?.classList.remove("hidden");
    if (typeof window.afficherTachesJS === "function") afficherTachesJS();
  };
  btnJ?.addEventListener("click", showGlobal);
  btnS?.addEventListener("click", showGlobal);

  //  Supprimer / Sauvegarder / Charger 
  const btnDel  = $("btn-supp");
  const btnSave = $("btn-save");
  const btnLoad = $("btn-charg");

  btnDel?.addEventListener("click", () => {
    const t = prompt("Titre à supprimer ?");
    if (!t) return;
    supprimerTacheJS(t);
  });
  btnSave?.addEventListener("click", () => {
    const f = prompt("Nom de fichier de sauvegarde ?");
    if (!f) return;
    sauvegarderListeJS(f);
  });
  btnLoad?.addEventListener("click", () => {
    const f = prompt("Nom du fichier à charger ?");
    if (!f) return;
    chargerListeJS(f); 
  });

  (window.wasmReady ? window.wasmReady : Promise.resolve()).then(() => {
    if (typeof window.afficherTachesJS === "function") afficherTachesJS();
  });
});

/*form?.addEventListener("submit", (e) => {
    e.preventDefault();
    if (typeof window.creationlistetache !== "function") {
      alert("Le module n'est pas encore prêt (WASM).");
      return;
    }
    creationTacheJS();
    if (!listMode) form?.classList.add("hidden"); // en mode "créer une tâche" simple on referme
  });*/