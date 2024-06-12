#!/bin/bash
#๏ Per semplificare le operazioni di archiviazione e di gestione dei file contenenti le informazioni di log sul funzionamento di un determinato servizio, spesso si fa in modo di conservare su file distinti i log relativi a giorni differenti. In tal modo ogni file conterrà solo le informazioni relative ad un periodo di tempo, che è possibile archiviare o gestire con maggiore facilità rispetto ad un unico file di log molto ingombrante.
#๏ Se l'applicazione “myapp” produce un file di log in “/tmp/mylog”, allora applicare l'operazione di log rotation a quel file, significa spostare ogni volta il log “mylog” nel file “mylog.0” e creare un nuovo file vuoto denominato “mylog”. Per mantenere in archivio i file relativi alle ultime n turnazioni, prima di rinominare il log corrente si provvederà a     rinominare/spostare “mylog.n−1” in “mylog.n”, “mylog.n−2” in “mylog.n−1” e così via, fino a spostare “mylog.0” in “mylog.1”.
#๏ Si scriva uno script che esegua le operazioni descritte, avendo cura di comprimere i file di log archiviati (non quello corrente) in modo da risparmiare spazio sul filesystem.

if [[ $# -ne 3 ]]; then
	echo "Syntax: $0 <log_directory> <log_name> <retention>."
	exit 1
fi



exit 0
