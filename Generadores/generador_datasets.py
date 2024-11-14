import random
import string

# Función para generar una cadena aleatoria de longitud n
def generar_cadena_aleatoria(longitud):
    return ''.join(random.choices(string.ascii_lowercase, k=longitud))

# Función para crear una transposición (intercambio de caracteres)
def crear_transposicion(cadena):
    if len(cadena) < 2:
        return cadena
    lista = list(cadena)
    i, j = random.sample(range(len(lista)), 2)
    lista[i], lista[j] = lista[j], lista[i]
    return ''.join(lista)


def CadenasRepetidas(cantidad):
    datasets=[]
    for _ in range(cantidad):
        cadena = generar_cadena_aleatoria(random.randint(5, 10))
        cadena_repetida = ''.join(random.choices(cadena, k=len(cadena)))
        datasets.append([cadena, cadena_repetida])
    return datasets

def CadenasVacias(cantidad):
    datasets=[]
    for _ in range(cantidad):
        datasets.append([" ", generar_cadena_aleatoria(random.randint(1, 10))])
        datasets.append([generar_cadena_aleatoria(random.randint(1, 10)), " "])
    return datasets

def CadenasTranspuestas(cantidad):
    datasets=[]
    for _ in range(cantidad):
        cadena = generar_cadena_aleatoria(random.randint(5, 15))
        cadena_transpuesta = crear_transposicion(cadena)
        datasets.append([cadena, cadena_transpuesta])
    return datasets

def guardarCadenas(filename, lista):
    # Abrir el archivo en modo escritura
    with open(filename, 'w') as file:
        file.write(f'{len(lista)}\n')
        for cadena1, cadena2 in lista:

            file.write(f'{cadena1} {cadena2}\n')



guardarCadenas("Datasets/Repetidos.txt",CadenasRepetidas(20))
guardarCadenas("Datasets/Traspuestos.txt",CadenasTranspuestas(20))
guardarCadenas("Datasets/Vacias.txt",CadenasVacias(10))
