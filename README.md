# Terrain Procedural Generation Project

Questo progetto è stato sviluppato come lavoro finale per il corso universitario di Informatica Grafica. Esso è un ampliamento del codice del prof (che implementava un sistema di illuminazione completo)
e implementa un sistema di generazione procedurale di terreni 3D utilizzando OpenGL, con l'obiettivo di creare ambienti realistici, dinamici e scalabili attraverso tecniche moderne di rendering.

---

## Descrizione

Il motore di rendering sviluppato permette la generazione, gestione e visualizzazione di terreni tridimensionali in tempo reale. Le principali funzionalità offerte dal sistema includono:

- Generazione procedurale del terreno tramite Fractal Perlin Noise
- Sistema di gestione a "chunk"
- Level of Detail in base alla distanza della camera
- Implementazione di una skybox
- Sistema di illuminazione completo: (già fornito dal codice del prof)
  - Luce ambientale
  - Luce direzionale
  - Illuminazione diffusa
  - Illuminazione speculare
- Normal mapping
- Effetto nebbia

---

## Requisiti

- Sistema operativo: Windows
- Librerie richieste (incluse nella cartella del progetto):
  - OpenGL
  - GLEW
  - FreeGLUT
  - GLM
  - Assimp

---

## Compilazione ed Esecuzione

Aprire il terminale nella cartella principale del progetto ed eseguire:

```bash
cd "src"
make
```

```bash
./progetto.exe
```

## Controlli

- **W / A / S / D**: movimento della camera
- **Mouse**: rotazione della visuale
- **] / [**: aumentare e diminuire la velocità della camera
- **Space**: reset della camera al punto di partenza
- **1-8**: modifiche dei livelli di luce
- **ESC**: uscita dal programma
