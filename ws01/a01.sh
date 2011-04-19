DIR="training"

ewc=`gunzip -c "$DIR/e" | wc -w`
fwc=`gunzip -c "$DIR/f" | wc -w`
elc=`gunzip -c "$DIR/e" | wc -l`
flc=`gunzip -c "$DIR/f" | wc -l`
echo "Anz lfd Wörter in e:"
echo $ewc
echo "And lfd Wörter in f:"
echo $fwc

echo "Größe des Vokabulars in e:"
gunzip -c "$DIR/e" | sed "s/ /\n/g" | sort | uniq | wc -l
echo "Größe des Vokabulars in f:"
gunzip -c "$DIR/f" | sed "s/ /\n/g" | sort | uniq | wc -l

echo "durchschnittliche Satzlänge e:"
echo "$ewc/$elc" | bc -l
echo "durchschnittliche Satzlänge f:"
echo "$fwc/$flc" | bc -l
