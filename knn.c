#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <ctype.h> // isspace
#include <errno.h>
#include <math.h>

typedef struct
{
    double x, y;
    int c;
} xyc;

typedef struct
{
    double x, y;
} xy;

typedef struct
{
    double d;
    int c;
} dc;

int lineaVacia(const char *s)
{
    while (*s != '\0')
    {
        if (!isspace((unsigned char)*s))
        {
            return 0;
        }
        s++;
    }
    return 1;
}

void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

double calculoKNN(xy *arrPuntos, xyc *arrClase0, xyc *arrClase1, int linasConDatos, int cantidadPuntos, dc **arrPorPuntos, int cantidadDatosEnClase, xyc *knn)
{

    printf("\n");

    int mitad = linasConDatos / 2;

    int a = 0;
    double distancia;
    for (int i = 0; i < mitad; i++) // CLASE 0
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase0[i].x - arrPuntos[j].x;
            double y = arrClase0[i].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = arrClase0[i].c;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase0[i].x, arrClase0[i].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    int b = -1;
    for (int i = mitad; i < linasConDatos; i++) // CLASE 1
    {
        b++;
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase1[b].x - arrPuntos[j].x;
            double y = arrClase1[b].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = 1;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase1[b].x, arrClase1[b].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    printf("\n");

    // Ordenar cada fila de arrPorPuntos por distancia (d), de menor a mayor
    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < linasConDatos - 1; j++)
        {
            for (int k = 0; k < linasConDatos - j - 1; k++)
            {
                if (arrPorPuntos[i][k].d > arrPorPuntos[i][k + 1].d)
                {
                    // Intercambiar toda la estructura dc, no solo el valor de d
                    dc temp = arrPorPuntos[i][k];
                    arrPorPuntos[i][k] = arrPorPuntos[i][k + 1];
                    arrPorPuntos[i][k + 1] = temp;
                }
            }
        }
    }

    printf("\tKNN, clasificación de punto por clase:\n");
    int k = 1; // número de vecinos
    for (int i = 0; i < cantidadPuntos; i++)
    {
        // printf("\nPunto %d - %d vecinos más cercanos:\n", i, k);
        for (int j = 0; j < k; j++)
        {
            // printf("\tPunto: (%lf, %lf) Clase: %d\n", arrPuntos[i].x, arrPuntos[i].y, arrPorPuntos[i][j].c);
            knn[i].x = arrPuntos[i].x;
            knn[i].y = arrPuntos[i].y;
            knn[i].c = arrPorPuntos[i][j].c;
        }
    }
    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("\tPunto: (%lf, %lf) Clase: %d\n", knn[i].x, knn[i].y, knn[i].c);
    }
}

double calcularPromedioX(xyc *arr, int cantidadDatosEnClase)
{
    double suma = 0;
    double promedio;

    for (int i = 0; i < cantidadDatosEnClase; i++)
    {
        suma += arr[i].x;
    }

    promedio = (double)suma / cantidadDatosEnClase;
    return promedio;
}

double calcularPromedioY(xyc *arr, int cantidadDatosEnClase)
{
    double suma = 0;
    double promedio;

    for (int i = 0; i < cantidadDatosEnClase; i++)
    {
        suma += arr[i].y;
    }

    promedio = (double)suma / cantidadDatosEnClase;
    return promedio;
}

double calcularsumaXX(xyc *arr, int n)
{
    double suma = 0;
    // double sumaX = 0;
    // double sumaY = 0;

    for (int i = 0; i < n; i++)
    {
        suma += arr[i].x * arr[i].x;
    }

    return suma;
}

double calcularsumaXY(xyc *arr, int n)
{
    double suma = 0;
    // double sumaX = 0;
    // double sumaY = 0;

    for (int i = 0; i < n; i++)
    {
        suma += arr[i].x * arr[i].y;
    }

    return suma;
}

double pendiente(double promedioX, double promedioY, double sumaX2, double sumaXY, int cantidadDatosEnClase)
{
    double numerador;
    double denominador;
    double m;

    numerador = sumaXY - (cantidadDatosEnClase * promedioX * promedioY);
    denominador = sumaX2 - (cantidadDatosEnClase * (promedioX * promedioX));
    m = numerador / denominador;

    return m;
}

double ordenadaAlOrigen(double promedioX, double promedioY, double pendiente)
{
    double b = promedioY - (pendiente * promedioX);
    return b;
}

double puntoRectaDistancia(double pendiente0, double ordenada0, double pendiente1, double ordenada1, xy *arrPuntos, xyc *arrClase0, xyc *arrClase1, dc **arrPorPuntosMin, xyc *minimoscuadrados, int cantidadPuntos, int linasConDatos)
{
    double numerador;
    double denominador;
    double distancia;
    double B = -1;

    printf("\n");

    int mitad = linasConDatos / 2;

    int a = 0;
    for (int i = 0; i < mitad; i++) // CLASE 0
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            numerador = fabs(pendiente0 * arrClase0[i].x + B * arrClase0[i].y + ordenada0);
            denominador = sqrt(pendiente0 * pendiente0 + B * B);
            if (denominador == 0.0)
            {
                return -1.0;
            }
            distancia = numerador / denominador;

            arrPorPuntosMin[j][i].d = distancia;
            arrPorPuntosMin[j][i].c = arrClase0[i].c;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase0[i].x, arrClase0[i].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    int b = -1;
    for (int i = mitad; i < linasConDatos; i++) // CLASE 1
    {
        b++;
        for (int j = 0; j < cantidadPuntos; j++)
        {
            numerador = fabs(pendiente1 * arrClase0[i].x + B * arrClase0[i].y + ordenada1);
            denominador = sqrt(pendiente1 * pendiente1 + B * B);
            if (denominador == 0.0)
            {
                return -1.0;
            }
            distancia = numerador / denominador;

            arrPorPuntosMin[j][i].d = distancia;
            arrPorPuntosMin[j][i].c = 1;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase1[b].x, arrClase1[b].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    printf("\n");

    // Ordenar cada fila de arrPorPuntos por distancia (d), de menor a mayor
    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < linasConDatos - 1; j++)
        {
            for (int k = 0; k < linasConDatos - j - 1; k++)
            {
                if (arrPorPuntosMin[i][k].d > arrPorPuntosMin[i][k + 1].d)
                {
                    // Intercambiar toda la estructura dc, no solo el valor de d
                    dc temp = arrPorPuntosMin[i][k];
                    arrPorPuntosMin[i][k] = arrPorPuntosMin[i][k + 1];
                    arrPorPuntosMin[i][k + 1] = temp;
                }
            }
        }
    }

    printf("\tKNN, clasificación de punto por clase:\n");
    int k = 1; // número de vecinos
    for (int i = 0; i < cantidadPuntos; i++)
    {
        // printf("\nPunto %d - %d vecinos más cercanos:\n", i, k);
        for (int j = 0; j < k; j++)
        {
            // printf("\tPunto: (%lf, %lf) Clase: %d\n", arrPuntos[i].x, arrPuntos[i].y, arrPorPuntos[i][j].c);
            minimoscuadrados[i].x = arrPuntos[i].x;
            minimoscuadrados[i].y = arrPuntos[i].y;
            minimoscuadrados[i].c = arrPorPuntosMin[i][j].c;
        }
    }
    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("\tPunto: (%lf, %lf) Clase: %d\n", minimoscuadrados[i].x, minimoscuadrados[i].y, minimoscuadrados[i].c);
    }
}

int main()
{
    FILE *archivoDatosClases;
    archivoDatosClases = fopen("datos_clases.txt", "r");

    if (!archivoDatosClases) // Mensaje de error al abirir el archivo
    {
        printf("El archivo 'datos_clases.txt' no existe o la dirección es incorrecta");
        return 1;
    }

    int cantidadDatosEnClase;
    for (int i = 0; i < 1; i++) // Cantidad de datos en cada clase
    {
        fscanf(archivoDatosClases, "%d", &cantidadDatosEnClase);
        printf("\tEvalucaicón de %d coordenadas por cada clase\n\n", cantidadDatosEnClase);
    }

    char linea[256];
    int linasConDatos = 0;
    while (fgets(linea, 256, archivoDatosClases) != NULL) // Cantidad total de datos
    {
        if (!lineaVacia(linea))
        {
            linasConDatos++;
        }
    }
    rewind(archivoDatosClases);

    printf("Cantidad de datos: %d", linasConDatos);

    xyc arrClase0[cantidadDatosEnClase];
    xyc arrClase1[cantidadDatosEnClase];

    char enteroAcadena[256];
    sprintf(enteroAcadena, "%d", cantidadDatosEnClase);
    size_t posicionC1 = strlen(enteroAcadena); // Calcular el fin de la primera línea

    printf("\n\tClase 0:\n\n");
    fseek(archivoDatosClases, posicionC1, SEEK_SET);    // Dirigir puntero al inicio de la clase 0
    for (int i = 0; i <= cantidadDatosEnClase - 1; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoDatosClases, "%lf %lf %d", &arrClase0[i].x, &arrClase0[i].y, &arrClase0[i].c)) == 3)
        {
            printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase0[i].x, arrClase0[i].y, arrClase0[i].c);
        }
    }

    long posicionC2 = ftell(archivoDatosClases); // Calcular el fin de la primera clase
    printf("\n\n\tClase 1:\n\n");

    fseek(archivoDatosClases, posicionC2, SEEK_SET);    // Dirigir puntero al inicio de la clase 1
    for (int i = 0; i <= cantidadDatosEnClase - 1; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoDatosClases, "%lf %lf %d", &arrClase1[i].x, &arrClase1[i].y, &arrClase1[i].c)) == 3)
        {
            printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase1[i].x, arrClase1[i].y, arrClase1[i].c);
        }
    }

    fclose(archivoDatosClases);
    //------------------------------------------------------------------------------------------

    // printf("\n Clase 0:");
    double promedioX0 = calcularPromedioX(arrClase0, cantidadDatosEnClase);
    // printf("\nPormedio de x: %lf", promedioX0);

    double promedioY0 = calcularPromedioY(arrClase0, cantidadDatosEnClase);
    // printf("\nPormedio de y: %lf", promedioY0);

    double sumaX20 = calcularsumaXX(arrClase0, cantidadDatosEnClase);
    // printf("\nSuma de x²: %lf", sumaX20);

    double sumaXY0 = calcularsumaXY(arrClase0, cantidadDatosEnClase);
    // printf("\nSuma de xy: %lf", sumaXY0);

    double pendiente0 = pendiente(promedioX0, promedioY0, sumaX20, sumaXY0, cantidadDatosEnClase);
    // printf("\nPendiente: %lf", pendiente0);

    double ordenada0 = ordenadaAlOrigen(promedioX0, promedioY0, pendiente0);
    // printf("\nOrdenada: %lf", ordenada0);

    printf("\n\tLínea de ajuste de la clase 0: y=%lf x +%lf\n", pendiente0, ordenada0);

    printf("\tLínea de ajuste de la clase 0: %lfx -y +%lf=0\n", pendiente0, ordenada0);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // printf("\n Clase 1:");
    double promedioX1 = calcularPromedioX(arrClase1, cantidadDatosEnClase);
    // printf("\nPormedio de x: %lf", promedioX1);

    double promedioY1 = calcularPromedioY(arrClase1, cantidadDatosEnClase);
    // printf("\nPormedio de y: %lf", promedioY1);

    double sumaX21 = calcularsumaXX(arrClase1, cantidadDatosEnClase);
    // printf("\nSuma de x²: %lf", sumaX21);

    double sumaXY1 = calcularsumaXY(arrClase1, cantidadDatosEnClase);
    // printf("\nSuma de xy: %lf", sumaXY1);

    double pendiente1 = pendiente(promedioX1, promedioY1, sumaX21, sumaXY1, cantidadDatosEnClase);
    // printf("\nPendiente: %lf", pendiente1);

    double ordenada1 = ordenadaAlOrigen(promedioX1, promedioY1, pendiente1);
    // printf("\nOrdenada: %lf", ordenada1);

    printf("\n\tLínea de ajuste de la clase 1: y=%lf x +%lf\n", pendiente1, ordenada1);
    printf("\tLínea de ajuste de la clase 1: %lfx -y +%lf=0\n", pendiente1, ordenada1);

    //------------------------------------------------------------------------------------------------------------

    FILE *archivoPuntos;
    archivoPuntos = fopen("puntos.txt", "r");

    if (!archivoPuntos) // Mensaje de error al abirir el archivo
    {
        printf("El archivo 'puntos.txt' no existe o la dirección es incorrecta");
        return 1;
    }

    char coordenadas[256];
    int cantidadPuntos = 0;
    while (fgets(coordenadas, 256, archivoPuntos) != NULL) // Cantidad total de datos
    {
        if (!lineaVacia(coordenadas))
        {
            cantidadPuntos++;
        }
    }

    xy arrPuntos[cantidadPuntos];

    rewind(archivoPuntos);

    for (int i = 0; i <= cantidadPuntos; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoPuntos, "%lf %lf", &arrPuntos[i].x, &arrPuntos[i].y)) == 2)
        {
            // printf("\t%d \t(%f, %f) \n", i, arrPuntos[i].x, arrPuntos[i].y);
        }
    }

    dc **arrPorPuntos;
    arrPorPuntos = (dc **)malloc(cantidadPuntos * sizeof(dc *));

    if (arrPorPuntos == NULL)
    {
        printf("No se eligió un número de puntos a evaluar");
        return 1;
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        arrPorPuntos[i] = (dc *)malloc(linasConDatos * sizeof(dc));
        if (arrPorPuntos[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arrPorPuntos[j]);
            }
            free(arrPorPuntos);
            return 1;
        }
    }

    //------------------------------------------------------------------------------------

    dc **arrPorPuntosMin;
    arrPorPuntosMin = (dc **)malloc(cantidadPuntos * sizeof(dc *));

    if (arrPorPuntosMin == NULL)
    {
        printf("No se eligió un número de puntos a evaluar");
        return 1;
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        arrPorPuntosMin[i] = (dc *)malloc(linasConDatos * sizeof(dc));
        if (arrPorPuntosMin[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arrPorPuntosMin[j]);
            }
            free(arrPorPuntosMin);
            return 1;
        }
    }

    xyc knn[cantidadPuntos];
    xyc minimoscuadrados[cantidadPuntos];
    //----------------------------------------------------------------------------------------------------------------

    calculoKNN(arrPuntos, arrClase0, arrClase1, linasConDatos, cantidadPuntos, arrPorPuntos, cantidadDatosEnClase, knn);
    puntoRectaDistancia(pendiente0, ordenada0, pendiente1, ordenada1, arrPuntos, arrClase0, arrClase1, arrPorPuntosMin, minimoscuadrados, cantidadPuntos, linasConDatos);

    free(arrPorPuntos);
    free(arrPorPuntosMin);
}