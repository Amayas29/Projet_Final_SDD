
# Compilation
gcc -c test_fonction_hachage.c
gcc -o test_fonction_hachage test_fonction_hachage.o

# Recherche la valeur qui est apparu plusieur fois et le nombre d'apparition
max=$(./test_fonction_hachage | sed -En 's/.* .* = (.*)/\1/p' | sort -n -r | uniq -c | sed -En 's/ *(.*) .*/\1/p' | sort -n -r | uniq -c)

# affichage
echo $max