Emilio Araya - 202473561-6
Nicolas Munoz - 202473637-K

Explicacion de data.map

Decidimos seguir la misma estructura que en ejemplo.map  dado que podiamos verificar constantemente si
nuestro codigo iba bien encaminado o no. Luego de tener el codigo listo, expandimos el ejemplo con la misma
logica y lo implementamos en data.map

Explicacion de nueva mecanica (Viaje en el tiempo)

La mecanica nueva consiste en una probabilidad baja de que aparezca una puerta que te permite
viajar en el tiempo al nodo abuelo. Utilizamos una funcion extra (Habitacion* abuelo) que retorna
el nodo abuelo si se acepta entrar a la puerta, ademas añadimos un contador que cuando sea mayor
o igual a 2, sera posible tener la mecanica del viaje en el tiempo. El jugador vuelve al nodo abuelo
y sigue con normalidad.