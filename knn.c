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

double distancias(xy *arrPuntos, xyc *arrClase0, xyc *arrClase1, int linasConDatos, int cantidadPuntos, dc **arrPorPuntos, int cantidadDatosEnClase, dc *knn)
{

    // for (int i = 0; i < cantidadPuntos; i++)
    // {
    //     printf("0(%lf, %lf)\n", arrClase0[i].x, arrClase0[i].y);
    // }

    // for (int i = 0; i < cantidadPuntos; i++)
    // {
    //     printf("1(%lf, %lf)\n", arrClase1[i].x, arrClase1[i].y);
    // }

    // for (int i = 0; i < cantidadPuntos; i++)
    // {
    //     printf("P(%lf, %lf)\n", arrPuntos[i].x, arrPuntos[i].y);
    // }

    int mitad = linasConDatos / 2;

    int a = 0;
    int distancia;
    for (int i = 0; i < mitad; i++) // CLASE 0
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase0[i].x - arrPuntos[j].x;
            double y = arrClase0[i].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = arrClase0[i].c;
        }
        a++;
    }

    for (int i = mitad; i < linasConDatos; i++) // CLASE 1
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase1[i].x - arrPuntos[j].x;
            double y = arrClase1[i].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = 1;
        }
        a++;
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < linasConDatos; j++)
        {
            printf("d:(%lf)\n", arrPorPuntos[i][j].d);
        }
    }

    double temp;
    for (int i = 0; i < cantidadPuntos - 1; i++)
    {
        for (int j = 0; j < linasConDatos - i - 1; j++)
        {
            if (arrPorPuntos[i][j].d > arrPorPuntos[i][j + 1].d)
            {
                temp = arrPorPuntos[i][j].d;
                arrPorPuntos[i][j].d = arrPorPuntos[i][j + 1].d;
                arrPorPuntos[i][j + 1].d = temp;
            }
        }
        printf("\n");
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("El numero mas pequeño es: %lf\n", arrPorPuntos[1][0].d);
    }

    //-------------------------------------------------------------------

    /*	Para a <- 1 Hasta n-1 Con Paso 1 Hacer
        Para b <- 1 Hasta n-a Con Paso 1 Hacer
            Si arr[b] > arr[b+1] Entonces
                temp <- arr[b]
                arr[b] <- arr[b+1]
                arr[b+1] <- temp
            FinSi
        FinPara
    FinPara */
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
    int numeroDeClases = linasConDatos / cantidadDatosEnClase; // Cantidad de arreglos, si se esperan más de dos clases

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
            // printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase0[i].x, arrClase0[i].y, arrClase0[i].c);
        }
    }

    long posicionC2 = ftell(archivoDatosClases); // Calcular el fin de la primera clase
    printf("\n\n\tClase 1:\n\n");

    fseek(archivoDatosClases, posicionC2, SEEK_SET);    // Dirigir puntero al inicio de la clase 1
    for (int i = 0; i <= cantidadDatosEnClase - 1; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoDatosClases, "%lf %lf %d", &arrClase1[i].x, &arrClase1[i].y, &arrClase1[i].c)) == 3)
        {
            // printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase1[i].x, arrClase1[i].y, arrClase1[i].c);
        }
    }
    fclose(archivoDatosClases);
    //------------------------------------------------------------------------------------------------------------
    int cantidadPuntos;
    printf("\n\tIngrese la cantidad de puntos de las cuales desea conocer su clase\n");
    scanf("\t%d", &cantidadPuntos); // Numero de arreglos

    xy arrPuntos[cantidadPuntos];

    limpiar_buffer();
    char buffer[256];
    char *endptr;
    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("\n\tPunto: %d\n\tIngrese un numero para x: ", i);

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        arrPuntos[i].x = strtod(buffer, &endptr);

        if ((errno == ERANGE && (arrPuntos[i].x == 0 || arrPuntos[i].x == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }
        // else
        // {
        //     printf("El numero ingresado correctamente es: %lf\n", arrPuntos[i].x);
        // }

        //------------------------------------------
        printf("\tIngrese un numero para y: ");

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        arrPuntos[i].y = strtod(buffer, &endptr);

        if ((errno == ERANGE && (arrPuntos[i].y == 0 || arrPuntos[i].y == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }
        // else
        // {
        //     printf("El numero ingresado correctamente es: %lf\n", arrPuntos[i].y);
        // }
        printf("\t(%lf, %lf)\n\n", arrPuntos[i].x, arrPuntos[i].y);
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

    dc knn[cantidadPuntos];

    // for (int j = 0; j < cantidadPuntos; j++)
    // {
    //     printf("(%lf, %lf)\n", arrPuntos[j].x, arrPuntos[j].y);
    // }
    // printf("\n");

    distancias(arrPuntos, arrClase0, arrClase1, linasConDatos, cantidadPuntos, arrPorPuntos, cantidadDatosEnClase, knn);

    free(arrPorPuntos);
}