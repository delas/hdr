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

Il database originario è stato costruito dal NIST (National Institute of Standards and Technology). Le immagini di partenza erano in bianco e nero, ma successivamente, al fine di normalizzarle alla dimensione di 20&times;20 pixel, sono stati introdotti dei livelli di luminosità intermedi, dovuti all'effetto di <em>anti-aliasing</em> del filtro per il ridimensionamento. Successivamente, sono state centrate, nel centro di massa dei pixel, in un'area di 28&times;28 px, al fine di migliorare il processo di apprendimento.

L'intero databse è memorizzato in due file: uno contenente l'insieme con tutti i dati, ed uno con tutte le etichetti; i valori aspettati corrisponderanno all'immagine nella stessa posizione: l'immagine <em>i</em>-esima (ovvero quella che corrisponde all'insieme di 28&times;28 byte posti ad <em>i</em>&times;(28&times;28) dall'inizio del file con gli esempi) avrà come valore aspettato il valore <em>i</em>-esimo (ovvero il valore posto ad <em>i</em> posizioni dall'inizio del file delle etichette). La codifica del file con le etichette, così come quella del file con le immagini è decisamente semplice da essere utilizzata: i dati sono stati salvati come fosse un <em>dump</em> (ad eccezione di un breve header contenente il numero di esempi campionati e, per le immagini, il numero di righe e di colonne) in modo da facilitare al massimo la loro lettura, una volta conosciuta la codifica. Il file delle etichette è costruito in questa maniera:

```
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000801(2049) magic number (MSB first)
0004     32 bit integer  60000            number of items
0008     unsigned byte   ??               label
0009     unsigned byte   ??               label
...
xxxx     unsigned byte   ??               label
```
Il file con gli esempi, invece, è strutturato secondo questo schema:

```
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000803(2051) magic number
0004     32 bit integer  60000            number of images
0008     32 bit integer  28               number of rows
0012     32 bit integer  28               number of columns
0016     unsigned byte   ??               pixel
0017     unsigned byte   ??               pixel
...
xxxx     unsigned byte   ??               pixel
```

## Soluzioni implementate

Come suggerito in [Russel03], si è sperimentalmente osservato che i risultati migliori si ottengono con una rete neurale costituita da un neurone in input per ciascun pixel dell'immagine da analizzare, un livello di neuroni nascosti con 300 unità e 10 neuroni in output (uno per ciascuna delle cifre in output).

Una rappresentazione grafica della rete che è stata modellata è riportata in Figura 2.

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/net.png)

Figura 2. Struttura della rete neurale.

Dato che le immagini sono codificate in scala di grigi, ciascun pixel è rappresentato dal livello di luminosità in quel determinato punto. L'input per il neurone è quindi facilmente realizzabile, semplicemente considerando il valore del pixel come un intero. Come già detto, successivamente, il livello di input è connesso a 300 neuroni nascosti i quali, a loro volta, sono tutti collegati a 10 neuroni di output, ciascuno dei quali rappresenta la probabilità che l'input fornito alla rete rappresenti la cifra in considerazione.

Entrambe le sperimentazioni tecnologiche del problema sono state fatte utilizzando le librerie FANN (Fast Artificial Neural Networks).

Le librerie FANN sono un software free e open source per lo sviluppo di reti neurali multilayer. Sono scritte in C, e sono pensate per supportare sia reti <em>fully connected</em>, sia <em>sparsely connected</em>, inoltre sono cross-platform e adatte all'uso sia di valori interi che in virgola mobile, questa libreria, inoltre, comprende un framework per la gestione semplificata dei training set. Esistono porting per questa libreria in molti linguaggi, fra i quali: PHP, C++, .NET, Ada, Python, Delphi, Octave, Ruby, Prolog Pure Data e Mathematica. D'ora in avanti, si farà sempre riferimento alla versione originale della libreria, scritta in C. La documentazione è molto ben strutturata ed esauriente, ed è suddivisa per package. &Egrave; correlata da un insieme di tutorial ed esempi che ne illustrano tutte le fasi del ciclo di vita, dall'installazione all'uso.

Gli algoritmi di apprendimento, che le librerie FANN implementano sono:
 * l'algoritmo &ldquo;standard&rdquo; Backpropagation incrementale;
 * l'algoritmo &ldquo;standard&rdquo; Backpropagation batch;
 * l'algoritmo RPROP;
 * l'algoritmo Quickprop.

### Rete unica

L'intero progetto è costituito da più software, ciascuno dei quali è orientato verso una attività specifica. In totale sono stati realizzati cinque programmi:
 * <tt>convert</tt>: programma realizzato al fine di convertire le immagini e le etichette in un formato accettabili per la libreria FANN. Questo, in effetti, è formato da una sequenza di valori separati da uno spazio; ciascun esempio è costituito da due righe: nella prima ci sono tutti i valori per le unità in input, nel secondo ci sono i valori dei neuroni di output;
 * <tt>train</tt>: programma che avvia la sessione di apprendimento per la rete. &Egrave; parametrizzato per vari aspetti, i più importanti dei quali consentono di decidere se continuare ad apprendere da una rete già esistente o se costruirne una nuova; il massimo numero di epoche per cui fare l'apprendimento; l'errore che si deve raggiungere prima di fermare la fase di learning (espresso come scarto quadratico medio). Sono state inoltre sfruttate alcune caratteristiche peculiari delle FANN, fra cui l'uso delle funzioni di callback tramite i puntatori a funzione, che sono risultate particolarmente utili per consentire la stampa dei dati di log ed il salvataggio della rete aggiornata ad ogni epoca;
 * <tt>test</tt>: questo programma consente di eseguire dei test con degli esempi presi dal test set. Ciascuna esecuzione stampa a terminale una rappresentazione della cifra letta, oltre ai valori delle unità di output. Si riporta, per facilitare la comprensione, un esempio di esecuzione del programma:
```
$ ./test

Test della Rete
===============

+----------------------------+
|                            |
|                            |
|                            |
|                            |
|               ..XXXXXXX.   |
|              XXXXXXXXX..   |
|            ..XXXX..        |
|           .XXXX            |
|          .XX..             |
|          XXX.              |
|         .XX.......         |
|         XXXXXXXXXX.        |
|         .XXXXXXXXXXX.      |
|            .    .XXXX      |
|                   XXX      |
|                  .XXX      |
|                  XXX.      |
|               ..XXX.       |
|     ...     .XXXXXX.       |
|     XXXX...XXXXXX..        |
|     .XXXXXXXXXXX..         |
|      XXXXXXXXXX.           |
|      .XXXXXXX..            |
|       ..XXX..              |
|                            |
|                            |
|                            |
|                            |
+----------------------------+
P(0) = 0.060623;
P(1) = 0.114797;
P(2) = 0.060783;
P(3) = 0.083447;
P(4) = 0.0134032;
P(5) = 0.802559;
P(6) = 0.0718292;
P(7) = 0.0376242;
P(8) = 0.029419;
P(9) = 0.0565661;


  *** CORRETTO ***

Esempio numero 35267
Aspettato: 5
Calcolato: 5
```
 * <tt>bulk-test</tt>: programma pensato per costruire delle statistiche sulla bontà della rete, ha il solo scopo di eseguire molte volte la rete, sottoponendole degli esempi pescati in maniera casuale dal test set, e calcolando la percentuale di valori trovati correttamente, confrontando il valore target con quello calcolato dalla rete;
 * l'ultimo programma sviluppato, denominato <tt>own-test</tt>, parte dall'idea di estendere l'uso della rete ad esempi costruiti &ldquo;al volo&rdquo;. Questo programma, infatti, prende come parametro della linea di comando il nome di un file immagine, che considererà come un esempio da sottoporre alla rete e ne stampa il risultato calcolato (per la lettura e la decodifica del file immagine, sono state usate le librerie DevIL). Tuttavia, come già spiegato in questo documento, gli esempi che la rete apprende sono stati centrati secondo il centro di massa dei pixel. Nella release attuale del programma non viene fatta tale operazione e si ritiene che sia questa la causa principale del numero di errori molto maggiore rispetto agli esempi provenienti dal test set.

Per effettuare l'apprendimento, si è osservato che l'algoritmo Backpropagation è molto più lento di Quickprop, tuttavia, quando l'errore comincia ad essere sufficentemente basso, il primo si comporta decisamente meglio del secondo. Per questo motivo, si è pensato di combinare i due e di utilizzare Quickprop per raggiungere con estrema rapidità dei primi risultati grezzi, che sono poi stati raffinati tramite Backpropagation.

L'algoritmo Quickprop, secondo quanto l'autore dello stesso riporta in [Fahlman88], si differenzia dal Backpropagation nella funzione che ricalcola i pesi. La nuova funzione, infatti, assume che la funzione errore sia una parabola e calcola, di conseguenza, la derivata seconda al fine di raggiungere rapidamente il minimo della parabola stessa. Il gradiente è calcolato in questa maniera (con <em>S</em> si fa riferimento alla derivata parziale):

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/quickprop.png)

### Reti multiple

&Egrave; stato provato anche un approccio differente al problema. Per ogni cifra si è costruito una rete a tre layer: un neurone di input per ogni pixel dell'immagine della cifra, da 20 a 80 neuroni nel layer nascosto e un unico neurone di output. La singola rete apprende a riconoscere la cifra associata: per esempio la rete associata alla cifra cinque apprende a restituire uno se la cifra in input è un cinque e zero altrimenti. Per riconoscere una cifra si calcola l'output delle diverse reti per una stessa immagine e la cifra riconosciuta è quella associata alla rete che ha l'output più alto.

Anche il software scritto per realizzare questo approccio è in C e fa uso della libreria FANN. I tool principali sono i seguenti:
 * <tt>convert.py</tt>: converte i database forniti dal NIST nel formato utilizzato dalla libreria FANN. Può eseguire la conversione secondo due modalità differenti. La prima produce un database (in formato FANN) che ha gli stessi esempi del database iniziale: gli input sono riscalati in modo da essere nell'intervallo [0,1], mentre l'output è il valore della cifra associata all'input. La seconda produce un database per ogni cifra nel modo seguente: seleziona tutti gli esempi di quella cifra dal database iniziale, seleziona altrettanti esempi scelti casualmente tra quelli delle altre cifre e li riordina sempre in modo casuale. In questo modo si ottiene un database che contiene la stessa quantità di esempi positivi e negativi di una data cifra. Gli input sono riscalati come sopra, mentre l'output è un singolo valore binario che indica se l'esempio è positivo o negativo. &Egrave; l'unico tool scritto in Python.
 * <tt>create</tt>: dati come argomenti un cammino, il numero di neuroni di input, il numero di neuroni nascosti e il numero di neuroni di output, scrive nel cammino dato una rete con pesi casuali e tre layer delle dimensioni indicate. La rete è forward completamente connessa e usa funzioni di attivazione sigmoidali.
 * <tt>train</tt>: dati come argomenti un file contenente una rete, un file contenente il training set, un file contenente il validation set e il numero di epoche da usare per l'apprendimento, fa apprendere alla rete il training set e sovrascrive la rete ottenuta al posto della rete iniziale. Durante l'esecuzione produce anche un log dell'apprendimento dove registra gli errori quadratici medi e la percentuale di esempi classificati correttamente relativi sia al training set che al validation set. Oltre alla rete ottenuta alla fine dell'apprendimento salva anche la rete che ha fatto meno errori sul test set, che non necessariamente è quella ottenuta alla fine dell'apprendimento.  L'apprendimento usa l'algoritmo di backpropagation incrementale con parametro 0,1.
 * <tt>test</tt>: dati come argomenti un file contenente una rete, un file contenente il test set e una tolleranza, produce in output delle statistiche riguardo alla correttezza dei risultati della rete sul test set e in particolare l'elenco degli esempi (visualizzati in ascii-art) in cui l'errore supera la tolleranza.
 * <tt>test-final</tt>: dati come argomenti un file contenente una rete che riconosce la cifra zero e un file contenente il test set (prodotto dalla prima modalità di <tt>convert.py</tt>), estrapola dal cammino del primo file il cammino delle reti che riconoscono le altre nove cifre, carica tutte le reti e le usa per riconoscere le cifre del test set nel modo seguente: calcola l'output di tutte le reti sull'immagine in input e classifica l'immagine come la cifra a cui è associata la rete che ha l'output maggiore.

Per ogni cifra si crea (con il programma <tt>create</tt>) una rete con 784 input, un opportuno numero di neuroni nascosti e un unico output. Ogni rete apprende con il programma <tt>train</tt> il database creato dalla seconda modalità di <tt>convert.py</tt> per un numero opportuno di epoche. A questo punto si può testare la bontà delle reti ottenute con <tt>test-final</tt>. Per automatizzare questo procedimento sono stati scritti anche alcuni shell script che combinano i tool di base.

# Risultati ottenuti

In questa sezione verranno commentati i risultati ottenuti in base alle due tecnologie che si ha avuto modo di sperimentare, includendo alcuni commenti personali sugli strumenti che si sono usati.

## Risultati con rete unica

Come già descritto pocanzi, nell'esempio che è stato realizzato tramite le librerie FANN sono stati utilizzati più algoritmi di apprendimento, alcune volte anche incrociandoli fra loro, al fine di migliorare le prestazioni. Si riportano, di seguito alcuni dei risultati raggiunti.

Tutti i vari test prevedevano il cambiamento solo dell'algoritmo di apprendimento, la struttura e la topologia della rete sono rimaste invariate. Anche il training set non è cambiato da ciascun test: esso è composto da un sottoinsieme del training set originario, in particolare contiene 30000 esempi, ovvero circa 3000 esempi per ciascuna delle cifre da apprendere.

In particolare, una rete, è stata allenata per un lungo periodo, nel provare diverse configurazioni. Tramite questa, è possibile raggiungere risultati che arrivano anche al 95/96% di esempi classificati correttamete. In [Russel03] si dice che, con questa struttura, si deve riuscire ad arrivare a risultati ancora maggiori: 98.4%. Data la brevità dei tempi di apprendimento accettati per questa sperimentazione, si pensa sia possibile raggiungere risultati come quelli promessi del citato articolo semplicemente aumentando il numero di epoche a disposizione dell'algoritmo. In [Russel03], infatti, il risultato è raggiunto con 7 giorni di apprendimento, e sfruttando l'intero training set.

### Algoritmo Backpropagation batch

Il primo test è stato fatto utilizzando l'algoritmo Backpropagation, nella sua variante <em>batch</em>, ovvero calcolando lo scarto quadratico dopo aver valutato tutti gli esempi.

<table class="data">
<tr>
<th>Esempi analizzati:</th>
<td>30000</td>
</tr>
<tr>
<th>Epoche apprendimento:</th>
<td>200</td>
</tr>
<tr>
<th>MSE raggiunto:</th>
<td>0.01315</td>
</tr>
<tr>
<th>Errori sul validation set:</th>
<td>2101 su 2500 = 84% classificati correttamente</td>
</tr>
<tr>
<th>Tempo apprendimento:</th>
<td>~1h 5m</td>
</tr>
</table>

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/fann_backprop_batch.gif)

### Algoritmo Backpropagation incrementale

Il secondo test è stato fatto utilizzando l'algoritmo Backpropagation, nella sua variante <em>incrementale</em>, ovvero calcolando lo scarto quadratico per ciascun esempio del training set.

<table class="data">
<tr>
<th>Esempi analizzati:</th>
<td>30000</td>
</tr>
<tr>
<th>Epoche apprendimento:</th>
<td>173</td>
</tr>
<tr>
<th>MSE raggiunto:</th>
<td>0.04699</td>
</tr>
<tr>
<th>Errori sul validation set:</th>
<td>1792 su 2500 = 71% classificati correttamente</td>
</tr>
<tr>
<th>Tempo apprendimento:</th>
<td>~17h</td>
</tr>
</table>

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/fann_backprop_incr.gif)

### Algoritmo Quickprop

Il terzo test è stato fatto utilizzando l'algoritmo Quickprop. Questo esempio si riferisce ad uno con <em>momentum</em> a 0,9.

<table class="data">
<tr>
<th>Esempi analizzati:</th>
<td>30000</td>
</tr>
<tr>
<th>Epoche apprendimento:</th>
<td>200</td>
</tr>
<tr>
<th>MSE raggiunto:</th>
<td>0.01094</td>
</tr>
<tr>
<th>Errori sul validation set:</th>
<td>2209 su 2500 = 88% classificati correttamente</td>
</tr>
<tr>
<th>Tempo apprendimento:</th>
<td>~1h 20m</td>
</tr>
</table>

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/fann_quickprop.gif)

### Algoritmi misti

Il quarto, ed ultimo, test è stato fatto utilizzando l'algoritmo Quickprop, per le prime 50 epoche, poi, per 150 epoche, è stato usato Backpropagation, nella sua variante <em>batch</em>.

<table class="data">
<tr>
<th>Esempi analizzati:</th>
<td>30000</td>
</tr>
<tr>
<th>Epoche apprendimento:</th>
<td>50 + 150</td>
</tr>
<tr>
<th>MSE raggiunto:</th>
<td>0.02107, 0.01300</td>
</tr>
<tr>
<th>Errori sul validation set:</th>
<td>2301 su 2500 = 92% classificati correttamente</td>
</tr>
<tr>
<th>Tempo apprendimento:</th>
<td>~20m + ~1h = ~1h 20m</td>
</tr>
</table>

![](https://raw.githubusercontent.com/delas/hdr/master/stuff/fann_mixed.gif)

## Risultati con reti multiple

Sono state testate reti con 20, 30, 40, 60, 80 e 120 neuroni nascosti. L'apprendimento è stato per tutte di sole 60 epoche perché già dopo qualche decina di epoche si notavano fenomeni di overfitting o comunque l'apprendimento non procedeva sensibilmente. L'algoritmo backpropagation incrementale, che non funzionava nell'approccio con rete unica, si è rivelato veloce e stabile. Questa differenza si può ipotizzare dipendere dalla maggiore semplicità della funzione che approssimiamo, dato che deve riconoscere una sola cifra alla volta.

L'apprendimento è stato veloce, grazie alla convergenza rapida:
<table class="data">
<tr>
<th>Neuroni nascosti</th>
<th>Durata apprendimento</th>
</tr>
<tr>
<td>20</td>
<td>14m 25s</td>
</tr>
<tr>
<td>40</td>
<td>28m 2s</td>
</tr>
<tr>
<td>60</td>
<td>41m 38s</td>
</tr>
<tr>
<td>80</td>
<td>58m 25s</td>
</tr>
<tr>
<td>120</td>
<td>94m 45s</td>
</tr>
</table>

La tabella seguente mostra la percentuale di errore raggiunta dalle singole reti nel riconoscere la cifra corrispondente al variare del numero di neuroni nascosti. Le percentuali sono a coppie: la prima è la rete ottenuta alla fine dell'apprendimento e la seconda è quella che durante l'apprendimento ha raggiunto la precisione migliore. Come si vede il risultato migliore non è sempre raggiunto dalla rete con più neuroni nascosti: alcune cifre sono riconosciute meglio da reti più piccole, mentre altre cifre non sono riconosciute efficacemente neanche dalle reti più grandi. Questo indica come la difficoltà di riconoscere una cifra fortemente dalla rappresentazione grafica della stessa.

<table class="data">
<tr>
<th>Cifra</th>
<th>20 fin/min</th>
<th>30 fin/min</th>
<th>40 fin/min</th>
<th>60 fin/min</th>
<th>80 fin/min</th>
<th>120 fin/min</th>
</tr>
<tr>
<th>0</th>
<td>2.20/1.55</td>
<td>1.66/1.50</td>
<td>1.71/1.61</td>
<td><strong>1.18/1.18</strong></td>
<td>1.71/1.71</td>
<td>2.63/1.61</td>
</tr>
<tr>
<th>1</th>
<td><strong>1.34/1.02</strong></td>
<td>1.39/1.44</td>
<td>1.39/1.06</td>
<td>1.25/1.11</td>
<td>1.39/1.20</td>
<td>1.34/1.39</td>
</tr>
<tr>
<th>2</th>
<td>3.93/3.57</td>
<td>3.11/3.11</td>
<td>3.36/3.16</td>
<td>3.36/3.36</td>
<td><strong>3.01/3.11</strong></td>
<td>3.47/3.21</td>
</tr>
<tr>
<th>3</th>
<td>5.10/4.63</td>
<td><strong>4.95/4.11</strong></td>
<td>6.09/5.52</td>
<td>5.21/4.58</td>
<td>4.89/5.15</td>
<td>4.79/4.37</td>
</tr>
<tr>
<th>4</th>
<td><strong>3.32/3.00</strong></td>
<td>3.32/3.27</td>
<td>3.37/3.05</td>
<td>3.59/3.59</td>
<td>4.13/3.21</td>
<td>3.43/3.48</td>
</tr>
<tr>
<th>5</th>
<td>4.48/4.48</td>
<td><strong>3.83/3.60</strong></td>
<td>3.89/4.07</td>
<td>4.31/4.43</td>
<td>4.66/3.89</td>
<td>4.31/4.48</td>
</tr>
<tr>
<th>6</th>
<td>2.81/2.70</td>
<td>2.59/2.48</td>
<td>3.03/2.81</td>
<td>3.08/2.86</td>
<td>3.14/3.08</td>
<td><strong>2.48/2.42</strong></td>
</tr>
<tr>
<th>7</th>
<td>2.92/2.92</td>
<td><strong>2.51/2.62</strong></td>
<td>3.39/3.39</td>
<td>2.98/2.98</td>
<td>3.69/3.13</td>
<td>3.08/3.03</td>
</tr>
<tr>
<th>8</th>
<td>6.93/6.65</td>
<td>7.03/6.82</td>
<td><strong>6.38/5.46</strong></td>
<td>7.47/6.98</td>
<td>6.60/6.22</td>
<td>7.14/6.82</td>
</tr>
<tr>
<th>9</th>
<td>5.91/6.12</td>
<td>6.60/6.23</td>
<td><strong>6.96/5.81</strong></td>
<td>6.23/6.02</td>
<td>7.07/7.17</td>
<td>6.12/5.81</td>
</tr>
</table>

La tabella seguente mostra la precentuale di errore raggiunta nel riconoscimento cifre usando dieci reti della stessa dimensione. Anche qui i risultati sono a coppie, un valore per le reti ottenute alla fine dell'apprendimento e una per quelle con l'errore minimo. La precisione migliore è raggiunta dalle dieci reti con 30 neuroni nascosti, quindi un numero di neuroni comparabili con quello usato nell'approccio a rete singola.

<table class="data">
<tr>
<th></th>
<th>20</th>
<th>30</th>
<th>40</th>
<th>60</th>
<th>80</th>
<th>120</th>
</tr>
<tr>
<th>Precisione</th>
<td>4.83</td>
<td>4.33</td>
<td>4.32</td>
<td>4.50</td>
<td>4.54</td>
<td>4.44</td>
</tr>
<tr>
<th>Precisione(min)</th>
<td>4.38</td>
<td><strong>4.09</strong></td>
<td>4.14</td>
<td>4.32</td>
<td>4.47</td>
<td>4.46</td>
</tr>
</table>

# Conclusioni

Per quanto riguarda la risoluzione del problema tramite le librerie FANN, l'esperienza è stata estremamente positiva: la potenza della tecnica delle Reti Neurali è stata addomesticata tramite queste librerie estremamente potenti e relativamente facili da utilizzare. È stato entusiasmante osservare sperimentalmente la piena riuscita della risoluzione del problema, tramite l'applicazione diretta delle tecniche teoriche viste durante il corso.

È stato possibile rendersi conto in maniera abbastanza rilevante della versatilità e delle potenzialità delle Reti Neurali e, dopo l'iniziale stupore sulla loro estrema efficacia si è cercato di capirne a fondo il loro funzionamento e le particolari sfumature degli algoritmi di apprendimento.

# Bibliografia

<dl>
<dt>[Russel03]</dt>
<dd>Stuart Russell, Peter Norvig. <em>Artificial Intelligence: A Modern Approach (second edition)</em>. Prentice Hall, 2003</dd>
<dt>[Fann]</dt>
<dd><em>Fast Artificial Neural Network Library (FANN)</em>. http://leenissen.dk/fann/</dd>
<dt>[Fahlman88]</dt>
<dd>Scott E. Fahlman. <em>Faster-learning variations on back-propagation: An empirical study</em>. In T. J. Sejnowski G. E. Hinton and D. S. Touretzky, editors, 1988 Connectionist Models Summer School, San Mateo, CA, 1988. Morgan Kaufmann.</dd>
<dt>[Mitchell97]</dt>
<dd>Thomas Mitchell. <em>Machine learning</em>. McGraw-Hill Education, 1997</dd>
<dt>[LeCun]</dt>
<dd>Yann LeCun, Corinna Cortes. <em>THE MNIST DATABASE of handwritten digits</em>. http://yann.lecun.com/exdb/mnist/</dd>
</dl>
