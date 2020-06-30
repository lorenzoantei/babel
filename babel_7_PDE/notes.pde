/*
* TO-DO //////////////////////////////////////////////////////////////////////////////
 . aggiungere una GUI seducente;
 . aggiungere suoni;
 . correggere duplicato variabile ciclo_randomEntry e ciclo_RandomEntry
 . dopo un certo livello, introdurre animazione life buffer overflow (ENTROPIA);
 
 **INTERAZIONE**
 I TASTI PRESENTI spostano cursori, muovono il testo, emettono suoni e riassegnano la randomEntry
 
 **COME SI OTTIENE UNA NUOVA RANDOMENTRY?**
 . interagendo con gli input fisici;
 . dopo tot tempo (se PIR rileva qualcuno presente;
 . raggiunto un certo livello di entropia;
 
 **COME SI RIDUCE L'ENTROPIA?**
 . se PIR non rileva nessuno vicino resetta dopo timeoutReset
 
 **cambiare stato output arduino**
 int entropy;
 entropy++;
 if (entropy == 200) {
 myPort.write('A');
 println(entropy);
 }
 if (entropy == 500) {
 myPort.write('B');
 println(entropy);
 }
 ****
 
 */
 
/*
#per aggiungere shutdown da processing

creare script 

sudo nano /usr/local/lib/processing-3.5.3/processing_sudo_halt.sh

#!/bin/bash
sudo halt

poi aggiungere al progetto processing

launch("processing_sudo_halt.sh");
*/
