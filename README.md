# Projecto_compilador_kotlin
## Tests
```sh
 make
```
Saca los tests de la carperta __test__ y guarda los códigos resultantes en __outputs__.
## UI
Se necesita de sfml, la versión 2.6. Tal vez funcione la 2.5 y otras versiones anteriores de la versión 2.
```sh
g++ ui.cpp exp.cpp parser.cpp  scanner.cpp  token.cpp visitor.cpp -o editor -lsfml-graphics -lsfml-window -lsfml-system
```
Después correr el ejecutable:
```sh
./editor
```

Me parece que solo funciona con linux porque la ejecucción de assembly corre con gcc.

## Details
- Because lists and class are expressions, they can be inside whenever expressions are used. This essentially creates problems with operations in between classes or lists because they aren't defined.
- Nested index and dot expressions don't currently work.
- Strings inside lists are assigned but can't be printed.
- The for loop can work with a different step. It also accounts for a bigger value towards a smaller value, with downtTo, however itsn't working correctly.
- Memory isn't cleaned.



