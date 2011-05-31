# dieses Skript greift jeweils die $2ste Zeile alle $1 Zeilen aus dem Dokument $3 heraus. Dies ist nützlich, wenn man zum Beispiel die 50. Hypothese bei eine Ausgabe von jeweils 100 Hypothesen berechnen will. In diesem falle müssen $1 = 100 und $2 = 50 gewählt werden.
if [ $# -ne 3 ]; then
	echo "usage: <hypothesiscount> <pick hypothesis> <document>";
	exit 1;
fi;
awk '{if(NR % ('$1'+1) == '$2') print $0}' $3

