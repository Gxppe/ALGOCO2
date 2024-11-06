import numpy as np

costos1d=[
    'delete',
    'insert'
]

costos2d=[
    'transpose',
    'substitution'
]

#para generar los costos en 1 dimensión
for i in costos1d:
    with open('Costos/Dinamicos/{}.txt'.format(i),'w') as file:
        for j in range(97,123):
            letra= chr(j)
            costo_letra=np.random.randint(1, 20)
            file.write("{} {}\n".format(letra,costo_letra)) 
    file.close()

def generar_matriz_simetrica(n, min_val=0, max_val=10):
    # Generar una matriz aleatoria de enteros
    matriz = np.random.randint(min_val, max_val, (n, n))
    
    # Hacerla simétrica tomando la media entre la matriz y su traspuesta
    matriz_simetrica = (matriz + matriz.T) // 2
    np.fill_diagonal(matriz_simetrica, 0)
    return matriz_simetrica

matriz_costos= generar_matriz_simetrica(26,1,30)
for k in costos2d:
    with open('Costos/Dinamicos/{}.txt'.format(k),'w') as file:
        for i in range(97,123):
            for j in range(97,123):
                file.write("{} ".format(matriz_costos[i-97][j-97])) 
            file.write("\n")