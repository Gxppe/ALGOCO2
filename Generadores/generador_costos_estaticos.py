import numpy as np
costos1d=[
    'delete',
    'insert'
]

costos2d=[
    'transpose',
    'substitution'
]

def generar_matriz_simetrica_constante(n, valor):
    # Crear una matriz llena del valor deseado
    matriz = np.full((n, n), valor)
    
    # Poner ceros en la diagonal
    np.fill_diagonal(matriz, 0)
    return matriz

for k in costos2d:
    if k=='substitution':
        matriz_costos= generar_matriz_simetrica_constante(26,2)
    else:
        matriz_costos= generar_matriz_simetrica_constante(26,1)
    with open('Costos/Estaticos/{}.txt'.format(k),'w') as file:
        for i in range(97,123):
            for j in range(97,123):
                file.write("{} ".format(matriz_costos[i-97][j-97])) 
            file.write("\n")

for i in costos1d:
    with open('Costos/Estaticos/{}.txt'.format(i),'w') as file:
        for j in range(97,123):
            letra= chr(j)
            file.write("{} {}\n".format(letra,1)) 
    file.close()