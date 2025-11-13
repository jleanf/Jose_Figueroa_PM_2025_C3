#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char marca[20];
    char modelo[20];
    int ano;
    char tipoCombustible[20];
    int ID;
    char placa[20];
    float consumoCiudad;    //ambos expresados en
    float consumoCarretera;//     L/100km
    float costoSeguro; //al año
    float costoMant; //mantenimiento por km
    float costoCambio; //de gomas por km
    int tiempoDepreciacion; //en años
    float costoVehiculo;
    float costoDepreciacion;
    float kmPromedio; //al año
    float costoLimpieza; //al año
} vehiculo;

typedef struct
{
    int codigo;
    char nombre[20];
    float costo; //por galón
} combustible;

int imprimirMenu();
int imprimirVehiculos();
void agregarVehiculo();
void modificarVehiculo();
int imprimirCombustibles();
void agregarCombustible();
void modificarCombustible();
void calcularViaje();

int main()
{
    printf("--Programa: CostoViaje");
    int res = 1;
    while (res)
    {
        res = imprimirMenu();
        switch (res)
        {
        case 1:
            printf("\n--Lista de vehiculos--\n\n");
            imprimirVehiculos();
            break;
        case 2:
            agregarVehiculo();
            break;
        case 3:
            modificarVehiculo();
            break;
        case 4:
            printf("\n--Lista de combustibles--\n\n");
            imprimirCombustibles();
            break;
        case 5:
            agregarCombustible();
            break;
        case 6:
            modificarCombustible();
            break;
        case 7:
            calcularViaje();
            break;
        case 0:
            printf("\nPrograma finalizado\n");
            break;
        default:
            printf("\n!-Eleccion invalida\n");
            break;
        }
    }
}

int imprimirMenu()
{
    int res;
    printf("\n--Menu--\n\n1-Lista de vehiculos\n2-Agregar vehiculo\n3-Modificar vehiculo\n4-Lista de combustibles\n5-Agregar combustible\n6-Modificar combustible\n7-Calcular viaje\n0-Salir\n\nSelecciona una opcion:");
    scanf("%i", &res);
    return res;
}

int imprimirVehiculos()
{
    FILE *archvehiculos = fopen("vehiculos.dat", "rb");
    vehiculo vehALeer;
    int num = 1;
    while (fread(&vehALeer, sizeof(vehiculo), 1, archvehiculos) == 1)
    {
        printf("%i-%s %s %i\n",num, vehALeer.marca, vehALeer.modelo, vehALeer.ano);
        num++;
    }
    fclose(archvehiculos);
    if(!archvehiculos || num == 1)
    {
        printf("No hay vehiculos registrados\n");
    }
    return num-1;
}

void agregarVehiculo()
{
    vehiculo vnuevo;

    printf("\n--Agregar vehiculo--\n\nMarca: ");
    scanf("%s", &vnuevo.marca);
    printf("Modelo: ");
    scanf("%s", &vnuevo.modelo);
    printf("año: ");
    scanf("%i", &vnuevo.ano);
    printf("Tipo de combustible: ");
    scanf("%s", &vnuevo.tipoCombustible);
    printf("ID: ");
    scanf("%i", &vnuevo.ID);
    printf("Placa: ");
    scanf("%s", &vnuevo.placa);
    printf("Consumo en ciudad (L/100km): ");
    scanf("%f", &vnuevo.consumoCiudad);
    printf("Consumo en carretera (L/100km): ");
    scanf("%f", &vnuevo.consumoCarretera);
    printf("Costo del seguro anual: ");
    scanf("%f", &vnuevo.costoSeguro);
    printf("Costo del mantenimiento por km: ");
    scanf("%f", &vnuevo.costoMant);
    printf("Costo del cambio de gomas por km: ");
    scanf("%f", &vnuevo.costoCambio);
    printf("Tiempo de depreciacion: ");
    scanf("%i", &vnuevo.tiempoDepreciacion);
    printf("Costo del vehiculo: ");
    scanf("%f", &vnuevo.costoVehiculo);
    printf("Costo de depreciacion: ");
    scanf("%f", &vnuevo.costoDepreciacion);
    printf("Promedio de km anual: ");
    scanf("%f", &vnuevo.kmPromedio);
    printf("Costo de limpieza promedio anual: ");
    scanf("%f", &vnuevo.costoLimpieza);

    FILE *archvehiculos = fopen("vehiculos.dat", "ab");
    fwrite(&vnuevo, sizeof(vehiculo), 1, archvehiculos);
    fclose(archvehiculos);
    printf("\n!-Vehiculo agregado\n");
}

void modificarVehiculo()
{
    vehiculo vehAMod;
    int res = 1;
    while(res)
    {
        printf("\n--Modificar vehiculo--\n\n");
        int cant = imprimirVehiculos();
        if(!cant)
        {
            return;
        }

        printf("0-Cancelar\n\nSelecciona el vehiculo a modificar:");
        scanf("%i", &res);

        int res2 = 1;
        if(!res)
        {
            return;
        }
        else if(res < 0 || res > cant)
        {
            printf("\n!-Eleccion invalida\n");
            res = -1;
            res2 = 0;
        }

        FILE *archvehiculos = fopen("vehiculos.dat", "r+b");

        fseek(archvehiculos, sizeof(vehiculo) * (res-1), 0);
        fread(&vehAMod, sizeof(vehiculo), 1, archvehiculos);

        while(res2)
        {
            printf("\n--Modificar vehiculo--\n\n");
            printf("1-Marca: %s\n", vehAMod.marca);
            printf("2-Modelo: %s\n", vehAMod.modelo);
            printf("3-Año: %i\n", vehAMod.ano);
            printf("4-Tipo de combustible: %s\n", vehAMod.tipoCombustible);
            printf("5-ID: %i\n", vehAMod.ID);
            printf("6-Placa: %s\n", vehAMod.placa);
            printf("7-Consumo en ciudad: %.2fL/100km\n", vehAMod.consumoCiudad);
            printf("8-Consumo en carretera: %.2fL/100km\n", vehAMod.consumoCarretera);
            printf("9-Costo del seguro anual: RD$%.2f\n", vehAMod.costoSeguro);
            printf("10-Costo de mantenimiento: RD$%.2f/km\n", vehAMod.costoMant);
            printf("11-Costo cambio de gomas: RD%.2f/km\n", vehAMod.costoCambio);
            printf("12-Tiempo de depreciacion (años): %i\n", vehAMod.tiempoDepreciacion);
            printf("13-Costo del vehiculo: RD$%.2f\n", vehAMod.costoVehiculo);
            printf("14-Costo de depreciacion: RD$%.2f\n", vehAMod.costoDepreciacion);
            printf("15-Km promedio (anual): %.2f\n", vehAMod.kmPromedio);
            printf("16-Costo de limpieza (anual): RD$%.2f\n", vehAMod.costoLimpieza);
            printf("0-Salir\n\nSelecciona la propiedad a modificar:");

            scanf("%i", &res2);

            switch (res2 && res > 0 && res <= cant)
            {
            case 1:
                printf("Marca actual: %s\nMarca nueva: ", vehAMod.marca);
                scanf("%s", vehAMod.marca);
                break;
            case 2:
                printf("Modelo actual: %s\nModelo nuevo: ", vehAMod.modelo);
                scanf("%s", vehAMod.modelo);
                break;
            case 3:
                printf("Año actual: %i\nAño nuevo: ", vehAMod.ano);
                scanf("%i", &vehAMod.ano);
                break;
            case 4:
                printf("Tipo combustible actual: %s\nTipo combustible nuevo: ", vehAMod.tipoCombustible);
                scanf("%s", vehAMod.tipoCombustible);
                break;
            case 5:
                printf("ID actual: %i\nID nuevo: ", vehAMod.ID);
                scanf("%i", &vehAMod.ID);
                break;
            case 6:
                printf("Placa actual: %s\nPlaca nueva: ", vehAMod.placa);
                scanf("%s", vehAMod.placa);
                break;
            case 7:
                printf("Consumo ciudad actual: %.2fL/100km\nConsumo ciudad nuevo: ", vehAMod.consumoCiudad);
                scanf("%f", &vehAMod.consumoCiudad);
                break;
            case 8:
                printf("Consumo carretera actual: %.2fL/100km\nConsumo carretera nuevo: ", vehAMod.consumoCarretera);
                scanf("%f", &vehAMod.consumoCarretera);
                break;
            case 9:
                printf("Costo seguro actual: RD$%.2f\nCosto seguro nuevo: ", vehAMod.costoSeguro);
                scanf("%f", &vehAMod.costoSeguro);
                break;
            case 10:
                printf("Costo mant. actual: RD$%.2f/km\nCosto mant. nuevo: ", vehAMod.costoMant);
                scanf("%f", &vehAMod.costoMant);
                break;
            case 11:
                printf("Costo gomas actual: RD$%.2f/km\nCosto gomas nuevo: ", vehAMod.costoCambio);
                scanf("%f", &vehAMod.costoCambio);
                break;
            case 12:
                printf("Depreciacion (años) actual: %i\nDepreciacion (años) nueva: ", vehAMod.tiempoDepreciacion);
                scanf("%i", &vehAMod.tiempoDepreciacion);
                break;
            case 13:
                printf("Costo vehiculo actual: RD$%.2f\nCosto vehiculo nuevo: ", vehAMod.costoVehiculo);
                scanf("%f", &vehAMod.costoVehiculo);
                break;
            case 14:
                printf("Costo depreciacion actual: RD$%.2f\nCosto depreciacion nuevo: ", vehAMod.costoDepreciacion);
                scanf("%f", &vehAMod.costoDepreciacion);
                break;
            case 15:
                printf("Km promedio actual: %.2f\nKm promedio nuevo: ", vehAMod.kmPromedio);
                scanf("%f", &vehAMod.kmPromedio);
                break;
            case 16:
                printf("Costo limpieza actual: RD$%.2f\nCosto limpieza nuevo: ", vehAMod.costoLimpieza);
                scanf("%f", &vehAMod.costoLimpieza);
                break;
            case 0:
                fclose(archvehiculos);
                return;
            default:
                printf("\n!-Eleccion invalida\n");
                break;
            }
            fseek(archvehiculos, sizeof(vehiculo) * (res-1), 0);
            fwrite(&vehAMod, sizeof(vehiculo), 1, archvehiculos);
        }
        fclose(archvehiculos);
    }
}

int imprimirCombustibles()
{
    FILE *archcombustibles = fopen("combustibles.dat", "rb");
    combustible combALeer;
    int num = 1;
    while (fread(&combALeer, sizeof(combustible), 1, archcombustibles) == 1)
    {
        printf("%i-%s RD$%.2f/gl codigo: %i\n",num, combALeer.nombre, combALeer.costo, combALeer.codigo);
        num++;
    }
    fclose(archcombustibles);
    if(!archcombustibles || num == 1)
    {
        printf("No hay combustibles registrados\n");
    }
    return num-1;
}

void agregarCombustible()
{
    combustible cnuevo;

    printf("\n--Agregar combustible--\n\nNombre: ");
    scanf("%s", cnuevo.nombre);
    printf("Costo por galon: ");
    scanf("%f", &cnuevo.costo);
    printf("Codigo:");
    scanf("%i", &cnuevo.codigo);

    FILE *archcombustibles = fopen("combustibles.dat", "ab");
    fwrite(&cnuevo, sizeof(combustible), 1, archcombustibles);
    fclose(archcombustibles);
    printf("\n!-Combustible agregado\n");
}

void modificarCombustible()
{
    combustible combAMod;
    int res = -1;
    while(res != 0)
    {
        printf("\n--Modificar combustible--\n\n");
        int cant = imprimirCombustibles();
        if(!cant)
        {
            return;
        }

        printf("0-Cancelar\n\nSelecciona el combustible a modificar:");
        scanf("%i", &res);

        int res2 = 1;
        if(!res)
        {
            return;
        }
        else if(res < 0 || res > cant)
        {
            printf("\n!-Eleccion invalida\n");
            res = -1;
            res2 = 0;
        }

        FILE *archcombustibles = fopen("combustibles.dat", "r+b");

        fseek(archcombustibles, sizeof(combustible) * (res-1), 0);
        fread(&combAMod, sizeof(combustible), 1, archcombustibles);

        while(res2)
        {
            printf("\n--Modificar combustible--\n\n");
            printf("1-Nombre: %s\n2-Costo por galon: %.2f\n3-Codigo: %i\n0-Salir\n\nSelecciona la propiedad a modificar:", combAMod.nombre, combAMod.costo, combAMod.codigo);
            scanf("%i", &res2);
            switch (res2)
            {
            case 1:
                printf("Nombre actual: %s\nNombre nuevo: ",combAMod.nombre);
                scanf("%s", combAMod.nombre);
                break;
            case 2:
                printf("Costo actual: RD$%.2f\nCosto nuevo: ",combAMod.costo);
                scanf("%f", &combAMod.costo);
                break;
            case 3:
                printf("Codigo actual: %i\nCodigo nuevo: ",combAMod.codigo);
                scanf("%i", &combAMod.codigo);
                break;
            case 0:
                fclose(archcombustibles);
                return;
            default:
                printf("\n!-Eleccion invalida\n");
                break;
            }
            fseek(archcombustibles, sizeof(combustible) * (res-1), 0);
            fwrite(&combAMod, sizeof(combustible), 1, archcombustibles);
        }
        fclose(archcombustibles);
    }
}

void calcularViaje()
{
    vehiculo vehSelec;
    combustible combDelVeh;
    int res = 1;

    printf("\n--Calcular viaje--\n\n");
    if(!imprimirVehiculos())
    {
        return;
    }
    printf("0-Cancelar\n\nSelecciona el vehiculo:");
    scanf("%i", &res);

    if(!res)
    {
        return;
    }

    FILE *archvehiculos = fopen("vehiculos.dat", "rb");

    fseek(archvehiculos, sizeof(vehiculo) * (res-1), 0);
    fread(&vehSelec, sizeof(vehiculo), 1, archvehiculos);
    fclose(archvehiculos);

    FILE *archcombustibles = fopen("combustibles.dat", "rb");
    int encontrado = 0;
    while (fread(&combDelVeh, sizeof(combustible), 1, archcombustibles) == 1)
    {
        if(strcmp(combDelVeh.nombre, vehSelec.tipoCombustible) == 0)
        {
            encontrado = 1;
        }
    }
    fclose(archcombustibles);
    if(!encontrado)
    {
        printf("\n!-El combustible del vehiculo no esta registrado\n");
        return;
    }

    float kms = -1;

    while(kms == -1)
    {
        float valor;
        printf("Introduzca la cantidad de kilometros del viaje: ");
        scanf("%f", &valor);
        if (valor >= 0)
        {
            kms = valor;
        }
        else
        {
            printf("\n!-El valor no puede ser negativo\n\n");
        }
    }

    float porcCiudad = -1, porcCarretera;

    while(porcCiudad == -1)
    {
        float valor;
        printf("Introduzca el porcentage de ciudad en el viaje: ");
        scanf("%f", &valor);
        if (valor >= 0 && valor <= 100)
        {
            porcCiudad = valor;
            porcCarretera = 100 - valor;
        }
        else
        {
            printf("\n!-El valor debe de estar entre 0 y 100\n\n");
        }
    }

    float costoTotal = 0, costoPorKm = 0;
    costoTotal += (((vehSelec.consumoCiudad/100) * 0.264172) * combDelVeh.costo) * (kms * porcCiudad);
    costoTotal += (((vehSelec.consumoCarretera/100) * 0.264172) * combDelVeh.costo) * (kms * porcCarretera);
    costoTotal += (vehSelec.costoSeguro/vehSelec.kmPromedio) * kms;
    costoTotal += vehSelec.costoMant * kms;
    costoTotal += vehSelec.costoCambio * kms;
    costoTotal += ((vehSelec.costoDepreciacion/vehSelec.tiempoDepreciacion)/vehSelec.kmPromedio) * kms;
    costoTotal += (vehSelec.costoLimpieza/vehSelec.kmPromedio) * kms;

    costoPorKm = costoTotal/kms;

    printf("\nCosto total: RD$%.2f\nCosto pot km: RD$%.2f\n", costoTotal, costoPorKm);
}
