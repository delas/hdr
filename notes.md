# Riconoscimento di cifre manoscritte tramite una rete neurale

### Sommario

Per il corso di Sistemi Intelligenti (prof. Sperduti, A.A. 2007/2008, Università degli Studi di Padova) è stato sperimentato ed implementato un sistema che consente il riconoscimento di cifre manoscritte. Le soluzioni sono state sviluppate tramite la tecnologia FANN in linguaggio C.

## Introduzione al problema

Il riconoscimento automatico di cifre manoscritte è un importante problema, che si può riscontrare in molte applicazioni pratiche. Alcuni esempi di applicazioni di questa tecnica:
 * sistemi di smistamento automatico della posta cartacea, basato sul riconoscimento del CAP scritto nelle buste;
 * inserimento automatico degli importi delle tasse letti dai bollettini;
 * riconoscimento automatico degli input per computer palmari.
 
Ci sono stati diversi progressi, negli ultimi anni, in questo settore, principalmente dovuti all'introduzione di nuovi algoritmi per l'apprendimento, la disponibilità di nuovi training set sui quali poter fare l'apprendimento e, non in ultima istanza, l'aumento della potenza di calcolo a disposizione sui computer moderni.

Sia per l'apprendimento, sia per la validazione dei dati di input è stato utilizzato un archivio di immagini molto conosciuto nel settore: &ldquo;<em>The MNIST database of handwritten digits</em>&rdquo;. Questo è costituito da un training set di 60000 esempi, più un test set di 10000. Un esempio di dati, così come sono memorizzati nel file degli esempi è riportato in Figura 1.

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/example_mnist.gif)

Figura 1. Esempio di dati estratti dal database MNIST.

Il database originario è stato costruito dal NIST (National Institute of Standards and Technology). Le immagini di partenza erano in bianco e nero, ma successivamente, al fine di normalizzarle alla dimensione di 20&times;20 pixel, sono stati introdotti dei livelli di luminosità intermedi, dovuti all'effetto di <em>anti-aliasing</em> del filtro per il ridimensionamento. Successivamente, sono state centrate, nel centro di massa dei pixel, in un'area di 28&times;28 px, al fine di migliorare il processo di apprendimento.
