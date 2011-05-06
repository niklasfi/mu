###Hallo ihr alle!

Ich habe das makefile jetzt etwas modifiziert. Ab sofort werden nur noch der zur datei x.cpp gehörende header x.h rekompiliert. wenn ihr jetzt also x.h ändert und y.h x.h einbindet, dann müsst ihr einmal make clean machen.

###zu den tests: 

* angenommen ihr habt dictionaryc.h und dictionaryc.cpp dann solltet ihr auch eine `dictionaryc.test.cpp` anlegen, in der ihr eure klasse mit einigen kleinen tests testet.
* Beim dictionary würde ich zum beispiel mal ein paar Wörter einfügen, und gucken ob diese richtig sortiert sind.
* Das macht ihr in einer main in dictionaryc.test.cpp. In eurem Programm bindet ihr `<assert.h>` ein um dann mit `assert( <Wahre Aussage> )` zu testen, ob alles geklappt hat.
* Jetzt fügt ihr dictionaryc.test.exe zu der variable `TESTS` im makefile hinzu, und
* führt `make tests` aus.
* Wenn alles ok ist, dann kommt am ende **#ok!**.
* Jetzt könnt ihr pushen, anderenfalls wäre es ideal, wenn ihr erstmal das programm repariert.


###HAVE FUN!	
