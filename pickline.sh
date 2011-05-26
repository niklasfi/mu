awk '{if(NR % '$1' == '$2') print $0}' $3
