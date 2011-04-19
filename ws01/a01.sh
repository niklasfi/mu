DIR="training"

echo "Anz lfd Wörter in e:"
gunzip -c "$DIR/e" | wc -w
echo "And lfd Wörter in f:"
gunzip -c "$DIR/f" | wc -w

echo "Größe des Vokabulars in e:"
gunzip -c "$DIR/e" | sed "s/ /\n/g" | sort | uniq | wc -l
echo "Größe des Vokabulars in f:"
gunzip -c "$DIR/f" | sed "s/ /\n/g" | sort | uniq | wc -l

